#include "stm32f10x.h"
#include "dw1000.h"
#include <stdio.h>
#include "arm_math.h"  // 添加ARM数学库

// UWB锚点配置
#define ANCHOR_NUM 3
const Anchor anchors[ANCHOR_NUM] = {
    {0x0001, 1.0, 2.0, 0.0},  // 锚点1坐标
    {0x0002, 4.0, 5.0, 0.0},  // 锚点2坐标
    {0x0003, 7.0, 2.0, 0.0}   // 锚点3坐标
};

// 硬件初始化
// 添加头文件
#include "ssd1306.h"  // 确保路径正确

// 修改硬件初始化
void HW_Init(void) {
    // 初始化SPI（连接DW1000）
    SPI1_Init();
    
    // 初始化UART（用于调试输出）
    USART1_Init(115200);
    
    // 初始化DW1000
    dwt_initialize();
    
    // 配置中断引脚（EXTI）
    NVIC_Configuration();
    
    // 新增I2C初始化
    I2C1_Init();
    
    // 初始化OLED
    OLED_Init();
    OLED_Clear();
    OLED_Display_On();
}

// 新增显示更新函数
void Update_Display(float x, float y) {
    char buffer[16];
    
    OLED_Clear();
    
    // 显示X坐标
    OLED_Set_Pos(0,0);
    sprintf(buffer, "X:%.2fm", x);
    OLED_ShowString(buffer);
    
    // 显示Y坐标
    OLED_Set_Pos(0,2);
    sprintf(buffer, "Y:%.2fm", y);
    OLED_ShowString(buffer);
    
    // 显示状态
    OLED_Set_Pos(0,5);
    OLED_ShowString("UWB OK");
}

// 修改定位任务
void Positioning_Task(void) {
    static uint32_t last_tick = 0;
    if (HAL_GetTick() - last_tick > 100) {
        last_tick = HAL_GetTick();
        
        double ranges[ANCHOR_NUM];
        get_ranges_to_anchors(ranges);
        
        Position pos = trilaterate(anchors, ranges);
        
        // 新增卡尔曼滤波处理
        float32_t z[2] = {pos.x, pos.y};       // 观测值
        float32_t x[4] = {pos.x, pos.y, 0, 0}; // 初始状态
        
        // 预测步骤
        arm_kalman_predict_f32(&kf.instance, kf.A, x, kf.Q);
        // 更新步骤
        arm_kalman_update_f32(&kf.instance, z, kf.H, kf.R, x);
        
        // 输出滤波后的结果
        printf("Position: (%.2f, %.2f)\r\n", x[0], x[1]);
        Update_Display(x[0], x[1]);  // 新增OLED显示
    }
}

int main(void) {
    HW_Init();
    Kalman_Init();  // 初始化滤波器
    
    while(1) {
        Positioning_Task();
    }
}