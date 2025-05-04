#include "gtest/gtest.h"
#include "../GlobalFunctions.h"
#include "../Report.h"
#include "../infer_latch_violation.h"
#include <fstream>

TEST(InferLatchTest, MissingElseCausesLatch) {
    std::ofstream ofs("sample_latch.v");
    ofs << "module l; reg out; wire in; always @(*) begin if (in) out = 1; end endmodule\n";
    ofs.close();

    auto lines = GlobalFunctions::readFile("sample_latch.v");
    Report rep;
    InferLatchViolation::check("sample_latch.v", lines, rep);
    ASSERT_FALSE(rep.violations.empty());
    EXPECT_EQ(rep.violations[0].type, "InferredLatch");
}
