#ifndef _PBZ_PROTOCOL_H_
#define _PBZ_PROTOCOL_H_

#include <stdint.h>

#ifdef __cplusplis
extern "C"
{
#endif  //__cplusplis

//

typedef enum{
    PING = 0x00,
    PONG = 0x01
}cmd_t;

typedef struct{
    uint8_t* buf;
    uint16_t len;
    uint16_t buf_len;
}pzb_protocol_buf_t;

typedef struct{
    uint8_t head[2];
    uint8_t control_word[2];
    uint8_t len;
    cmd_t   cmd;
    pzb_protocol_buf_t payload;
    uint8_t crc_16[2];
}pbz_protocol_pack_t;

typedef pzb_protocol_buf_t pbz_protocol_pack_stream_t;

typedef struct{
    pzb_protocol_buf_t buf;
    pbz_protocol_pack_stream_t  stream;
    uint16_t buf_index;
}pbz_protocol_machine_t;

typedef struct{
}pbz_protocol_respond_t;

pbz_protocol_machine_t pbz_protocol_machine_init( uint8_t* buf , uint16_t buf_len );

void pbz_protocol_machine_input( pbz_protocl_machine_t* machine , uint8_t* input_data , uint16_t input_len );
void pbz_protocol_machine_set_control_word( pbz_protocl_machine_t* machine , uint16_t control_word );
void pbz_protocol_machine_set_cmd( pbz_protocol_machine_t* machine , cmd_t cmd );
void pbz_protocol_machine_set_payload( pbz_protocol_machine_t* machine , uint8_t* payload , uint16_t len );

#ifdef __cplusplis
}
#endif  //__cplusplis


#endif  //_PBZ_PROTOCOL_H_

