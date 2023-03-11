#include "oled12864.h"
#include "font_lib.h"
#include <stdio.h>

//引脚编号
#define OLED_RES    0
#define OLED_DC     1
#define OLED_CS     2

/************************************************
 * OLED12864缓存
 * [paeg][x]
 * 一个元素保函8像素点信息
 * 在屏幕上,Bit0~Bit7自上向下排列 高位在下
************************************************/
static uint8_t OLED12864_Sbuffer[8][128];
//OLED_初始化指令
//初始化为水平寻址模式
static unsigned char OLED12864_InitCmd[28] = {
    0xae,0x00,0x10,0x40,0x81,0xcf,
    0xa1,0xc8,0xa6,0xa8,0x3f,0xd3,
    0x00,0xd5,0x80,0xd9,0xf1,0xda,
    0x12,0xdb,0x40,0x20,0x00,0x8d,
    0x14,0xa4,0xa6,0xaf
};

/*************************************************************************************************/

//移植对接宏
#include "stm32f1xx_hal.h"
#define OLED12864_delay_ms(ms)              HAL_Delay(ms)
#define OLED12864_SPI_Send_Byte(dat)        spi_send_byte(dat)
#define OLED12864_Set_Bit(pin_Num)          gpio_write( pin_Num , 1)
#define OLED12864_Reset_Bit(pin_Num)        gpio_write( pin_Num , 0)
#define OLED12864_SPI_Init()                spi_init()
#define OLED12864_GPIO_Init()               gpio_init()
#define OLED12864_Auto_refresh()            auto_refresh()

//对接实现
static void spi_init(void);
static void spi_send_byte(uint8_t data);
static void gpio_write(uint8_t pin_num , uint8_t bit);
static void gpio_init(void);
static void auto_refresh(void);

#include "gpio.h"
#include "main.h"
#include "spi.h"
#include "dma.h"

static void auto_refresh()
{
    //dma初始化移交hal库
    HAL_SPI_Transmit_DMA( &OLED_SPI , OLED12864_Sbuffer[0] , 1024 );
}

static void spi_send_byte(uint8_t data)
{
    HAL_SPI_Transmit( &OLED_SPI , &data , 1 , 1000 );
}

static void gpio_write(uint8_t pin_num , uint8_t bit)
{
    GPIO_TypeDef* const GPIOs[3] = { OLED_RES_GPIO_Port , OLED_DC_GPIO_Port , OLED_CS_GPIO_Port };
    const uint16_t pins[3] = { OLED_RES_Pin , OLED_DC_Pin , OLED_CS_Pin };
    HAL_GPIO_WritePin( GPIOs[pin_num] , pins[pin_num] , bit!=0?GPIO_PIN_SET:GPIO_PIN_RESET );
}

static void gpio_init()
{
    //移交hal库 防止重复初始化
}

static void spi_init(void)
{
    MX_SPI2_Init();
}

/**********************************API****************************************************/

void OLED12864_Init(void)
{
    OLED12864_GPIO_Init();
    OLED12864_SPI_Init();
    OLED12864_Hard_Reset();
    OLED12864_Set_Position(0,0);
    OLED12864_Set_Bit(OLED_DC);
    OLED12864_delay_ms(100);
    OLED12864_Auto_refresh();
}

void OLED12864_Hard_Reset(void)
{
    OLED12864_Reset_Bit(OLED_RES);
    OLED12864_delay_ms(300);
    OLED12864_Set_Bit(OLED_RES);
    
    OLED12864_Reset_Bit(OLED_CS);

    OLED12864_Send_NumByte(OLED12864_InitCmd,28,OLED_CMD);
    OLED12864_Clear();
}

void OLED12864_Clear_Sbuffer(void)
{
    uint8_t page,x;
    for(page=0;page<8;page++)
    {
        for(x=0;x<128;x++)
            OLED12864_Sbuffer[page][x] = 0x00;
    }
}

void OLED12864_Clear(void)
{
    OLED12864_Clear_Sbuffer();
    OLED12864_Refresh();
}

void OLED12864_Refresh(void)
{
    #if 0
    OLED12864_Set_Position(0,0);
    OLED12864_Send_NumByte(OLED12864_Sbuffer[0],1024,OLED_DATA);
    #endif
}

