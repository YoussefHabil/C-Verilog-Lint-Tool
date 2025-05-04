#include "GlobalFunctions.h"
#include "Report.h"
#include "arithmetic_overflow_violation.h"
#include "uninitialized_register_violation.h"
#include "infer_latch_violation.h"
#include "case_violations.h"
#include "multidriven_register_violation.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: lint <file1> [file2 ...]\n";
        return 1;
    }
    Report report;
    for (int i = 1; i < argc; ++i) {
        std::string fn = argv[i];
        auto lines = GlobalFunctions::readFile(fn);
        ArithmeticOverflowViolation::check(fn, lines, report);
        UninitializedRegisterViolation::check(fn, lines, report);
        InferLatchViolation::check(fn, lines, report);
        CaseViolations::check(fn, lines, report);
        MultidrivenRegisterViolation::check(fn, lines, report);
    }
    report.printReport();
    return 0;
}
