import QtQuick
import QtQuick.Controls
import QtQml

Window {
    id:mainWindow
    width: 960
    height: 720
    maximumWidth: 960
    minimumWidth: 960
    maximumHeight: 720
    minimumHeight: 720
    visible: true
    title: qsTr("物联网全向移动平台终端")

    //自定义属性
    property int connectState: -1 //-1:离线 0:搜索设备中 1:设备1
    property string deviceIVP4Addr: "未知"
    property real targetPositionX: 0.0
    property real targetPositionY: 0.0
    property real targetPositionYaw: 0.0
    property real carPositionX: 0.0
    property real carPositionY: 0.0
    property real carPositionYaw: 0.0
    property real gyroscopePosition: 0.0

    //通知后台构件已经完成构建
    Component.onCompleted:{
        backend.qmlCompleted()
    }

    //模型面板
    Item{
        id:modelPanel
        width: parent.width
        height: parent.height * 0.7
        anchors.top: parent.top
        anchors.left: parent.left
        Text{
            text:"模型面板"
        }
    }

    //控制面板
    Item{
        id:ctrPanel
        width: parent.width * 0.6
        height: parent.height * 0.3 - 20
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 20
        Text{
            text:"控制面板"
        }
    }

    //日志面板
    Item{
        id:logPanel
        width: parent.width * 0.4
        height: parent.height * 0.3 - 20
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 20
        Text{
            text:"日志面板"
        }
    }

    //软件信息面板
    Item{
        id:appInfoPanel
        width: parent.width
        height: 20
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        Text{
            text:"demo"
        }
    }
}
