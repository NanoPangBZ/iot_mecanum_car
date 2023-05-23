#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <stdint.h>
#include <QTcpSocket>
#include <QHostAddress>

class TcpClient
{
public:
    TcpClient();
    ~TcpClient();
    bool connect( uint32_t ipv4_addr , uint16_t port );
    bool connect( char* ipv4_addr_str , uint16_t port );
    bool disconnect();
    bool isConnect();
    bool send( uint8_t* data , uint16_t len );
    bool readAll( uint8_t* buf , uint16_t buf_len );
private:
    QTcpSocket* sock = nullptr;
    bool waitConnect();
};

#endif // TCPCLIENT_H
