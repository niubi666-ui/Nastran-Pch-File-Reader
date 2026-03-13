#ifndef DTKNASTRANGRAPHPCHUTILS_H
#define DTKNASTRANGRAPHPCHUTILS_H
#include <string>

class PchUtils {
public:
    double safeStod(const std::string& str);
    std::string getField(const std::string& line, int fieldIndex);
};
#endif

