#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#ifdef __cplusplus
extern "C"
{
#endif  //__cplusplus

int tcp_server_init( void );

#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  //_TCP_SERVER_H_


