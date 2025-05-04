#ifndef UNINITIALIZED_REGISTER_VIOLATION_H
#define UNINITIALIZED_REGISTER_VIOLATION_H

#include "GlobalFunctions.h"
#include "Report.h"
#include <regex>
#include <unordered_set>

// Detects use of uninitialized registers.
class UninitializedRegisterViolation {
public:
    static void check(const std::string& filename,
                      const std::vector<std::string>& lines,
                      Report& report) {
        std::unordered_set<std::string> regs, assigned, used;
        std::regex regDecl(R"(\breg\s+(\w+))");

        // First pass: record all declared regs.
        for (const auto& line : lines) {
            std::smatch m;
            if (std::regex_search(line, m, regDecl))
                regs.insert(m[1]);
        }

        // Second pass: find assignments and usages.
        std::regex assignLHS(R"((\w+)\s*(?:<=|=)\s*(.*);)");
        for (int i = 0; i < (int)lines.size(); ++i) {
            const std::string& line = lines[i];
            std::smatch m;
            if (std::regex_search(line, m, assignLHS)) {
                std::string lhs = m[1], rhs = m[2];
                if (regs.count(lhs)) assigned.insert(lhs);
                for (auto& r : regs)
                    if (rhs.find(r) != std::string::npos) used.insert(r);
            }
        }

        // Any used reg not in assigned is a violation.
        for (auto& r : used) {
            if (!assigned.count(r))
                report.addViolation(filename, 0, "UninitializedRegister",
                                    "Register '" + r + "' used without prior assignment");
        }
    }
};

#endif // UNINITIALIZED_REGISTER_VIOLATION_H
