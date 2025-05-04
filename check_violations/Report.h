#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <vector>
#include <iostream>

// Structure to hold a lint violation.
struct Violation {
    std::string file;
    int line;
    std::string type;
    std::string message;
};

// Collects violations and prints a report.
class Report {
public:
    std::vector<Violation> violations;

    void addViolation(const std::string& file, int line,
                      const std::string& type, const std::string& msg) {
        violations.push_back({file, line, type, msg});
    }

    // Prints all violations to stdout.
    void printReport() {
        for (auto &v : violations) {
            std::cout << v.file << ":" << v.line
                      << " [" << v.type << "] " << v.message << "\n";
        }
    }
};

#endif // REPORT_H
