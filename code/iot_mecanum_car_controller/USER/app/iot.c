#include "iot.h"

#include "hardware.h"
#include "protocol.h"

static uint8_t init = 0;
static uint8_t pack_buf[1024];
static scp_pack_t pack;
static scp_trans_decoder_t decoder;

static int decoder_cb( scp_pack_t* pack )
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

void iot_start( void )
{
    pack = scp_trans_pack_create( pack_buf , 1024 );
    decoder = scp_trans_decoder_create( &pack , decoder_cb  );
    init = 1;
}

void iot_port_input( uint8_t byte )
{
    if( init ) scp_trans_decoder_input_byte( &decoder , byte );
}
