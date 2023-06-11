import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs
import Qt.labs.platform
import OSGItem
import ProxyItem
import MapperItem
import YQuick 1.0
import QtQuick.Controls.Universal  
Rectangle { 
    anchors.fill:parent 
    id:root_rect
    property var maxCTValue :2500.0
    property var minCTValue:0.0
        Rectangle{
            height:parent.height
            width:100
            color:  Qt.lighter(YConfig.themeColor , 1.2)
            Column{
                id: rectid
                anchors.horizontalCenter:parent.horizontalCenter
                Rectangle{
                    height: 1
                    width:parent.width
                    color: YConfig.themeColor 
                }
                spacing: 3
                Button {
                    id: btnopen
                    width : 80
                    height: 40
                    text: "STL"
                    onClicked:{
                        var request = {}
                        //request["filename"] = "D:/Disk/git/yardbirds/20230418/config/STL/human_head.stl"
                        request["filename"] = cppProxy.getConfigDir()+"/STL/mask.stl"
                        view.invoke("loadSTL",request);
                    }
                }

                Button {
                    id: zoominid
                    text: "Raw"
                    width : 80
                    height: 40
                    onClicked:{
                        var request = {}
                        //request["filename"] = "D:/Disk/git/yardbirds/20230418/config/410x410x264.raw"
                        request["filename"] = cppProxy.getConfigDir() + "/410x410x264.raw"
                        request["dimX"] = 410
                        request["dimY"] = 410
                        request["dimZ"] = 264
                        view.invoke("loadRaw",request);
                    }
                }
                Button {
                    id: resetid
                    width : 80
                    height: 40
                    text: "复位"
                    onClicked:{
                    }
                }
                // Slider {
                //     id: sliderid
                // // y:resetid.height + resetid.y +  10
                //     from: 1
                //     value: 0
                //     to: 10
                //     stepSize:1
                //     onValueChanged:{
                //         openGLItem.ImageChanged(value)
                //         openGLItem1.ImageChanged(value)
                //         openGLItem2.ImageChanged(value)
                //     }
                // }
            }

        }
        Rectangle{
            x:100
            height:parent.height
            width:parent.width - 100
            //color:"black"
            color:  Qt.lighter(YConfig.themeColor , 1.2)
             //1x2
            Row{
                id: rowid
                    spacing:10
                    height:parent.height/2.0
                    width:parent.width
                    property int mouseStyle:2
                    Rectangle { 
                        color:"red"
                        width:parent.width/2.0
                        height:parent.height

                        OSGItem
                        {
                            id: openGLItem
                            visible: true
                            //anchors.fill:parent//不明原因，用这句后MouseArea没有反应

                            width:parent.width
                            height:parent.height

                            MapperItem
                            {
                                id:volumeMapper2
                                property var objectName:"volumeMapper2"
                                property var mapperType:"OpenglVolumeMapper"   
                            }
                            MouseArea{
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                property var lastX: 0
                                property var lastY: 0
                                property int moveState: 0
                                property var control: parent 
                                property var isLeftButtonPressed:false
                                //cursorShape:  (containsMouse?(pressed?Qt.CloseHandCursor:Qt.OpenHandCursor):Qt.ArrowCursor)
                                cursorShape: rowid.mouseStyle<2 ? (rowid.mouseStyle===1 ? Qt.IBeamCursor : Qt.OpenHandCursor) : Qt.ArrowCursor // 1-窗宽窗位，0-拖动，
                                preventStealing: true
                                onPressed:function(mouse){

                                    lastX = mouseX
                                    lastY = mouseY
                                    if(mouse.button === Qt.LeftButton){
                                        //console.log("Qt.LeftButton is pressed" + mouseX + " " + mouseY);
                                        // 复原
                                        //openGLItem.MouseLeftBtnPressed(0, 0)
                                        isLeftButtonPressed = true
                                    }
                                    else if(mouse.button === Qt.RightButton){
                                        //console.log("Qt.RightButton is pressed" + mouseX + " " + mouseY);
                                        isLeftButtonPressed = false
                                    }
                                }
                                onPositionChanged:function(mouse){
                                    if(pressed && mouseX!=0 && isLeftButtonPressed) //
                                    {
                                        //openGLItem.MouseLeftDrag(-(mouseX-lastX), mouseY-lastY);
                                        //openGLItem.MouseLeftDrag(mouseX-lastX, mouseY-lastY);
                                        var request = {}
                                        request["value"] = -(lastX-mouseX)/5.0;
                                        request["x"] = 0.0;
                                        request["y"] = 1.0;
                                        request["z"] = 0.0;
                                        view.invoke("rotateVolume",request);


                                        request["value"] = -(lastY- mouseY)/5.0;
                                        request["x"] = 1.0;
                                        request["y"] = 0.0;
                                        request["z"] = 0.0;
                                        view.invoke("rotateVolume",request);       
                                    }
                                    else if(pressed && mouseX!=0 &&!isLeftButtonPressed)
                                    {
                                        //openGLItem.MouseRightDrag(mouseX-lastX, mouseY-lastY);
                                         var request = {}
                                        request["x"] = -(mouseX- lastX)/(openGLItem.width/2.0);
                                        request["y"] = (mouseY - lastY)/(openGLItem.width/2.0);
                                        request["z"] = 0.0;
                                        view.invoke("translateVolume",request);

                                    }
                                    lastX = mouseX
                                    lastY = mouseY
                                }
                    
                                onWheel:{
                                    if (wheel.angleDelta.y > 0) {
                                        openGLItem.ZoomOut(); 
                                    } else if (wheel.angleDelta.y < 0) {
                                        openGLItem.ZoomIn();
                                    }
                                }
                            }

                        }

                    }
                    Rectangle { 
                        color:"blue"
                        width:parent.width/2.0
                        height:parent.height

                        OSGItem//不明原因，要放在Rectangle才能适应窗口的大小变化重新布局
                        {
                            id: openGLItem1
                            visible: true
                            //anchors.fill:parent//不明原因，用这句后MouseArea没有反应
                            width:parent.width 
                            height:parent.height
                           

                            MapperItem
                            {
                                id:polyMapper2
                                property var objectName:"polyMapper2"
                                property var mapperType:"OpenglPolyMapper"
                            }

                            MouseArea{
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                property int lastX: 0
                                property int lastY: 0
                                property int moveState: 0
                                property var isLeftButtonPressed:false
                                property var control: parent 
                                cursorShape: rowid.mouseStyle<2 ? (rowid.mouseStyle===1 ? Qt.IBeamCursor : Qt.OpenHandCursor) : Qt.ArrowCursor // 1-窗宽窗位，0-拖动，
                                preventStealing: true//scrollview里边要这个参数才能正常进入onPositionChanged
                                
                                onPressed:function(mouse){
                                    lastX = mouseX
                                    lastY = mouseY
                                    if(mouse.button === Qt.LeftButton){
                                        //openGLItem.MouseLeftBtnPressed(0, 0)
                                        isLeftButtonPressed = true
                                    }
                                    else if(mouse.button === Qt.RightButton){
                                        isLeftButtonPressed = false
                                    }
                                }
                                onPositionChanged:function(mouse){
                                
                                    if(pressed &&mouseX!=0&& isLeftButtonPressed) //
                                    {       
                                       //openGLItem1.MouseLeftDrag(mouseX-lastX, mouseY-lastY);
                                        var request = {}
                                        request["value"] = (lastX - mouseX)/5.0;
                                        request["x"] = 0.0;
                                        request["y"] = 1.0;
                                        request["z"] = 0.0;
                                        view.invoke("rotatePoly",request);

                                        request["value"] = (lastY- mouseY)/5.0;
                                        request["x"] = 1.0;
                                        request["y"] = 0.0;
                                        request["z"] = 0.0;
                                        view.invoke("rotatePoly",request);
                                    }
                                    else if(pressed && mouseX!=0 &&!isLeftButtonPressed)
                                    {
                                        //openGLItem1.MouseRightDrag(mouseX-lastX, mouseY-lastY);
                                         var request = {}
                                        request["x"] = -(mouseX- lastX)/(openGLItem1.width/2.0);
                                        request["y"] = (mouseY - lastY)/(openGLItem1.width/2.0);
                                        request["z"] = 0.0;
                                        view.invoke("translatePoly",request);     
                                    }
                                    lastX = mouseX
                                    lastY = mouseY
                                }

                                onWheel:{
                                    if (wheel.angleDelta.y > 0) {
                                        openGLItem1.ZoomOut(); 
                                    } else if (wheel.angleDelta.y < 0) {
                                        openGLItem1.ZoomIn();
                                    }
                                }
                            }
                           
                        }
                    }
                }
                Rectangle{
                    y:parent.height/2.0 + 10
                    width:parent.width/2.0
                    height:parent.height/2.0
                    color:"red"
                    OSGItem{
                        id: openGLItem2
                        visible: true
                        width:parent.width
                        height:parent.height
                        MapperItem
                        {
                            id:volumeMapper
                            property var objectName:"volumeMapper"
                            property var mapperType:"OpenglVolumeMapper"                   
                        }

                        MapperItem
                        {
                            id:polyMapper
                            property var objectName:"polyMapper"
                            property var mapperType:"OpenglPolyMapper"
                        }

                        MouseArea{
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            property int lastX: 0
                            property int lastY: 0
                            property int moveState: 0
                            property var control: parent 
                            property var isLeftButtonPressed:false
                            cursorShape: rowid.mouseStyle<2 ? (rowid.mouseStyle===1 ? Qt.IBeamCursor : Qt.OpenHandCursor) : Qt.ArrowCursor // 1-窗宽窗位，0-拖动，
                            preventStealing: true//scrollview里边要这个参数才能正常进入onPositionChanged
                  
                            onPressed:function(mouse){
                                lastX = mouseX
                                lastY = mouseY
                                if(mouse.button === Qt.LeftButton){
                                    //openGLItem.MouseLeftBtnPressed(0, 0)
                                    isLeftButtonPressed = true
                                }
                                else if(mouse.button === Qt.RightButton){
                                    isLeftButtonPressed = false
                                }
                            }
                            onPositionChanged:function(mouse){
                                if(pressed &&mouseX!=0&& isLeftButtonPressed) //
                                {
                                    //openGLItem2.MouseLeftDrag(mouseX-lastX, mouseY-lastY);
                                     {
                                        var request = {}
                                        request["value"] = (lastX - mouseX)/5.0;
                                        request["x"] = 0.0;
                                        request["y"] = 1.0;
                                        request["z"] = 0.0;
                                        view.invoke("rotateComposeP",request);

                                        request["value"] = (lastY - mouseY)/5.0;
                                        request["x"] = 1.0;
                                        request["y"] = 0.0;
                                        request["z"] = 0.0;
                                        view.invoke("rotateComposeP",request);
                                    }
                                    var request = {}
                                    request["value"] = -(lastX - mouseX)/5.0;
                                    request["x"] = 0.0;
                                    request["y"] = 1.0;
                                    request["z"] = 0.0;
                                    view.invoke("rotateComposeV",request);

                                    request["value"] = -(lastY - mouseY)/5.0;
                                    request["x"] = 1.0;
                                    request["y"] = 0.0;
                                    request["z"] = 0.0;
                                    view.invoke("rotateComposeV",request);
 
                                }
                                else if(pressed && mouseX!=0 &&!isLeftButtonPressed)
                                {
                                    openGLItem2.MouseRightDrag(mouseX-lastX, mouseY-lastY)
                                }
                                lastX = mouseX
                                lastY = mouseY
                            }

                            onWheel:{
                                if (wheel.angleDelta.y > 0) {
                                    //console.log("backward")
                                    openGLItem2.ZoomOut(); 
                                } else if (wheel.angleDelta.y < 0) {
                                    //console.log("forward")
                                    openGLItem2.ZoomIn();
                                }
                            }
                        }
                    }
                }
                Rectangle{
                    y:parent.height/2.0 + 10
                    x:parent.width/2.0 + 10
                    width:parent.width/2.0
                    height:parent.height/2.0
                    color:"gray"
                   
                    Canvas {
                        id:slider_root
                        width :parent.width
                        height:60
                        property var slider_item_width:15
                        onPaint: {   

                            //
                            var request = {}
                            request["points"] = []
                            var num = 0   

                            //openGLItem2.clearColorStops()
                            //openGLItem2.addColorStop(0,"black");

                            request["points"][num]={}
                            request["points"][num]["pos"] = 0.0;
                            request["points"][num]["r"] = 0;
                            request["points"][num]["g"] = 0;
                            request["points"][num]["b"] = 0;
                            num++;

                            for (var i = 0; i < slider_root.children.length; i++) {
                                if(slider_root.children[i] instanceof ColorSlider)
                                {
                                    var pos = (slider_root.children[i].x+slider_item_width) / slider_root.width
                                    //openGLItem2.addColorStop(pos,slider_root.children[i].color);

                                    var r, g, b, a;
                                    r = slider_root.children[i].color.r
                                    g = slider_root.children[i].color.g
                                    b = slider_root.children[i].color.b
                                    a = slider_root.children[i].color.a
                                    //console.log(r, g, b, a)
                                    request["points"][num]={}
                                    request["points"][num]["pos"] = pos;
                                    request["points"][num]["r"] = r;
                                    request["points"][num]["g"] = g;
                                    request["points"][num]["b"] = b;
                                    num++;

                                }                                
                            }
                            //openGLItem2.addColorStop(1,"white");


                            request["points"][num]={}
                            request["points"][num]["pos"] = 1.0;
                            request["points"][num]["r"] = 1.0;
                            request["points"][num]["g"] = 1.0;
                            request["points"][num]["b"] = 1.0;
                            num++;

                            view.invoke("setColor",request);

                            var ctx = getContext("2d")
                            var gradient = ctx.createLinearGradient(0, 0, width, 0)
                            //var table = openGLItem2.getColorTable()
                            var table = volumeMapper.getColorTable()
                            
                            for(var i = 0 ;i<(table.length/4);i++)
                            {
                                var pos =  i /(table.length/4)
                                var color = Qt.rgba(table[i*4],table[i*4+1],table[i*4+2],1.0)
                                //var color = Qt.rgba(0.33, 1, 1, 0.5)
                                //console.log(pos + ":("+color.r+","+color.g+","+color.b+","+color.a+")")
                                gradient.addColorStop(pos, color)
                            }
                            ctx.fillStyle = gradient
                            ctx.fillRect(0, 0, width, height)
                        }
                        MouseArea{
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton 
                            
                            property var control: parent 
                            cursorShape: rowid.mouseStyle<2 ? (rowid.mouseStyle===1 ? Qt.IBeamCursor : Qt.OpenHandCursor) : Qt.ArrowCursor // 1-窗宽窗位，0-拖动，
                            preventStealing: true
                            propagateComposedEvents:true
                            property var isPressed:false
                            property var isDraging:false
                            onPressed:function(mouse){
                                isPressed = true;
                                isDraging= false;
                            }    
                            onReleased:function(mouse){
                                if(isDraging == false)
                                {
                                    var component = Qt.createComponent("ColorSlider.qml");
                                    component.createObject(slider_root, {"x": mouseX-slider_root.slider_item_width, "y": 0,"width":slider_root.slider_item_width*2,"height":60});
                                }
                                slider_root.requestPaint()
                                isPressed = false;
                                isDraging = false;
                            }
                            onPositionChanged:function(mouse){
                                if(isPressed)
                                {
                                    isDraging = true
                                }
                            }
                        }
                    }
                    Canvas {
                        id:opacity_root
                        width :parent.width - 15
                        height:parent.height - 75
                        y:60
                        property var slider_item_radius:5
                        onPaint: { 

                            var ctx = getContext("2d")
                            ctx.clearRect(0, 0, width, height)


                            //绘制网格
                            ctx.strokeStyle = "white"
                            ctx.lineWidth = 1
                            ctx.beginPath()
                            ctx.moveTo(0, height-0)
                            for(var i = 0;i<=10;i++)
                            {
                                ctx.moveTo(0, height- i*height/10)
                                ctx.lineTo(width, height- i*height/10)
                                ctx.moveTo(width/10*i, 0)
                                ctx.lineTo(width/10*i, height)
                            }
                            ctx.stroke() // 绘制路径


                            //绘制直方图
                            ctx.fillStyle = "yellow"
                            var hist = volumeMapper.getHistTable()
                            // var hist = []
                            // for(var i = 0 ;i<80;i++)
                            // {
                            //     //hist[i] = 0.001* (i%20)

                            //     hist[i] = 0.1* Math.random()
                            // }
                            
                            var rectwidth = width / (hist.length*1.5);
                            var hist_scale = hist.length / 10;//放大倍数
                            for(var i = 0 ;i<hist.length;i++)
                            {
                                var h = hist[i]*height*hist_scale
                                var w = width * i / hist.length
                                ctx.fillRect(w, height-h, rectwidth, h)
                            }
                           

                            //绘制透明度曲线
                            var request = {}
                            request["points"] = []
                            var num = 0             

                            //openGLItem2.clearOpacityStops()
                            //openGLItem2.addOpacityStop(1.0,0.0);
                            
                            
                            request["points"][num]={}
                            request["points"][num]["x"] = 1.0;
                            request["points"][num]["y"] = 0;
                            num++;

                            for (var i = 0; i < opacity_root.children.length; i++) 
                            {
                                if(opacity_root.children[i] instanceof OpacitySlider)
                                {
                                    var posx = opacity_root.children[i].x + slider_item_radius 
                                    var posy = opacity_root.children[i].y + slider_item_radius 
                             
                                    var ctvalue = posx / width * root_rect.maxCTValue + root_rect.minCTValue;
                                    var opacity = (height - posy) / height * 0.3;
                                    if(opacity<0)
                                    {
                                        opacity = 0 ;
                                    }
                                    //console.log("ctvalue:" + ctvalue + " opacity:" + opacity)
                                    //openGLItem2.addOpacityStop(ctvalue,opacity);
                                    request["points"][num]={}
                                    request["points"][num]["x"] = ctvalue;
                                    request["points"][num]["y"] = opacity;
                                    num++

                                }                                
                            }
                            

                            //openGLItem2.addOpacityStop(root_rect.maxCTValue,0.0);
                            request["points"][num]={}
                            request["points"][num]["x"] = root_rect.maxCTValue;
                            request["points"][num]["y"] = 0.0;
                            num++
                            view.invoke("setOpacity",request);


                            //var array = openGLItem2.getOpacityTable()
                            var array = volumeMapper.getOpacityTable()

                            // 设置画笔颜色和宽度
                            ctx.strokeStyle  = "green"
                            ctx.lineWidth = 5
                            ctx.beginPath()
                            ctx.moveTo(0, height-0)
                       
                            for(var i = 0 ;i<array.length;i++)
                            {
                                var posx = width* i/array.length;
                                var posy = height - (height * array[i]/0.3)
                                ctx.lineTo(posx, posy)
                            }
                            ctx.stroke() // 绘制路径                         
                        }

                        MouseArea{
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton 
                            
                            property var control: parent 
                            cursorShape: rowid.mouseStyle<2 ? (rowid.mouseStyle===1 ? Qt.IBeamCursor : Qt.OpenHandCursor) : Qt.ArrowCursor // 1-窗宽窗位，0-拖动，
                            preventStealing: true
                            propagateComposedEvents:true
                            property var isPressed:false
                            property var isDraging:false
                            onPressed:function(mouse){
                                isPressed = true;
                                isDraging= false;
                            }    
                            onReleased:function(mouse){
                                if(isDraging == false)
                                {
                                    var component = Qt.createComponent("OpacitySlider.qml");
                                    component.createObject(opacity_root, {"x": mouseX - opacity_root.slider_item_radius, "y": mouseY-opacity_root.slider_item_radius,"width":opacity_root.slider_item_radius*2,"height":opacity_root.slider_item_radius*2});
                                }

                                opacity_root.requestPaint()

                                isPressed = false;
                                isDraging = false;
                            }
                            onPositionChanged:function(mouse){
                                if(isPressed)
                                {
                                    isDraging = true
                                }
                            }
        
                        }
                        ProxyItem{
                            id:view
                            Component.onCompleted:
                            {
                               //view.registerYModule("View")//need to register to recieved signal 
                            }
                        }
                        Component.onCompleted:{
                            var component = Qt.createComponent("OpacitySlider.qml");
                            var mouseX = 320;
                            var mouseY = 200;
                            component.createObject(opacity_root, {"x": mouseX - opacity_root.slider_item_radius, "y": mouseY-opacity_root.slider_item_radius,"width":opacity_root.slider_item_radius*2,"height":opacity_root.slider_item_radius*2});

                            mouseX = 230;
                            mouseY = height;
                            component.createObject(opacity_root, {"x": mouseX - opacity_root.slider_item_radius, "y": mouseY-opacity_root.slider_item_radius,"width":opacity_root.slider_item_radius*2,"height":opacity_root.slider_item_radius*2});
                        }
                     }
                }
        }
    //}
   
    Component.onDestruction: {                    
        polyMapper2.unRegisterYModule()
        volumeMapper2.unRegisterYModule()
        polyMapper.unRegisterYModule()
        volumeMapper.unRegisterYModule()
        view.unRegisterYModule();
        cppProxy.stopCppSequence()  
    }
    Component.onCompleted:{
        window.width = 1024 
        window.height = 768
       
        view.registerYModule("View")//need to register to recieved signal 

        polyMapper2.createMapper()
        cppProxy.registerYModule(polyMapper2.objectName)
        volumeMapper2.createMapper()
        cppProxy.registerYModule(volumeMapper2.objectName)
        volumeMapper.createMapper()
        cppProxy.registerYModule(volumeMapper.objectName)
        polyMapper.createMapper()
        cppProxy.registerYModule(polyMapper.objectName)
        cppProxy.startCppSequence("Opengl.cfg")


        var request = {}

        request["value"] = -90.0;
        request["x"] = 1.0;
        request["y"] = 0.0;
        request["z"] = 0.0;
        view.invoke("rotateVolume",request);

        request["value"] = 180;
        request["x"] = 0.0;
        request["y"] = 0.0;
        request["z"] = 1.0;
        view.invoke("rotateVolume",request);    

        var request = {}

        request["value"] = 90.0;
        request["x"] = 1.0;
        request["y"] = 0.0;
        request["z"] = 0.0;
        view.invoke("rotatePoly",request);  

    }
}
