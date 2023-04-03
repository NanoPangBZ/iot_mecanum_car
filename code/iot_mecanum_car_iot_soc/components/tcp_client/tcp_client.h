#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

class tcp_client
{
public:
    tcp_client(  uint32_t ip , uint16_t port  );
    tcp_client( char* ip , uint16_t port );
    ~tcp_client();

    int connect();

    int send( uint8_t* data , uint16_t len );
    int revieve( uint8_t* data_buf , uint16_t buf_len );
private:
    int _sockfd;
    uint32_t _ip;
    uint16_t _port;
};

#endif  //_TCP_CLIENT_H_