void OLED12864_Set_Position(uint8_t page,uint8_t x)
{
    uint8_t dat[3];
    dat[0] = 0xb0 + page;           //页地址
    dat[1] = ((0xf0&x)>>4)|0x10;    //x坐标高四位设定
    dat[2] = 0x0f&x;                //x坐标低四位设定
    OLED12864_Send_NumByte(dat,3,OLED_CMD);
}

void OLED12864_Send_NumByte(const uint8_t*dat,uint16_t len,uint8_t cmd)
{
    if(cmd)
        OLED12864_Set_Bit(OLED_DC);
    else
        OLED12864_Reset_Bit(OLED_DC);

    for(uint16_t temp=0;temp<len;temp++)
    {
        OLED12864_SPI_Send_Byte(*dat);
        dat++;
    }
}

void OLED12864_Send_Byte(uint8_t dat,uint8_t cmd)
{
    if(cmd)
        OLED12864_Set_Bit(OLED_DC);
    else
        OLED12864_Reset_Bit(OLED_DC);
    
    OLED12864_SPI_Send_Byte(dat);
}

void OLED12864_Clear_PageBlock(uint8_t page,uint8_t x,uint8_t len)
{
    uint8_t sx = x+len;
    if(sx > x_MAX-1 || page > page_MAX-1)
        return;
    for(uint8_t temp=0;temp<len;temp++)
        OLED12864_Sbuffer[page][temp+x] = 0x00;
}

void OLED12864_Clear_Page(uint8_t page)
{
    OLED12864_Clear_PageBlock(page,0,127);
}

void OLED12864_Show_Char(uint8_t x,uint8_t y,uint8_t chr,uint8_t size)
{
    uint8_t* offsetAddr = OLED12864_Sbuffer[0] + ( (y/8) * 128 ) + x;
    uint8_t pageOffset = y%8;
    uint8_t pageUpOffset = 8 - pageOffset;
    switch (size)
    {
    case 1:
        for(uint8_t sx = 0; sx<6 ;sx++){
            *(offsetAddr) &=  ~(0xff << pageOffset);
            *(offsetAddr) |=  assic_0806[chr-0x20][sx] << pageOffset;
            offsetAddr += 128;
            *(offsetAddr) &=  ~(0xff >> pageUpOffset);
            *(offsetAddr) |=  assic_0806[chr-0x20][sx] >> pageUpOffset;
            offsetAddr -= 127;
        }
        break;
    case 2:
        for(uint8_t sx = 0; sx<8 ;sx++){
            *(offsetAddr) &=  ~(0xff << pageOffset);
            *(offsetAddr) |=  (assic_1608[chr-0x20][sx] << pageOffset);
            offsetAddr += 128;
            *(offsetAddr) &=  ~0xff ;
            *(offsetAddr) |=  assic_1608[chr-0x20][sx] >> pageUpOffset;
            *(offsetAddr) |=  assic_1608[chr-0x20][sx+8] << pageOffset;
            offsetAddr += 128;
            *(offsetAddr) &=  ~(0xff >> pageUpOffset);
            *(offsetAddr) |=  (assic_1608[chr-0x20][sx+8] >> pageUpOffset);
            offsetAddr -= 255;
        }
        break;
    default:
        break;
    }
}

uint8_t  OLED12864_Show_Num(uint8_t page,uint8_t x,int num,uint8_t size)
{
    char sbuf[8];
    sprintf((char*)sbuf,"%d",num);
    OLED12864_Show_String( x , page*8 , sbuf , 1 );
    return 0;
}

