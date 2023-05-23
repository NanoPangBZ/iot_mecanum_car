#include "BackendThread.h"
#include <windows.h>
#include <QVariant>
#include <QQuickItem>
#include <math.h>

#include "TcpClient/TcpClient.h"

//唯一实例
BackendThread BackendThread::backendThread;

//UI对象
extern QObject* qmlObj;

//静态变量
static TcpClient* client = NULL;

//后台线程
void* BackendThread::_main(void*param)
{
    sem_wait( &backendThread._rootUiCompleted );
    Sleep(100);

    backendThread.setCarRectangle( 100 , 140 );

    float t = 0;
    while(1)
    {
        Sleep(200);
    }
    return param;
}

BackendThread::BackendThread() : QObject{NULL}
{
    sem_init( &_rootUiCompleted , 0 , 0 );
    pthread_create( &_thread , NULL , _main , this );
}

BackendThread::~BackendThread()
{
}

void BackendThread::qmlCompleted()
{
    sem_post( &_rootUiCompleted );
}

void BackendThread::targetPositionRefresh( float x , float y )
{
}

void BackendThread::connectCar( QString addr )
{
    if( client != NULL )
    {
        delete client;
        client = NULL;
    }

    client = new TcpClient;
    if( client->connect( addr.toLocal8Bit().data() , 144 ) )
    {
        qDebug("连接成功 %s" , addr.toLocal8Bit().data());
    }else
    {
        qDebug( "连接失败" );
        delete client;
        client = NULL;
    }
    Sleep( 2000 );
}

void BackendThread::carYawModeRefresh( int mode )
{

}

void BackendThread::updataCarPosition( float x , float y , float yaw )
{
    qmlObj->setProperty( "carPositionX" , QVariant(x) );
    qmlObj->setProperty( "carPositionY" , QVariant(y) );
    qmlObj->setProperty( "carPositionYaw" , QVariant(yaw) );
}

void BackendThread::setCarRectangle( float width , float height )
{
    qmlObj->setProperty( "carWidth" , QVariant(width) );
    qmlObj->setProperty( "carHeight" , QVariant(height) );
}

