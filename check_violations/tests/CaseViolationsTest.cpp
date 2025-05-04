#include "gtest/gtest.h"
#include "../GlobalFunctions.h"
#include "../Report.h"
#include "../case_violations.h"
#include <fstream>

TEST(CaseViolationsTest, NonFullAndParallel) {
    std::ofstream ofs("sample_case.v");
    ofs << "module c; reg [1:0] sel; reg out;\n";
    ofs << "always @(*) begin\n";
    ofs << "  case(sel)\n";
    ofs << "    2'b00: out = 0;\n";
    ofs << "    2'b00: out = 1; // duplicate label\n";
    ofs << "  endcase\n";
    ofs << "end\nendmodule\n";
    ofs.close();

    auto lines = GlobalFunctions::readFile("sample_case.v");
    Report rep;
    CaseViolations::check("sample_case.v", lines, rep);
    // Expect both NonFullCase (no default) and NonParallelCase (duplicate 2'b00)
    bool hasNonFull = false, hasNonParallel = false;
    for (auto &v : rep.violations) {
        if (v.type == "NonFullCase") hasNonFull = true;
        if (v.type == "NonParallelCase") hasNonParallel = true;
    }
    EXPECT_TRUE(hasNonFull);
    EXPECT_TRUE(hasNonParallel);
}
