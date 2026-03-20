#include "dtknastranpchdefinitions.h"
#include "dtknastrangraphpchdatastore.h"
#include <algorithm>

void PchDataStore::addEntry(const PchEntry& entry) {
    // 将 PchEntry 的维度转换为 Key
    ResultKey key(entry.category, entry.subcase, entry.eType,entry.parentID, entry.gridID,entry.loc, entry.comp);

    m_db[key].push_back({ entry.xVal, entry.yVal });
}

std::vector<DataPoint> PchDataStore::getCurveData(ResultCategory category,int subcase, int eType, int pID, int gID, LocationType loc, Component comp)
{
    ResultKey key(category, subcase, eType, pID, gID, loc, comp);
    if (m_db.find(key) != m_db.end())
    {
        return m_db[key];
    }
    return {};
}

void PchDataStore::finalize()
{
    for (auto& pair : m_db)
    {
        std::sort(pair.second.begin(), pair.second.end(), [](const DataPoint& a, const DataPoint& b) {
            return a.x < b.x;
            });
    }
}
