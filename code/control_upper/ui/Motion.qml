import QtQml
import QtQuick

Item {
    visible: false

    property int currentX: 0
    property int currentY: 0
    property int targetX: 0
    property int targetY: 0

    Rectangle{
        id: mapBackground
        anchors.fill: parent
        color: "#00ff00"

        Image {
            id: targetPosImg
            clip: false
            x :  parent.width/2 - width / 2
            y :  parent.height/2 - height / 2
            source: "./img/test.jpg"
        }

        Image {
            id: currentPosImg
            clip: false
            x: currentX
            y: currentY
            source: "./img/test.jpg"
        }

        MouseArea{
            anchors.fill: parent

            onPressed: {
                targetPosImg.x = mouseX - targetPosImg.width/2
                targetPosImg.y = mouseY - targetPosImg.height/2
                signal_TargePosChange(mouseX,mouseY)
            }

            onPositionChanged: {
                targetPosImg.x = mouseX - targetPosImg.width/2
                targetPosImg.y = mouseY - targetPosImg.height/2
                signal_TargePosChange(mouseX,mouseY)
            }
        }
    }
}
