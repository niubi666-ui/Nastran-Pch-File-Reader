#ifndef DTKNASTRANGRAPHPCHDATASTORE_H
#define DTKNASTRANGRAPHPCHDATASTORE_H
#include "dtknastranpchdefinitions.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <set>

class PchDataStore {
public:
    void finalize();
    void addEntry(const PchEntry& e);
    std::vector<DataPoint> getCurveData(
        ResultCategory category,
        int subcase,
        int elementType,
        int pointID,
        int gridID,
        LocationType location,
        Component component);
private:
    // 核心数据库：Key对应唯一曲线，Value对应曲线上的所有点
    std::unordered_map<ResultKey, std::vector<DataPoint>, ResultKeyHasher> m_db;
    std::map<int, std::set<ResultModule>> m_uiNavigationTree;
    bool m_isSorted = false;
};
#endif
