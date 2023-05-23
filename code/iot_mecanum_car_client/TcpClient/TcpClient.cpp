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
    return waitConnect();
}

bool TcpClient::connect( char* ipv4_addr_str , uint16_t port )
{
    if( sock != nullptr )
        return false;
    QHostAddress hostAddr(ipv4_addr_str);
    sock = new QTcpSocket;
    sock->connectToHost( hostAddr , port );
    return waitConnect();
}

bool TcpClient::waitConnect()
{
    sock->waitForConnected( 5000 );
    if( sock->waitForConnected( 5000 ) )
    {
        return true;
    }else
    {
        sock->disconnect();
        delete sock;
        sock = nullptr;
        return false;
    }
}

bool TcpClient::send( uint8_t* data , uint16_t len )
{
    if( sock == nullptr )
    {
        return false;
    }

    sock->write( (char*)data , len );

    return true;
}

bool TcpClient::disconnect()
{
    if( sock == nullptr )
    {
        return true;
    }
    sock->disconnect();
    sock->waitForDisconnected(3000);
    delete sock;
    sock = nullptr;
    return true;
}

TcpClient::~TcpClient()
{
    disconnect();
}
