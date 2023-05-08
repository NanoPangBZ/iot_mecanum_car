#include "oled12864.h"
#include "font_lib.h"
#include <stdio.h>
#include <string.h>

//引脚编号
#define OLED_RES    0
#define OLED_DC     1
#define OLED_CS     2

static void OLED12864_Send_Byte(uint8_t dat,uint8_t cmd);
static void OLED12864_Send_NumByte(const uint8_t*dat,uint16_t len,uint8_t cmd);
static void OLED12864_Set_Position(uint8_t page,uint8_t x);
static void OLED12864_Draw_SegOfs( uint8_t x , uint8_t y , uint8_t data );

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

/*********************************port macro*******************************************************/
//移植对接宏
#include "stm32f1xx_hal.h"
#define OLED12864_delay_ms(ms)              HAL_Delay(ms)
#define OLED12864_SPI_Send_Byte(dat)        spi_send_byte(dat)
#define OLED12864_Set_Bit(pin_Num)          gpio_write( pin_Num , 1)
#define OLED12864_Reset_Bit(pin_Num)        gpio_write( pin_Num , 0)
#define OLED12864_SPI_Init()                spi_init()
#define OLED12864_GPIO_Init()               gpio_init()
#if OLED12864_DMA_ENABLE
#define OLED12864_DMA_Init()                dma_init()
#define OLED12864_DMA_Buf_Sync_Start( buf , len )   dma_start( buf , len )
#endif  //OLED12864_DMA_ENABLE

/*********************************port realize************************************************/
#include "gpio.h"
#include "main.h"
#include "spi.h"
#include "dma.h"

//对接实现
static void spi_init(void);
static void spi_send_byte(uint8_t data);
static void gpio_write(uint8_t pin_num , uint8_t bit);
static void gpio_init(void);
static void dma_init(void);
static void dma_start( uint8_t* buf , uint16_t len );

static void dma_start( uint8_t* buf , uint16_t len )
{
    HAL_SPI_Transmit_DMA( &OLED_SPI , buf , len );
}

static void dma_init(void)
{
    //DMA初始化交给Hal初始化
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
#if OLED12864_DMA_ENABLE
    OLED12864_DMA_Init();
#endif
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

#if OLED12864_DMA_ENABLE
void OLED12864_DMA_Sync_Start(void)
{
    OLED12864_DMA_Buf_Sync_Start( OLED12864_Sbuffer[0] , x_MAX * y_MAX / 8 );
}
#endif  //OLED12864_DMA_ENABLE

void OLED12864_Flush( void )
{
    //..
}

void OLED12864_Test( void )
{
    uint8_t temp[8];
    memset( temp , 0xff , 8 );
    OLED12864_Draw_Map( 0 , 0 , 8 , 0 , temp );
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
    OLED12864_Flush();
}

void OLED12864_Draw_Point( uint8_t x, uint8_t y , uint8_t bit )
{
    uint8_t page = y / 8;
    uint8_t bit_mask = (0x01)<<( y % 8 );
    if( bit )
        OLED12864_Sbuffer[ page ][ x ] |= bit_mask;
    else
        OLED12864_Sbuffer[ page ][ x ] &= ~bit_mask;
}

void OLED12864_Draw_Map( uint8_t sx , uint8_t sy , uint8_t ex , uint8_t ey , uint8_t* map )
{
    uint8_t s_page = sy / 8 , e_page = ey / 8;
    uint8_t ofs = sy % 8;
    uint8_t ofs_s = 8 - ofs;
    for( uint8_t c_page = s_page ; c_page <= e_page ;  c_page ++ )
    {
        uint8_t* buf_addr = OLED12864_Sbuffer[ c_page ];
        for( uint8_t cx = sx ; cx <= ex ; sx ++ )
        {
            buf_addr[ cx ] |= ~( 0xff << ofs );
            *buf_addr |= ( (*map) << ofs);
            buf_addr += 128;
            *buf_addr &= ~( 0xff >> ofs );
            *buf_addr |= ( (*map) >> ofs );
            map ++;
        }
    }
}

/**********************************内部函数************************************************/

static void OLED12864_Set_Position(uint8_t page,uint8_t x)
{
    uint8_t dat[3];
    dat[0] = 0xb0 + page;           //页地址
    dat[1] = ((0xf0&x)>>4)|0x10;    //x坐标高四位设定
    dat[2] = 0x0f&x;                //x坐标低四位设定
    OLED12864_Send_NumByte(dat,3,OLED_CMD);
}

static void OLED12864_Send_NumByte(const uint8_t*dat,uint16_t len,uint8_t cmd)
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

static void OLED12864_Send_Byte(uint8_t dat,uint8_t cmd)
{
    if(cmd)
        OLED12864_Set_Bit(OLED_DC);
    else
        OLED12864_Reset_Bit(OLED_DC);
    
    OLED12864_SPI_Send_Byte(dat);
}

static void OLED12864_Draw_SegOfs( uint8_t x , uint8_t y , uint8_t data )
{
    uint8_t page = y / 8;
    uint8_t ofs = y % 8;
    uint8_t* buf_addr = &OLED12864_Sbuffer[ page ][ x ];

    if( ofs )
    {
        *buf_addr &= ~( 0xff << ofs );
        *buf_addr |= (data << ofs);
        buf_addr += 128;
        ofs = 8 - ofs;
        *buf_addr &= ~( 0xff >> ofs );
        *buf_addr |= ( data >> ofs );
    }else
    {
        *buf_addr = data;
    }
}
