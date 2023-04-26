#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <stdint.h>
#include <QTcpSocket>
#include <QHostAddress>

class TcpClient
{
public:
    TcpClient();
    bool connect();
    bool disconnect();
    bool isConnect();
    bool send( uint8_t* data , uint16_t len );
    bool readAll( uint8_t* buf , uint16_t buf_len );

};

#endif // TCPCLIENT_H
