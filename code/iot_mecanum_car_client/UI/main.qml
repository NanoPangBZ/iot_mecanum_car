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
    color: "#292421"

    //自定义属性
    property int connectState: -1 //-1:离线 0:搜索设备中 1:设备1
    property string deviceIVP4Addr: "未知"
    property string appVersion: "demo"
    property real targetPositionX: 200
    property real targetPositionY: 200
    property real targetPositionYaw: 200
    property real carPositionX: 200
    property real carPositionY: 200
    property real carPositionYaw: 200
    property real gyroscopePosition: 200

    //通知后台构件已经完成构建
    Component.onCompleted:{
        backend.qmlCompleted()
    }

    onConnectStateChanged: {
        if( connectState === -1 )
        {

        }
    }

    //模型面板
    Item{
        id:modelPanel
        width: parent.width
        height: parent.height * 0.65
        anchors.top: parent.top
        anchors.left: parent.left

        //模型
        Item{
            id: modelView
            width: parent.width * 0.7
            height: parent.height
            anchors.top: parent.top
            anchors.right: parent.right

            //map
            Rectangle{
                anchors.fill: parent
                anchors.topMargin: parent.height * 0.02
                anchors.bottomMargin: parent.height * 0.02
                anchors.leftMargin: parent.width * 0.02
                anchors.rightMargin: parent.width * 0.02
                color:"#9c661f"
                clip:true

                //wh比
                //width恒定单位500
                //小车width恒定30
                //小车height恒定40
                property real whs: width / height

                //实际小车位置视图
                Rectangle{
                    width: parent.width * 3 / 50
                    height: width * 4 / 3
                    radius: width * 0.2
                    color: Qt.rgba( 1 , 1 , 1 , 0.5 )

                    x: parent.width/2 + carPositionX - width/2
                    y: parent.height/2 - carPositionY - height/2
                }

                //实际小车位置视图
                Rectangle{
                    color: Qt.rgba( 0 , 0 , 0 , 0.0 )
                    width: parent.width * 3 / 50 * 1.2
                    height: width * 4 / 3
                    radius: width * 0.2
                    border.width: width * 0.02
                    border.color: "#802a2a"

                    x: parent.width/2 + carPositionX - width/2
                    y: parent.height/2 - carPositionY - height/2
                }
            }
        }

        //参数列表
        Item{
            id:paramTabel
            width: parent.width * 0.3
            height: parent.height
            anchors.top: parent.top
            anchors.left: parent.left
            Text{
                id: connectStateLabel
                anchors.top: parent.top
                anchors.left: parent.left
                text:"状态:离线"
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: targetPosXLeabel
                anchors.top: connectStateLabel.bottom
                text:"目标x坐标:\t" + targetPositionX
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: targetPosYLeabel
                anchors.top: targetPosXLeabel.bottom
                text:"目标y坐标:\t" + targetPositionX
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: curPosXLeabel
                anchors.top: targetPosYLeabel.bottom
                text:"当前x坐标:\t" + targetPositionX
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: curPosYLeabel
                anchors.top: curPosXLeabel.bottom
                text:"当前y坐标:\t" + targetPositionX
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: targetYawLeabel
                anchors.top: curPosYLeabel.bottom
                text:"目标航向角:\t" + targetPositionX
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: curYawLeabel
                anchors.top: targetYawLeabel.bottom
                text:"当前航向角:\t" + targetPositionX
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
        }
    }

    //控制面板
    Item{
        id:ctrPanel
        width: parent.width * 0.6
        height: parent.height * 0.30
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
        height: parent.height * 0.30
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
