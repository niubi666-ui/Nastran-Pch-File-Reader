#include "dtknastrangraphpchparser.h"
#include <fstream>

PchParser::PchParser(PchDataStore& store) : m_store(store) {}

bool PchParser::parse(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line.find("$SUBCASE ID =") != std::string::npos) m_currentSubcase = std::stoi(line.substr(line.find("=") + 1));
        if (line.find("$ELEMENT STRAINS") != std::string::npos) m_currentCategory = "STRAIN";
        if (line.find("$DISPLACEMENTS") != std::string::npos) m_currentCategory = "DISPLACEMENT";
        if (line.find("$ELEMENT STRAIN ENERGIES") != std::string::npos) {
            m_currentCategory = "ENERGY";
            m_currentElementType = -1; // 标量标识
            processBlock(file);
        }
        if (line.find("$FREQUENCY =") != std::string::npos) m_currentXVal = m_utils.safeStod(line.substr(line.find("=") + 1));
        if (line.find("ELEMENT TYPE =") != std::string::npos) {
            m_currentElementType = std::stoi(line.substr(line.find("=") + 1));
            processBlock(file);
        }
    }
    m_store.finalize();
    return true;
}

void PchParser::processBlock(std::ifstream& file) {
    m_store.registerMetadata(m_currentSubcase, m_currentCategory, m_currentElementType);
    ElementLayout layout = m_mapping.getLayout(m_currentElementType, m_currentCategory);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '$') break;
        if (line.compare(0, 6, "-CONT-") != 0) parseElementData(file, line, layout);
    }
}

void PchParser::parseElementData(std::ifstream& file, const std::string& firstLine, const ElementLayout& layout) {
    std::string currentLine = firstLine;
    int parentID = (int)m_utils.safeStod(m_utils.getField(currentLine, 0));

    for (int r = 0; r < layout.repeatCount; ++r) {
        if (r > 0) {
            if (!std::getline(file, currentLine)) break;
        }

        int currentGridID = 0;
        std::string gridStr = m_utils.getField(currentLine, 1);
        if (gridStr.find("CEN") == std::string::npos) {
            currentGridID = (int)m_utils.safeStod(gridStr);
        }

        int wordCounter = 1;
        while (wordCounter < layout.wordsPerPoint) {
            // C++11 兼容写法
            bool isCont = (currentLine.compare(0, 6, "-CONT-") == 0);
            int fieldsInLine = isCont ? 4 : 3;
            int startIdx = isCont ? 1 : 2;

            for (int f = 0; f < fieldsInLine; ++f) {
                wordCounter++;

                if (layout.wordToInfo.count(wordCounter)) {
                    auto& info = layout.wordToInfo.at(wordCounter);
                    double val = m_utils.safeStod(m_utils.getField(currentLine, startIdx + f));

                    PchEntry entry;
                    entry.subcase = m_currentSubcase;
                    entry.parentID = parentID;
                    entry.gridID = currentGridID;
                    entry.loc = info.loc;
                    entry.comp = info.comp;
                    entry.xVal = (float)m_currentXVal;
                    entry.yVal = (float)val;

                    m_store.addEntry(entry);
                }

                if (wordCounter >= layout.wordsPerPoint) break;
            }

            if (wordCounter < layout.wordsPerPoint) {
                if (!std::getline(file, currentLine)) break;
            }
        }
    }
}