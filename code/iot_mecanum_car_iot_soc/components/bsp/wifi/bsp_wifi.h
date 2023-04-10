#ifndef _BSP_WIFI_H
#define _BSP_WIFI_H_

#ifdef __cplusplus
extern "C"
{
#endif  //_cplusplus

void bsp_wifi_init( void );

int bsp_wifi_connect( char* wifi_name , char* passwd );
int bsp_wifi_ap( char* wifi_name , char* passwd );

void bsp_wifi_stop( void );

#ifdef __cplusplus
}
#endif  //_cplusplus

#endif  //_BSP_WIFI_H_

