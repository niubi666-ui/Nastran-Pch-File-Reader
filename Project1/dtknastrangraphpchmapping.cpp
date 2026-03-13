#include "dtknastrangraphpchmapping.h"

ElementLayout PchMapping::getLayout(int elementType, const std::string& cat) {
    ElementLayout layout;
    if (cat == "STRAIN" || cat == "STRESS") {
        if (elementType == 82) { // QUADR
            layout.repeatCount = 5; // CEN + 4 Corners
            layout.wordsPerPoint = 19;
            layout.wordToInfo[5] = { Component::SX, LocationType::Z1 };
            layout.wordToInfo[13] = { Component::SX, LocationType::Z2 };
            // ... 继续添加其他 Word 映射
        }
        else if (elementType == 34) { // BAR
            layout.repeatCount = 1;
            layout.wordsPerPoint = 10;
            layout.wordToInfo[3] = { Component::SX, LocationType::SINGLE };
        }
    }
    else if (cat == "ENERGY") { // 应变能
        layout.repeatCount = 1;
        layout.wordsPerPoint = 4;
        layout.wordToInfo[2] = { Component::STRAIN_ENERGY, LocationType::SINGLE };
        layout.wordToInfo[3] = { Component::ENERGY_PERCENT, LocationType::SINGLE };
    }
    else if (cat == "DISPLACEMENT") {
        layout.repeatCount = 1;
        layout.wordsPerPoint = 8;
        layout.wordToInfo[3] = { Component::T1, LocationType::SINGLE };
        layout.wordToInfo[4] = { Component::T2, LocationType::SINGLE };
        layout.wordToInfo[5] = { Component::T3, LocationType::SINGLE };
    }
    return layout;
}