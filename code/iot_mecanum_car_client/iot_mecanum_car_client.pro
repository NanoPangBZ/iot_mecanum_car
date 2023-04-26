QT += quick network

SOURCES += \
        BackendThread.cpp \
        CustomProtocol/iot_mecanum_car_protocol.c \
        NetIpHelper.cpp \
        TcpClient/TcpClient.cpp \
        main.cpp

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    BackendThread.h \
    CustomProtocol/iot_mecanum_car_protocol.h \
    NetIpHelper.h \
    TcpClient/TcpClient.h

DISTFILES += \
    UI/main.qml
