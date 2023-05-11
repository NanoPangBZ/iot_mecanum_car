#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include ".\transmission\scp_trans.h"

int scp_send_port( uint8_t* buf , uint16_t len )
{
    int index = 0;
    printf( "Send %d byte.\n" , len );
    while( index < len )
    {
        for( int line_c = 0 ; line_c < 4 ; line_c++ )
        {
            if( index < len )
                printf( "0x%02X  " , buf[index] );
            index ++;
        }
        printf("\n");
    }
    return 0;
}

int main(  )
{
    uint8_t* pack_buf = new uint8_t[1024];
    auto pack = scp_pack_create( pack_buf , 1024 );
    auto decoder = scp_decoder_create( &pack , NULL );
    uint8_t test[64] = {
        0xEA , 0xAF ,   //头
        0x00 , 0x00,    //控制
        0x00 , 0x00 , 0x00, //命令 长度
        0xaa , 0xcc //校验
    };
    scp_decoder_input( &decoder , test , 8 );
    system("pause");
    return 0;
}

