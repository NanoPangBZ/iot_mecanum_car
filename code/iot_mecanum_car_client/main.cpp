#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "BackendThread.h"
#include "TcpClient\TcpClient.h"

QObject* qmlObj = 0;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    QQmlApplicationEngine engine;

    //将封装了业务逻辑的backend实例添加到UI引擎中，方便qml直接调用后台
    engine.rootContext()->setContextProperty( "backend" , &BackendThread::backendThread );

    const QUrl url("./UI/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    //获取UI对象，方便c/c++控制UI
    qmlObj = engine.rootObjects().at(0);

    return app.exec();
}
