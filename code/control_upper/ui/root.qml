import QtQml
import QtQuick

Window {
    id:root
    objectName: "root"  //用于c++端在引擎中查找

    property string windowTitle: "Undefine"

    visible: true
    title: windowTitle

    //控制窗体大小
    minimumWidth: Screen.desktopAvailableWidth * 0.5
    minimumHeight: Screen.desktopAvailableHeight * 0.5
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight

    //响应uiBridge class信号 c++->ui
    function slot_CurrentPosChange( x , y ){
        motion.currentX = x
        motion.currentY = y
    }
    //调用uiBridge class方法 ui->c++
    function signal_TargePosChange( x , y ){
         uiBridge.slot_TargePosChange( x , y )
    }

    Component.onCompleted: {
        uiBridge.Signal_CurrentPosChange.connect(slot_CurrentPosChange);
    }


    Motion{
        id: "motion"
        width: 800
        height: 800
        anchors.centerIn: parent
        visible: true
        currentX: parent.currentX
    }
}
