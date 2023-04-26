#ifndef NETIPHELPER_H
#define NETIPHELPER_H

#include <QHostInfo>

class NetIpHelper
{
public:
    static NetIpHelper netIpHelper;
    quint32 getIpv4Addr( QString hostName );
private:
    NetIpHelper();
    ~NetIpHelper();
};

#endif // NETIPHELPER_H
