#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>

// Utility functions for file I/O and string operations.
class GlobalFunctions {
public:
    // Reads a file into a vector of lines (removing // comments).
    static std::vector<std::string> readFile(const std::string& filename) {
        std::vector<std::string> lines;
        std::ifstream fin(filename);
        if (!fin) return lines;
        std::string line;
        while (std::getline(fin, line)) {
            // Remove single-line comments
            size_t pos = line.find("//");
            if (pos != std::string::npos) line = line.substr(0, pos);
            lines.push_back(line);
        }
        return lines;
    }
    // Splits a string by a delimiter.
    static std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            if (!item.empty())
                elems.push_back(item);
        }
        return elems;
    }
};

#endif // GLOBAL_FUNCTIONS_H
