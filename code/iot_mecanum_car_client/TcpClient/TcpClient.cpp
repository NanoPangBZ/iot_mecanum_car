#include "TcpClient.h"

TcpClient::TcpClient()
{

}

bool TcpClient::connect( uint32_t ipv4_addr , uint16_t port )
{
    if( sock != nullptr )
        return false;
    QHostAddress hostAddr(ipv4_addr);
    sock = new QTcpSocket;
    sock->connectToHost( hostAddr , port );
    return true;
}

bool TcpClient::connect( char* ipv4_addr_str , uint16_t port )
{
    if( sock != nullptr )
        return false;
    QHostAddress hostAddr(ipv4_addr_str);
    sock = new QTcpSocket;
    sock->connectToHost( hostAddr , port );
    return true;
}
