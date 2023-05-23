#ifndef BACKENDTHREAD_H
#define BACKENDTHREAD_H

#include <QObject>
#include <pthread.h>
#include <semaphore.h>

class BackendThread : public QObject
{
    Q_OBJECT
public:
    static BackendThread backendThread;

//对qml开放的接口
public:
    Q_INVOKABLE void qmlCompleted();
    Q_INVOKABLE void targetPositionRefresh( float x , float y );
    Q_INVOKABLE void connectCar( QString addr );
    Q_INVOKABLE void carYawModeRefresh( int mode );

//线程
private:
    static void* _main(void*);
    pthread_t   _thread;
    sem_t       _rootUiCompleted;

//单例模式相关
private:
    BackendThread();
    ~BackendThread();

    void updataCarPosition( float x , float y , float yaw );
    void setCarRectangle( float width , float height );
};

#endif // BACKENDTHREAD_H
