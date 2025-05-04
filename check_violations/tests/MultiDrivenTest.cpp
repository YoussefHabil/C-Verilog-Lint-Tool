#include "gtest/gtest.h"
#include "../GlobalFunctions.h"
#include "../Report.h"
#include "../multidriven_register_violation.h"
#include <fstream>

TEST(MultiDrivenTest, DetectMultiDriver) {
    std::ofstream ofs("sample_multi.v");
    ofs << "module m; reg a;\n";
    ofs << "always @(*) a = 0;\n";
    ofs << "always @(*) a = 1;\n";  // 'a' assigned twice
    ofs << "endmodule\n";
    ofs.close();

    auto lines = GlobalFunctions::readFile("sample_multi.v");
    Report rep;
    MultidrivenRegisterViolation::check("sample_multi.v", lines, rep);
    ASSERT_FALSE(rep.violations.empty());
    EXPECT_EQ(rep.violations[0].type, "MultiDrivenRegister");
}
