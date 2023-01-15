#ifndef _OLED12864_H_
#define _OLED12864_H_

#include <stdint.h>

/**************************************************************************************
 * 基于stm32f103zet6的OLED12864 4Pin_SPI
 * OLED12864驱动芯片:SSD1306
 * 使用了self_type.h
 * 方便引脚初始化 和 输出
 * 
 * 2021/10/16: 将spi分离了出去,留出了spi接口OLED12864_SPI_Send_Byte()
 * 
 * Creat by: 庞碧璋
 * Github: https://github.com/CodingBugStd
 * csdn:   https://blog.csdn.net/RampagePBZ
 * Encoding: utf-8
 * date:    2021/6/22
 * last date: 2021/10/16
 * 
 * 备注:在移植时注意数据类型的依赖!
**************************************************************************************/

#define x_MAX           128
#define y_MAX           64
#define page_MAX        8

//OLED模式
#define USE_POINT_CRT       1   //使用像素点级别操作
//todo 
// #define oled_printf         0   //printf重定向到oled屏幕
// #define oled_printf_line    0   //printf重定向到oled第一行

#define OLED_CMD    0
#define OLED_DATA   1

//初始化
void OLED12864_Init(void);
void OLED12864_GPIO_Init(void);
void OLED12864_Hard_Reset(void);

void OLED12864_Send_Byte(uint8_t dat,uint8_t cmd);
void OLED12864_Send_NumByte(const uint8_t*dat,uint16_t len,uint8_t cmd);

void OLED12864_Refresh(void);
void OLED12864_Set_Position(uint8_t page,uint8_t x);
void OLED12864_Clear_Sbuffer(void);   
void OLED12864_Clear(void);

//y坐标位置和高度都以page为单位的图形操作
void OLED12864_Clear_PageBlock(uint8_t page,uint8_t x,uint8_t len);
void OLED12864_Clear_Page(uint8_t page);
void OLED12864_Show_Char(uint8_t page,uint8_t x,uint8_t chr,uint8_t size);
void OLED12864_Show_String(uint8_t page,uint8_t x,uint8_t*str,uint8_t size);
uint8_t OLED12864_Show_Num(uint8_t page,uint8_t x,int num,uint8_t size);    //返回num的长度
uint8_t OLED12864_Show_fNum(uint8_t page,uint8_t x,double num,uint8_t size,uint8_t d_len);

    //以单个像素点为单位的图形操作
    #if USE_POINT_CRT == 1
    void OLED12864_Draw_Point(uint8_t x,uint8_t y,uint8_t bit);             //<--像素点操作底层函数
    void OLED12864_Draw_Line(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
    void OLED12864_Draw_Rect(uint8_t x,uint8_t y,uint8_t len,uint8_t hight);
    //*img 以字节竖直,高位在上,数据水平
    void OLED12864_Draw_Img(uint8_t x,uint8_t y,uint8_t len,uint8_t hight,uint8_t*img);
    void OLED12864_Draw_aImg(uint8_t x,uint8_t y,uint8_t*img);  //自带图形大小识别
    #endif  //USE_POINT_CRT

#endif

