#ifndef ARITHMETIC_OVERFLOW_VIOLATION_H
#define ARITHMETIC_OVERFLOW_VIOLATION_H

#include "GlobalFunctions.h"
#include "Report.h"
#include <regex>
#include <unordered_map>
#include <algorithm>

// Detects arithmetic overflow in assignments.
class ArithmeticOverflowViolation {
public:
    // Scans lines of Verilog code and reports overflow violations.
    static void check(const std::string& filename,
                      const std::vector<std::string>& lines,
                      Report& report) {
        std::unordered_map<std::string,int> bitWidth;
        std::regex regDecl(R"(\breg\s*\[(\d+):0\]\s+(\w+))");
        std::regex wireDecl(R"(\bwire\s*\[(\d+):0\]\s+(\w+))");
        std::regex singleReg(R"(\breg\s+(\w+))");
        std::regex singleWire(R"(\bwire\s+(\w+))");

        // First pass: parse declarations to record bit widths.
        for (const auto& line : lines) {
            std::smatch m;
            if (std::regex_search(line, m, regDecl)) {
                int msb = std::stoi(m[1]);
                bitWidth[m[2]] = msb + 1;
            }
            else if (std::regex_search(line, m, wireDecl)) {
                int msb = std::stoi(m[1]);
                bitWidth[m[2]] = msb + 1;
            }
            else if (std::regex_search(line, m, singleReg)) {
                bitWidth[m[1]] = 1;
            }
            else if (std::regex_search(line, m, singleWire)) {
                bitWidth[m[1]] = 1;
            }
        }

        // Second pass: find assignments and check for + or -.
        std::regex assignExpr(R"((\w+)\s*(?:<=|=)\s*(.*);)");
        for (int i = 0; i < (int)lines.size(); ++i) {
            const std::string& line = lines[i];
            std::smatch m;
            if (std::regex_search(line, m, assignExpr)) {
                std::string lhs = m[1];
                std::string expr = m[2];
                // Check for '+' operator
                auto pos = expr.find('+');
                if (pos != std::string::npos) {
                    std::string left = expr.substr(0, pos);
                    std::string right = expr.substr(pos+1);
                    // Remove whitespace
                    left = std::regex_replace(left, std::regex("\\s+"), "");
                    right = std::regex_replace(right, std::regex("\\s+"), "");

                    int w1 = 0, w2 = 0;
                    if (bitWidth.count(left)) w1 = bitWidth[left];
                    else if (left.find('\'') != std::string::npos) {
                        // e.g. "8'd255"
                        int litWidth = std::stoi(left);
                        w1 = litWidth;
                    }
                    if (bitWidth.count(right)) w2 = bitWidth[right];
                    else if (right.find('\'') != std::string::npos) {
                        int litWidth = std::stoi(right);
                        w2 = litWidth;
                    }
                    int needed = std::max(w1, w2) + 1;
                    int lhsw = bitWidth.count(lhs) ? bitWidth[lhs] : 1;
                    if (lhsw < needed) {
                        report.addViolation(filename, i+1, "ArithmeticOverflow",
                            "Overflow in '" + expr + "' assigned to '" + lhs + "'");
                    }
                }
                // Check for '-' similarly
                pos = expr.find('-');
                if (pos != std::string::npos) {
                    std::string left = expr.substr(0, pos);
                    std::string right = expr.substr(pos+1);
                    left = std::regex_replace(left, std::regex("\\s+"), "");
                    right = std::regex_replace(right, std::regex("\\s+"), "");

                    int w1 = 0, w2 = 0;
                    if (bitWidth.count(left)) w1 = bitWidth[left];
                    else if (left.find('\'') != std::string::npos) {
                        int litWidth = std::stoi(left);
                        w1 = litWidth;
                    }
                    if (bitWidth.count(right)) w2 = bitWidth[right];
                    else if (right.find('\'') != std::string::npos) {
                        int litWidth = std::stoi(right);
                        w2 = litWidth;
                    }
                    int needed = std::max(w1, w2) + 1;
                    int lhsw = bitWidth.count(lhs) ? bitWidth[lhs] : 1;
                    if (lhsw < needed) {
                        report.addViolation(filename, i+1, "ArithmeticOverflow",
                            "Overflow in '" + expr + "' assigned to '" + lhs + "'");
                    }
                }
            }
        }
    }
};

#endif // ARITHMETIC_OVERFLOW_VIOLATION_H
