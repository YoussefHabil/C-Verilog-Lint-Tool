#ifndef MULTIDRIVEN_REGISTER_VIOLATION_H
#define MULTIDRIVEN_REGISTER_VIOLATION_H

#include "GlobalFunctions.h"
#include "Report.h"
#include <regex>
#include <unordered_map>
#include <unordered_set>

// Detects registers assigned in multiple blocks.
class MultidrivenRegisterViolation {
public:
    static void check(const std::string& filename,
                      const std::vector<std::string>& lines,
                      Report& report) {
        std::unordered_map<std::string,int> count;
        std::regex regDecl(R"(\breg\s+(\w+))");
        std::regex assignLHS(R"((\w+)\s*(?:<=|=)\s*.*;)");
        std::unordered_set<std::string> regs;

        // Record all regs
        for (auto& L : lines) {
            std::smatch m;
            if (std::regex_search(L, m, regDecl)) {
                regs.insert(m[1]);
                count[m[1]] = 0;
            }
        }
        // Count assignments
        for (auto& L : lines) {
            std::smatch m;
            if (std::regex_search(L, m, assignLHS)) {
                std::string lhs = m[1];
                if (regs.count(lhs)) count[lhs]++;
            }
        }
        // Report multi-driven
        for (auto& kv : count) {
            if (kv.second > 1)
                report.addViolation(filename, 0, "MultiDrivenRegister",
                    "Register '" + kv.first + "' has multiple drivers (" +
                    std::to_string(kv.second) + ")");
        }
    }
};

#endif // MULTIDRIVEN_REGISTER_VIOLATION_H
