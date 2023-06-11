import QtQuick 2.0
import QtQuick.Dialogs
import Qt.labs.platform

Rectangle {
    id:slider_item
    width: 100
    height: 100
    color: "blue"
    border.color:"white"
    ColorDialog {
        id:colorDialog
        //standardButtons: ColorDialog.Ok | ColorDialog.Cancel
        onAccepted: {
            slider_item.color = currentColor
            slider_root.requestPaint();
        }
    }

    MouseArea{
        anchors.fill:parent
        //drag.target:parent
        property var isPressed:false
        property var isDraging:false
        property var hitX:-1
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        drag.target: parent
        drag.axis: Drag.XAndYAxis
        drag.minimumX: 0
        drag.maximumX: slider_root.width - slider_item.width
        drag.minimumY: 0
        drag.maximumY: slider_root.height - slider_item.height

        onPressed:function(mouse){
            if(mouse.button === Qt.RightButton)
            {
                //console.log("destroy")
                slider_item.destroy()
                slider_root.requestPaint();
            }
            else
            {
                isPressed = true;
                isDraging= false;
                hitX = mouseX
            }
        }    
        onReleased:function(mouse){
            if(isDraging)
            {
                slider_root.requestPaint();
            }
            isPressed = false;
            isDraging = false;
            hitX  = width/2.0
        }
        onPositionChanged:function(mouse){
            if(isPressed)
            {
                isDraging = true
                slider_root.requestPaint();
                //slider_item.x += mouseX - hitX
            }
        }
        onDoubleClicked:function(mouse){
            colorDialog.open()      
        }
       
    }
}