#pragma once

// 定义锚点结构体
typedef struct {
    uint16_t id;
    double x;
    double y;
    double z;
} Anchor;

// 定义位置结构体
typedef struct {
    double x;
    double y;
} Position;

// 声明外部变量
extern const Anchor anchors[];
extern const int ANCHOR_NUM;

// 函数声明
void SPI1_Init(void);
void get_ranges_to_anchors(double ranges[]);
Position trilaterate(const Anchor anchors[], const double ranges[]);
