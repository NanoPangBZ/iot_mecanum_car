#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>
#include <QQmlContext>

#include "UiBridge.h"

#include <QDebug>
#define DEBUG_INFO(...) qDebug(__VA_ARGS__)

#include <windows.h>
#include <pthread.h>
#include <math.h>


void* _test_thread(void* param)
{
    UiBridge* obj = (UiBridge*)param;
    int pos[2];
    float angle = 0;

    while(1)
    {
        pos[0] = sin(angle) * 300 + 300;
        pos[1] = cos(angle) * 300 + 300;
        emit obj->Signal_CurrentPosChange( pos[0] , pos[1] );
        angle+=0.01;
        Sleep(10);
    }
}

pthread_t _thread;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    UiBridge uiBridge;

    //将uiBridge加入到ui引擎中
    engine.rootContext()->setContextProperty("uiBridge", &uiBridge);

    //载入UI文件 & 启动UI
    const QUrl url("./ui/root.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    //获取qml对象  通过c++能访问UI
    QObject* qmlRootObj = engine.rootObjects().at(0);
    qmlRootObj->setProperty( "windowTitle" , "Test" );

    //ui与c++交互使用信号与槽,在qml文件中连接

    pthread_create( &_thread , NULL ,  _test_thread , &uiBridge);

    return app.exec();
}
