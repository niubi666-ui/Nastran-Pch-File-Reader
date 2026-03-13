#include "dtknastrangraphpchutils.h"
#include <algorithm>

double PchUtils::safeStod(const std::string& str) {
    if (str.empty()) return 0.0;
    bool isAllSpace = true;
    for (char c : str) {
        if (!std::isspace(static_cast<unsigned char>(c))) { isAllSpace = false; break; }
    }
    if (isAllSpace) return 0.0;
    try { return std::stod(str); }
    catch (...) { return 0.0; }
}

std::string PchUtils::getField(const std::string& line, int fieldIndex) {
    size_t start = fieldIndex * 16;
    if (start >= line.size()) return "";
    return line.substr(start, 16);
}