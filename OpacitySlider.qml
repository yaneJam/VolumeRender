import QtQuick 2.0
import QtQuick.Dialogs
import Qt.labs.platform

Rectangle {
    id:slider_item
    width: 10
    height: 10
    color: "blue"
    radius:slider_item.width

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
        drag.maximumX: opacity_root.width - slider_item.width/2.0
        drag.minimumY: 0
        drag.maximumY: opacity_root.height - slider_item.width/2.0

        onPressed:function(mouse){
            if(mouse.button === Qt.RightButton)
            {
                //console.log("destroy")
                slider_item.destroy()
                opacity_root.requestPaint();
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
                
                opacity_root.requestPaint();
            }
            isPressed = false;
            isDraging = false;
            hitX  = width/2.0
        }
        onPositionChanged:function(mouse){
            if(isPressed)
            {
                //console.log("isDraging")
                isDraging = true
                opacity_root.requestPaint();
                //slider_item.x += mouseX - hitX
            }
        }
     
       
    }
}