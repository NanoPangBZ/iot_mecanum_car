#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include "esp_netif.h"
#include "driver/gpio.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#ifdef __cplusplus
extern "C"
{
#endif  //__cplusplus

int tcp_client_connect( uint32_t ip , uint16_t port );
int tcp_client_send( uint8_t* data , uint8_t len );

#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //_TCP_CLIENT_H_

