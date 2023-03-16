#include "wit_protocol.h"
#include <string.h>

#define WIT_DATA_BUFF_SIZE  64

static uint8_t s_ucWitDataBuff[ WIT_DATA_BUFF_SIZE ];
static uint8_t s_uiWitDataCnt = 0;

__attribute__((weak)) void wit_protocol_respond( uint8_t type,  uint16_t* t_4_uint16 )
{

}

static uint8_t __CaliSum(uint8_t *data, uint32_t len)
{
    uint32_t i;
    uint8_t ucCheck = 0;
    for(i=0; i<len; i++) ucCheck += *(data + i);
    return ucCheck;
}

void wit_protocol_machine_input( uint8_t data )
{
    uint16_t usCRC16, usTemp, i, usData[4];
    uint8_t ucSum;

    s_ucWitDataBuff[s_uiWitDataCnt++] = data;
    if(s_ucWitDataBuff[0] != 0x55)
    {
        s_uiWitDataCnt--;
        memcpy(s_ucWitDataBuff, &s_ucWitDataBuff[1], s_uiWitDataCnt);
        return ;
    }

    if(s_uiWitDataCnt >= 11)
    {
        ucSum = __CaliSum(s_ucWitDataBuff, 10);
        if(ucSum != s_ucWitDataBuff[10])
        {
            s_uiWitDataCnt--;
            memcpy(s_ucWitDataBuff, &s_ucWitDataBuff[1], s_uiWitDataCnt);
            return ;
        }
        usData[0] = ((uint16_t)s_ucWitDataBuff[3] << 8) | (uint16_t)s_ucWitDataBuff[2];
        usData[1] = ((uint16_t)s_ucWitDataBuff[5] << 8) | (uint16_t)s_ucWitDataBuff[4];
        usData[2] = ((uint16_t)s_ucWitDataBuff[7] << 8) | (uint16_t)s_ucWitDataBuff[6];
        usData[3] = ((uint16_t)s_ucWitDataBuff[9] << 8) | (uint16_t)s_ucWitDataBuff[8];
        wit_protocol_respond( s_ucWitDataBuff[1] , usData );
        //CopeWitData(s_ucWitDataBuff[1], usData, 4);
        s_uiWitDataCnt = 0;
    }

    if(s_uiWitDataCnt == WIT_DATA_BUFF_SIZE)s_uiWitDataCnt = 0;
}
