// DW1000 SPI通信接口
void SPI1_Init(void) {
    // 配置SPI1引脚（PA5/6/7）和模式
    // ... 具体实现 ...
}

// 双边双向测距实现
void get_ranges_to_anchors(double ranges[]) {
    for (int i = 0; i < ANCHOR_NUM; i++) {
        // 与每个锚点进行TWR测距
        ranges[i] = perform_twr(anchors[i].id);
    }
}

// 三边定位算法
Position trilaterate(const Anchor anchors[], const double ranges[]) {
    // 实现最小二乘法定位计算
    // ... 具体算法实现 ...
}
