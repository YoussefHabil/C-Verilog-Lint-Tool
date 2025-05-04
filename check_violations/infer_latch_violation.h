#ifndef INFER_LATCH_VIOLATION_H
#define INFER_LATCH_VIOLATION_H

#include "GlobalFunctions.h"
#include "Report.h"
#include <regex>

// Detects inferred latches due to incomplete assignments.
class InferLatchViolation {
public:
    static void check(const std::string& filename,
                      const std::vector<std::string>& lines,
                      Report& report) {
        std::regex alwaysComb(R"(\balways\s*@\s*\([^)]*\))");
        std::regex posedge(R"(posedge|negedge)");
        int lineNum = 0;
        while (lineNum < (int)lines.size()) {
            const std::string& line = lines[lineNum];
            if (std::regex_search(line, alwaysComb) &&
                !std::regex_search(line, posedge)) {
                int ifCount = 0, elseCount = 0, depth = 0;
                if (line.find("begin") != std::string::npos) depth++;
                int j = lineNum;
                while (j < (int)lines.size()) {
                    const std::string& l = lines[j];
                    if (l.find("begin") != std::string::npos) depth++;
                    if (l.find("end")   != std::string::npos) depth--;
                    if (l.find("if")    != std::string::npos) ifCount++;
                    if (l.find("else")  != std::string::npos) elseCount++;
                    j++;
                    if (depth <= 0) break;
                }
                if (ifCount > elseCount)
                    report.addViolation(filename, lineNum+1, "InferredLatch",
                        "Combinational block may infer latch (missing else/default)");
                lineNum = j;
            } else {
                lineNum++;
            }
        }
    }
};

#endif // INFER_LATCH_VIOLATION_H
