#include "iot.h"

#include "bsp.h"
#include "hardware.h"
#include "protocol.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static uint8_t init = 0;
static uint8_t decoder_buf[512];
static uint8_t pack_buf[512];
static scp_pack_t recieve_pack;
static scp_pack_t send_pack;
static scp_trans_decoder_t decoder;

#include "elog.h"
#define TAG "iot"

static int iot_send_port( uint8_t* data , uint16_t len );
static int iot_recieve_respond( scp_pack_t* pack );

static TaskHandle_t iot_respond_taskHandle = NULL;
static void iot_respond_task( void* param );

/*****************************************************************/

#include "hmi.h"
#include "motion_control.h"

static float _buf_to_float( uint8_t* buf )
{
    float r;
    *((uint32_t*)&r) = buf[0];
    *((uint32_t*)&r) <<= 8;
    *((uint32_t*)&r) = buf[1];
    *((uint32_t*)&r) <<= 8;
    *((uint32_t*)&r) = buf[2];
    return r;
}

//!!!这里是从isr中调用的!!!
static int iot_recieve_respond( scp_pack_t* pack )
{   
    BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR( iot_respond_taskHandle , 0 , eNoAction , &xHigherPriorityTaskWoken );
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    return 0;
}

/**************************************************************************/

static int iot_send_port( uint8_t* data , uint16_t len )
{
    while( bsp_uart_send( &huart3 , data , len ) != 0 );
    return len;
}

int iot_send( scp_pack_t* pack )
{
    return scp_trans_send( pack , iot_send_port );
}

void iot_start( void )
{
    recieve_pack = scp_trans_pack_create( decoder_buf , 512 );
    decoder = scp_trans_decoder_create( &recieve_pack , iot_recieve_respond  );
    send_pack = scp_trans_pack_create( pack_buf , 512 );
    
    xTaskCreate(
        iot_respond_task,
        "iot",
        128,
        NULL,
        15,
        &iot_respond_taskHandle
    );

    init = 1;
}

static void iot_respond_task( void* param )
{
    uint8_t* payload_addr;
    while(1)
    {
        ulTaskNotifyTake( pdFALSE , -1 );
        payload_addr = recieve_pack.payload.buf;
        switch( recieve_pack.cmd_word )
        {
            default:
                motion_set_target_position( _buf_to_float( payload_addr ) , _buf_to_float( payload_addr + 4 ) , SOFT_REF );
            break;
        }
        beep_notice( BEEP_SYS_DEBUG );
    }
}

void iot_port_input( uint8_t byte )
{
    if( init ) scp_trans_decoder_input_byte( &decoder , byte );
}
