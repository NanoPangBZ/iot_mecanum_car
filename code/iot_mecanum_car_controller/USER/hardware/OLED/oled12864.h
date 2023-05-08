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

#define OLED12864_DMA_ENABLE    1

#define x_MAX           128
#define y_MAX           64
#define page_MAX        8

#define OLED_CMD    0
#define OLED_DATA   1

typedef enum{
    ASSIC_8x6 = 0,
    ASSIC_16X8 = 1
}font_t;

//初始化
void OLED12864_Init(void);
void OLED12864_Hard_Reset(void);
#if OLED12864_DMA_ENABLE
void OLED12864_DMA_Sync_Start(void);
#endif  //OLED12864_DMA_ENABLE

void OLED12864_Test( void );

void OLED12864_Flush( void );
void OLED12864_Draw_Point( uint8_t x, uint8_t y , uint8_t bit );
void OLED12864_Draw_Map( uint8_t sx , uint8_t sy , uint8_t ex , uint8_t ey , uint8_t* map );
void OLED12864_Fill_Block( uint8_t sx , uint8_t sy , uint8_t width , uint8_t height , uint8_t bit );
void OLED12864_Draw_Line(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void OLED12864_Clear( void );
void OLED12864_Clear_Block( uint8_t sx , uint8_t sy , uint8_t width , uint8_t height );
void OLED12864_Fast_Clear_Block( uint8_t page , uint8_t x , uint8_t len );

void OLED12864_Show_Char( char chr , uint8_t x , uint8_t y , font_t font );
void OLED12864_Show_String( char* str , uint8_t x , uint8_t y , font_t font );

#endif  //_OLED12864_H_