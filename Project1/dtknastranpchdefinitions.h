#ifndef DTKNASTRANPCHDEFINITIONS_H
#define DTKNASTRANPCHDEFINITIONS_H

#include <string>
enum class ResultCategory : int
{
    UNKNOWN = 0,
    DISPLACEMENT = 1,
    VELOCITY = 2,
    ACCELERATION = 3,
    SPCF = 4,
    STRESS = 5,
    STRAIN = 6,
    ENERGY = 7
};

enum class Component {
    NONE = 0,
    // 基础位移/加速度分量
    T1, T2, T3, R1, R2, R3,

    //幅值相位
    T1_MAG, T1_PHASE, T1_REAL, T1_IMAG,
    T2_MAG, T2_PHASE, T2_REAL, T2_IMAG,
    T3_MAG, T3_PHASE, T3_REAL, T3_IMAG,
    R1_MAG, R1_PHASE, R1_REAL, R1_IMAG,
    R2_MAG, R2_PHASE, R2_REAL, R2_IMAG,
    R3_MAG, R3_PHASE, R3_REAL, R3_IMAG,

    // 基础正应力/切应力
    SX,   // Normal-X
    SY,   // Normal-Y
    SZ,   // Normal-Z
    SXY,  // Shear-XY
    SYZ,  // Shear-YZ
    SZX,  // Shear-ZX

    // 主应力 (Principal Stresses)
    P1,   // First Principal Stress
    P2,   // Second Principal Stress
    P3,   // Third Principal Stress

    // 方向余弦 (Principal Cosines) - 针对第一主应力 P1
    P1X,  // First Principal X-Cos
    P1Y,  // First Principal Y-Cos
    P1Z,  // First Principal Z-Cos

    // 方向余弦 - 针对第二主应力 P2
    P2X,  // Second Principal X-Cos
    P2Y,  // Second Principal Y-Cos
    P2Z,  // Second Principal Z-Cos

    // 方向余弦 - 针对第三主应力 P3
    P3X,  // Third Principal X-Cos
    P3Y,  // Third Principal Y-Cos
    P3Z,  // Third Principal Z-Cos

    // 其他合成应力
    VON_MISES, // Von Mises
    P_MAX, // Max Shear (表格中的第三列可能包含)
    P_AVG, // Mean Pressure
    OCT,   // Octahedral Shear

    // 应变能分量
    STRAIN_ENERGY, ENERGY_PERCENT, ENERGY_DENSITY
};

enum class LocationType {
    SINGLE = 0,
    CENTER,     // CEN
    CORNER,     // 节点处
    Z1,         // Top
    Z2          // Bottom
};

// 原子数据条目
struct PchEntry {
    int subcase;
    int eType;
    int parentID;   // Element 或 Node ID
    int gridID;     // 关联的 Grid ID (如果没有则为0)
    ResultCategory category;
    LocationType loc;
    Component comp;
    float xVal;     // 频率、时间或 Subcase ID
    float yVal;     // 物理数值
};

// UI 导航树的节点关键字
struct ResultModule {
    std::string category; // "STRESS", "STRAIN", "DISPLACEMENT", "ENERGY"
    int elementType;      // Nastran ID (如 34, 82, 39)，标量结果为 -1

    bool operator<(const ResultModule& other) const {
        if (category != other.category) return category < other.category;
        return elementType < other.elementType;
    }
};

struct DataPoint
{
    float x;
    float y;
};

struct ResultKey
{
    uint64_t value;

    ResultKey(ResultCategory cat, int sub, int etype, int pID, int gID, LocationType loc, Component comp) {
        // 总计 64 位分配：
        // Category(4), Subcase(10), elementType(9), pID(20), gID(11), Loc(2), Comp(8)
        value = (static_cast<uint64_t>(static_cast<int>(cat) & 0xF) << 60) |    // 0-15
            (static_cast<uint64_t>(sub & 0x3FF) << 50) |  // 0-1023
            (static_cast<uint64_t>(etype & 0x1FF) << 41) | // 0-511
            (static_cast<uint64_t>(pID & 0xFFFFF) << 21) | // 0-1,048,575 (1百万ID)
            (static_cast<uint64_t>(gID & 0x7FF) << 10) |   // 0-2047
            (static_cast<uint64_t>(static_cast<int>(loc) & 0x3) << 8) |      // 0-3 (TOP, BOT, MID, CENTER)
            (static_cast<uint64_t>(static_cast<int>(comp) & 0xFF));          // 0-255 (支持100多种分量)
    }

    bool operator==(const ResultKey& other) const { return value == other.value; }
};

struct ResultKeyHasher
{
    size_t operator()(const ResultKey& k) const { return static_cast<size_t>(k.value); }
};
#endif