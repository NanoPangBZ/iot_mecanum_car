#include "NetIpHelper.h"

NetIpHelper NetIpHelper::netIpHelper;

quint32 NetIpHelper::getIpv4Addr( QString hostName )
{
    QHostInfo info = QHostInfo::fromName(hostName);
    return info.addresses().isEmpty() ? 0 : info.addresses().at(0).toIPv4Address();
}

NetIpHelper::NetIpHelper()
{

}

NetIpHelper::~NetIpHelper()
{

}
