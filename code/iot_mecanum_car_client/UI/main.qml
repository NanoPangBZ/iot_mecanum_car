import QtQuick
import QtQuick.Controls
import QtQuick.Window 2.2
import QtQml

Window {
    id:mainWindow
    width: Screen.width * 0.6
    height: width * 0.6
    maximumWidth: Screen.width * 0.6
    minimumWidth: Screen.width * 0.6
    maximumHeight: width * 0.6
    minimumHeight: width * 0.6
    visible: true
    title: qsTr("物联网全向移动平台终端")

    //自定义属性
    property int connectState: -1 //-1:离线 0:搜索设备中 1:设备1
    property string deviceIVP4Addr: "未知"
    property string appVersion: "demo"
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

        //参数列表
        Item{
            id:paramTabel
            width: parent.width * 0.3
            height: parent.height
            anchors.top: parent.top7
            anchors.left: parent.left
            Text{
                id: connectStateLabel
                anchors.top: parent.top
                anchors.left: parent.left
                text:"状态:离线"
                font.pixelSize: parent.height * 0.05
            }
            Text{
                id: targetPosXLeabel
                anchors.top: parent.top
                anchors.left: parent.left
                text:"目标x坐标:" + targetPositionX
                font.pixelSize: parent.height * 0.05
            }
            Text{
                id: targetPosYLeabel
                anchors.top: targetPosXLeabel.bottom
                text:"目标y坐标:" + targetPositionX
                font.pixelSize: parent.height * 0.05
            }
            Text{
                id: curPosXLeabel
                anchors.top: targetPosYLeabel.bottom
                text:"当前x坐标:" + targetPositionX
                font.pixelSize: parent.height * 0.05
            }
            Text{
                id: curPosYLeabel
                anchors.top: curPosXLeabel.bottom
                text:"当前y坐标:" + targetPositionX
                font.pixelSize: parent.height * 0.05
            }
            Text{
                id: targetYawLeabel
                anchors.top: curPosYLeabel.bottom
                text:"目标航向角:" + targetPositionX
                font.pixelSize: parent.height * 0.05
            }
            Text{
                id: curYawLeabel
                anchors.top: targetYawLeabel.bottom
                text:"当前航向角:" + targetPositionX
                font.pixelSize: parent.height * 0.05
            }
        }
    }

    //控制面板
    Item{
        id:ctrPanel
        width: parent.width * 0.6
        height: parent.height * 0.25
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
        height: parent.height * 0.25
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
        height: parent.height * 0.05
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        Text{
            text:"版本号:" + appVersion
            font.pixelSize: parent.height * 0.6
        }
    }
}
