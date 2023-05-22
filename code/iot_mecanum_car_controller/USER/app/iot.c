#include "iot.h"

#include "bsp.h"
#include "hardware.h"
#include "protocol.h"

#include "FreeRTOS.h"
#include "task.h"

static uint8_t init = 0;
static uint8_t pack_buf[1024];
static scp_pack_t pack;
static scp_trans_decoder_t decoder;

#include "elog.h"
#define TAG "iot"

static int iot_send_port( uint8_t* data , uint16_t len );
static int iot_recieve_respond( scp_pack_t* pack );

static int iot_recieve_respond( scp_pack_t* pack )
{
    switch ( pack->cmd_word )
    {
    case 0x01:
        
        break;
    
    default:
        break;
    }
    return 0;
}

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
    pack = scp_trans_pack_create( pack_buf , 1024 );
    decoder = scp_trans_decoder_create( &pack , iot_recieve_respond  );
    init = 1;
}

void iot_port_input( uint8_t byte )
{
    if( init ) scp_trans_decoder_input_byte( &decoder , byte );
}
