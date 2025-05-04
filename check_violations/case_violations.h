#ifndef CASE_VIOLATIONS_H
#define CASE_VIOLATIONS_H

#include "GlobalFunctions.h"
#include "Report.h"
#include <regex>
#include <unordered_set>

// Detects full-case and parallel-case issues in case statements.
class CaseViolations {
public:
    static void check(const std::string& filename,
                      const std::vector<std::string>& lines,
                      Report& report) {
        std::regex caseStart(R"(\bcase\s*\((\w+)\))");
        std::regex caseItem(R"(([^:]+):)");
        int n = lines.size();
        for (int i = 0; i < n; ++i) {
            std::smatch mCase;
            if (std::regex_search(lines[i], mCase, caseStart)) {
                bool hasDefault = false;
                std::unordered_set<std::string> seen;
                int j = i+1;
                while (j < n && lines[j].find("endcase") == std::string::npos) {
                    const std::string& L = lines[j];
                    if (L.find("default:") != std::string::npos) hasDefault = true;
                    std::smatch mItem;
                    if (std::regex_search(L, mItem, caseItem)) {
                        auto labels = GlobalFunctions::split(mItem[1], ',');
                        for (auto& lab : labels) {
                            lab = std::regex_replace(lab, std::regex("\\s+"), "");
                            if (seen.count(lab))
                                report.addViolation(filename, j+1, "NonParallelCase",
                                    "Duplicate case item '" + lab + "'");
                            seen.insert(lab);
                        }
                    }
                    j++;
                }
                if (!hasDefault)
                    report.addViolation(filename, i+1, "NonFullCase",
                        "Case statement missing default (non-full case)");
                i = j;
            }
        }
    }
};

#endif // CASE_VIOLATIONS_H
