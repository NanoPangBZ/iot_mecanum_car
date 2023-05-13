#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include ".\transmission\scp_trans.h"

scp_trans_decoder_t decoder;

int scp_send_port( uint8_t* buf , uint16_t len )
{
    int index = 0;
    printf( "Send %d byte.\n" , len );
    while( index < len )
    {
        for( int line_c = 0 ; line_c < 16 ; line_c++ )
        {
            if( index < len )
                printf( "0x%02X  " , buf[index] );
            index ++;
        }
        printf("\n");
    }
    printf("\n");
    scp_trans_decoder_input( &decoder , buf , len );
    return 0;
}

int printf_pack( scp_pack_t* pack )
{
    printf("pack printf.\n");
    printf( "control word: 0x%04X\n" , pack->control_word );
    printf( "cmd word: 0x%02x\n" , pack->cmd_word );
    printf( "param:\n" );
    int index = 0;
    while( index < pack->payload_len )
    {
        for( int line_c = 0 ; line_c < 16 ; line_c++ )
        {
            if( index < pack->payload_len )
                printf( "0x%02X  " , pack->payload.buf[index] );
            index ++;
        }
        printf("\n");
    }
    printf("\n");
}

int main(  )
{
    uint8_t* rec_pack_buf = new uint8_t[1024];
    uint8_t* send_pack_buf = new uint8_t[1024];
    auto rec_pack = scp_trans_pack_create( rec_pack_buf , 1024 );
    auto send_pack = scp_trans_pack_create( send_pack_buf , 1024 );
    decoder = scp_trans_decoder_create( &rec_pack , printf_pack );

    send_pack.cmd_word = rand();
    send_pack.control_word = rand();
    send_pack.payload_len = rand() % 1024;
    for( uint16_t temp = 0 ;temp < send_pack.payload_len ; temp++ )
        send_pack.payload.buf[ temp ] = rand();

    scp_trans_send( &send_pack , scp_send_port );

    return 0;
}

