#include "BackendThread.h"
#include <windows.h>
#include <QVariant>
#include <math.h>

//唯一实例
BackendThread BackendThread::backendThread;

//UI对象
extern QObject* qmlObj;

//后台线程
void* BackendThread::_main(void*param)
{
    sem_wait( &backendThread._rootUiCompleted );
    Sleep(100);

    backendThread.setCarRectangle( 100 , 140 );

    float t = 0;
    while(1)
    {
        backendThread.updataCarPosition( sin( t ) * 300 , cos( t ) * 300 , sin(t*2) * 180 );
        t += 0.01;
        Sleep(10);
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