uint8_t OLED12864_Show_fNum(uint8_t page,uint8_t x,double num,uint8_t size,uint8_t d_len)
{
    int L_Num;              //整数部分
    double R_Num;           //小数部分
    int R_Num2;             //根据小数部分化整
    uint8_t L_len = 0;      //整数部分长度
    if(d_len==0)
        d_len = 1;
    L_Num = (int)num;
    R_Num = num - L_Num;
    if(R_Num<0)
        R_Num = -R_Num;
    L_len = OLED12864_Show_Num(page,x,L_Num,size);
    switch(size)
    {
        case 1: x += 6*(L_len+1); OLED12864_Show_Char(page,x,'.',size); x+=6; break;
        case 2: x += 8*(L_len+1); OLED12864_Show_Char(page,x,'.',size); x+=8; break;
        default:break;
    }
    while(d_len!=0)
    {
        R_Num*=10;
        d_len--;
    }
    R_Num2 = (int)R_Num;
    OLED12864_Show_Num(page,x,R_Num2,size);
    return L_len+1+d_len;
}

void OLED12864_Show_String(uint8_t x,uint8_t y,char*str,uint8_t size)
{
    uint8_t sx = 0;
    while(*str!='\0')
    {
        OLED12864_Show_Char(x+sx,y,*str,size);
        switch(size)
        {
            case 1:
                sx+=6;
                break;
            case 2:
                sx+=8;
                break;
            default:
                break;
        }
        str++;
    }
}

//像素点相关操作
#if USE_POINT_CRT == 1

void OLED12864_Draw_Point(uint8_t x,uint8_t y,uint8_t bit)
{
    if(y > y_MAX-1 || x > x_MAX-1)
        return;
    uint8_t page = y/8;
    uint8_t col = y%8;
    if(bit)
        OLED12864_Sbuffer[page][x] |= (0x01<<col);
    else
        OLED12864_Sbuffer[page][x] &= ~(0x01<<col);
}

void OLED12864_Draw_Line(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
    float k = (float)(y1 - y2) / (float)(x1 -x2);   //斜率
    float k_1 = 1 / k;

    float sx = x1;
    float sy = y1;
    while( sx != x2){
        OLED12864_Draw_Point((int)sx,(int)sy,1);
        if( sx < x2 ){
            sx ++;
            sy += k;
        }else{
            sx --;
            sy -= k;
        }
    }

    sx = x1;
    sy = y1;
    while( sy != y2 ){
        OLED12864_Draw_Point((int)sx,(int)sy,1);
        if( sy < y2 ){
            sy ++;
            sx += k_1;
        }else{
            sy --;
            sx -= k_1;
        }
    }
}

void OLED12864_Draw_Rect(uint8_t x,uint8_t y,uint8_t len,uint8_t hight)
{
    for(uint8_t temp=0;temp<len;temp++)
    {
        OLED12864_Draw_Point(x+temp,y,1);
        OLED12864_Draw_Point(x+temp,y+hight,1);
    }
    for(uint8_t temp=0;temp<hight;temp++)
    {
        OLED12864_Draw_Point(x,y+temp,1);
        OLED12864_Draw_Point(x+len,y+temp,1);
    }
}

void OLED12864_Draw_Img(uint8_t x,uint8_t y,uint8_t len,uint8_t hight,uint8_t*img)
{
    uint8_t sx,sy;
    uint16_t dat_addr_pos;
    uint8_t page_pos;
    uint8_t bit_pos;
    for(sy=0;sy<hight;sy++)
    {
        page_pos = sy/8;
        bit_pos = sy%8;
        for(sx=0;sx<len;sx++)
        {
            dat_addr_pos = page_pos*len + sx;
            OLED12864_Draw_Point(sx+x,sy+y, *(img+dat_addr_pos) & ((0x80)>>bit_pos) );
        }
    }
}

void OLED12864_Draw_aImg(uint8_t x,uint8_t y,uint8_t*img)
{
    uint8_t len,hight;
    uint8_t sx,sy;
    uint16_t dat_addr_pos;
    uint8_t page_pos;
    uint8_t bit_pos;
    len = *(img+3) + *(img+2)*256;
    hight = *(img+5) + *(img+4)*256;
    for(sy=0;sy<hight;sy++)
    {
        page_pos = sy/8;
        bit_pos = sy%8;
        for(sx=0;sx<len;sx++)
        {
            dat_addr_pos = page_pos*len + sx + 6;
            OLED12864_Draw_Point(sx+x,sy+y, *(img+dat_addr_pos) & ((0x80)>>bit_pos) );
        }
    }
}

#endif  //USE_POINT_CRT
