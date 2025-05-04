#include "gtest/gtest.h"
#include "../GlobalFunctions.h"
#include "../Report.h"
#include "../uninitialized_register_violation.h"
#include <fstream>

TEST(UninitializedRegTest, DetectUninitializedUsage) {
    std::ofstream ofs("sample_uninit.v");
    ofs << "module m; reg a, b; wire c;\n";
    ofs << "assign c = a; // 'a' never assigned\n";
    ofs << "endmodule\n";
    ofs.close();

    auto lines = GlobalFunctions::readFile("sample_uninit.v");
    Report rep;
    UninitializedRegisterViolation::check("sample_uninit.v", lines, rep);
    ASSERT_FALSE(rep.violations.empty());
    EXPECT_EQ(rep.violations[0].type, "UninitializedRegister");
}
