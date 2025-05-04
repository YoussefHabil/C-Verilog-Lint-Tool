#include "gtest/gtest.h"
#include "../GlobalFunctions.h"
#include "../Report.h"
#include "../arithmetic_overflow_violation.h"
#include <fstream>

TEST(ArithmeticOverflowTest, DetectOverflow) {
    // Create a Verilog file where 4-bit sum overflows (needs 5 bits)
    std::ofstream ofs("sample_overflow.v");
    ofs << "module top; reg [3:0] a, b; wire [3:0] sum;\n";
    ofs << "assign sum = a + b;\n";
    ofs << "endmodule\n";
    ofs.close();

    auto lines = GlobalFunctions::readFile("sample_overflow.v");
    Report rep;
    ArithmeticOverflowViolation::check("sample_overflow.v", lines, rep);
    // Expect one overflow violation
    ASSERT_FALSE(rep.violations.empty());
    EXPECT_EQ(rep.violations[0].type, "ArithmeticOverflow");
}
