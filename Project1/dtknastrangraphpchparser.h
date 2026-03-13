#ifndef DTKNASTRANGRAPHPCHPARSER_H
#define DTKNASTRANGRAPHPCHPARSER_H
#include "dtknastrangraphpchdatastore.h"
#include "dtknastrangraphpchutils.h"
#include "dtknastrangraphpchmapping.h"

class PchParser {
public:
    PchParser(PchDataStore& store);
    bool parse(const std::string& filePath);

private:
    void processBlock(std::ifstream& file);
    void parseElementData(std::ifstream& file, const std::string& firstLine, const ElementLayout& layout);

    PchDataStore& m_store;
    PchUtils m_utils;
    PchMapping m_mapping;
    int m_currentSubcase = 0;
    int m_currentElementType = 0;
    std::string m_currentCategory;
    double m_currentXVal = 0.0;
};
#endif