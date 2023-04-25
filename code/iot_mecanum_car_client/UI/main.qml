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
    property real targetPositionYaw: 0
    property real carPositionX: 200
    property real carPositionY: 200
    property real carPositionYaw: 200
    property real gyroscopePosition: 200
    property real carWidth: 100
    property real carHeight: 120
    property real carSpeedX: 0
    property real carSpeedY: 0
    property real carSpeedYaw: 0
    property real carWheel1Speed: 0
    property real carWheel2Speed: 0
    property real carWheel3Speed: 0
    property real carWheel4Speed: 0
    property int carYawMode: 0  //0:锁定 1:不锁定


    //模型视图横纵缩放比 像素点:车位移单位
    property real zoom: 1

    //通知后台构件已经完成构建
    Component.onCompleted:{
        backend.qmlCompleted()
    }

    onTargetPositionXChanged: {
        backend.targetPositionRefresh( targetPositionX , targetPositionY )
    }

    onTargetPositionYChanged: {
        backend.targetPositionRefresh( targetPositionX , targetPositionY )
    }

    onCarYawModeChanged: {
        if( carYawMode === 0 )
        {
            yawSwitch.text = "航向角解锁"
        }else{
            yawSwitch.text = "航向角锁定"
        }
        backend.carYawModeRefresh( carYawMode );
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
                radius: width * 0.01
                clip:true

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        //坐标转换!
                        targetPositionX = mouseX*zoom - parent.width/2
                        targetPositionY = parent.height/2 - mouseY*zoom
                    }
                }

                //实际小车位置视图
                Rectangle{
                    id: curCarModel
                    width: parent.width * 0.06
                    height: width / carWidth * carHeight
                    radius: width * 0.2
                    color: Qt.rgba( 1 , 1 , 1 , 0.5 )

                    x: parent.width/2 + carPositionX*zoom - width/2
                    y: parent.height/2 - carPositionY*zoom - height/2
                    rotation: carPositionYaw

                }

                //目标小车位置视图
                Rectangle{
                    id: targetCarModel
                    color: Qt.rgba( 0 , 0 , 0 , 0.0 )
                    width: parent.width * 0.06 * 1.2
                    height: width / carWidth * carHeight
                    radius: width * 0.2
                    border.width: width * 0.04
                    border.color: "#802a2a"

                    x: parent.width/2 + targetPositionX*zoom - width/2
                    y: parent.height/2 - targetPositionY*zoom - height/2
                    rotation: targetPositionYaw
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
                id: targetPosXLabel
                anchors.top: connectStateLabel.bottom
                text:"目标x坐标:\t" + targetPositionX.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: targetPosYLabel
                anchors.top: targetPosXLabel.bottom
                text:"目标y坐标:\t" + targetPositionY.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: curPosXLabel
                anchors.top: targetPosYLabel.bottom
                text:"当前x坐标:\t" + carPositionX.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: curPosYLabel
                anchors.top: curPosXLabel.bottom
                text:"当前y坐标:\t" + carPositionY.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: targetYawLabel
                anchors.top: curPosYLabel.bottom
                text:"目标航向角:\t" + targetPositionYaw.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: curYawLabel
                anchors.top: targetYawLabel.bottom
                text:"当前航向角:\t" + carPositionYaw.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: carSpeedXLabel
                anchors.top: curYawLabel.bottom
                text:"小车X速度:\t" + carSpeedX.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: carSpeedYLabel
                anchors.top: carSpeedXLabel.bottom
                text:"小车Y速度:\t" + carSpeedY.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: carSpeedYawLabel
                anchors.top: carSpeedYLabel.bottom
                text:"小车Yaw速度:" + carSpeedYaw.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: carWheel1SpeedLabel
                anchors.top: carSpeedYawLabel.bottom
                text:"电机1速度:\t" + carWheel1Speed.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: carWheel2SpeedLabel
                anchors.top: carWheel1SpeedLabel.bottom
                text:"电机2速度:\t" + carWheel2Speed.toFixed(2)
                font.pixelSize: parent.height * 0.05
                color: "#FFFFFF"
            }
            Text{
                id: carWheel3SpeedLabel
                anchors.top: carWheel2SpeedLabel.bottom
                text:"电机3速度:\t" + carWheel3Speed.toFixed(2)
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

        Button{
            id: connectBtn
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: parent.width * 0.03
            anchors.rightMargin: parent.height * 0.02

            width: parent.width * 0.2
            height: parent.height * 0.2
            text: "连接小车"
            font.pixelSize: parent.width * 0.03

            onClicked: {
                backend.autoConnectCar()
            }
        }

        Button{
            id: posZeroBtn
            anchors.left: parent.left
            anchors.top: connectBtn.bottom
            anchors.leftMargin: parent.width * 0.03
            anchors.rightMargin: parent.height * 0.02

            width: parent.width * 0.2
            height: parent.height * 0.2
            text: "目标归零"
            font.pixelSize: parent.width * 0.03

            onClicked: {
                targetPositionX = 0
                targetPositionY = 0
            }
        }

        Button{
            id: yawSwitch
            anchors.left: parent.left
            anchors.top: posZeroBtn.bottom
            anchors.leftMargin: parent.width * 0.03
            anchors.rightMargin: parent.height * 0.02

            width: parent.width * 0.2
            height: parent.height * 0.2
            text: "航向角解锁"
            font.pixelSize: parent.width * 0.03

            onClicked: {
                if( carYawMode )
                    carYawMode = 0
                else
                    carYawMode = 1
            }
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
            color: "#FFFFFF"
            font.pixelSize: parent.height * 0.06
        }
    }

    //软件信息面板
    Item{
        id:appInfoPanel
        width: parent.width
        height: parent.height * 0.03
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        Text{
            anchors.right: parent.righ
            text:"版本号:" + appVersion
            font.pixelSize: parent.height * 0.6
            color: "#ffffff"
        }
    }
}
