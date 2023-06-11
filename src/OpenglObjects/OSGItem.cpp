//#include "OSGItem.h"
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QQmlProperty>
#include <QQmlContext>

//************OSGItemRender************//


OpenglRenderer::OpenglRenderer( OSGItem* parent)
{
    
    mPolyModelMatrix.setToIdentity();
    mPolyModelMatrix.rotate(90, QVector3D(1.0, 0.0, 0.0));


    mVolumeModelMatrix.setToIdentity();
    mVolumeModelMatrix.rotate(-90, QVector3D(1.0, 0.0, 0.0));
    mVolumeModelMatrix.rotate(180, QVector3D(0.0, 0.0, 1.0));


    //mModelMatrix.rotate(90, QVector3D(0.0, 1.0, 0.0));

    mCamaraMatrix.setToIdentity();
    mCamaraMatrix.lookAt(
        QVector3D(0.0f, 0.0f, 0.0f),
        QVector3D(0.0f, 0.0f, -0.0f),
        QVector3D(0.0f, 0.0f, 1.0f));


    //暂时先这样
    m_parent = parent;
    _tcmd(cmd);
    cmd->method = "invalid";//先搞个空操作进去
    cmds.push_back(cmd);

    //cmds_iter = cmds.begin();
    cmd_index = 0;


    // frame_buffer_id = 0;
     Framebuffercolor = 0;
	// Framebufferdepth = 0;
}

//每一个指令存到备忘录里边，后面改进的时候用于前进，撤销
void OpenglRenderer::doCommand(_tsptr<yCMD> cmd, bool isTemp)
{
    if (!isTemp)
    {
        int size = cmd_index + 1;
        int cmdsize = cmds.size();

        cmds.push_back(cmd);
       // forwardCommand();//放到渲染线程，即render()函数里边运行，Opengl的代码不适合运行在调用doCommand的这个线程
        new_cmd++;
    }
    //  DEBUG_LOG("step2");
}

void  OpenglRenderer::forwardCommand()
{
    int size = cmd_index + 1;
    int cmdsize = cmds.size();
    if (size < cmds.size())
    {   
        cmd_index++;
      
        _tsptr<yCMD>cmd = cmds.at(cmd_index);
        _tstring method = cmd->method;

        if (method == "addMapper")
        {
            addMapper(cmd);//
        }
        else if (method == "ImageChanged")
        {
            ImageChanged(cmd);
        }
        else if (method == "AdjustifyWindows")
        {
            AdjustifyWindows(cmd);
        }
        else if (method == "MouseLeftBtnPressed")
        {
            MouseLeftBtnPressed(cmd);
        }
        else if (method == "MouseRightBtnPressed")
        {
            MouseRightBtnPressed(cmd);
        }
        else if (method == "Reset")
        {
            for (auto mapper : mappers)
            {
                if (mapper)
                {
                    mapper->isReset = true;
                }
            }
        }
        else if (method == "Contrast")
        {
            for (auto mapper : mappers)
            {
                if (mapper)
                {
                    mapper->isContrast = true;
                }
            }
        }
        else if (method == "Drag")
        {
            for (auto mapper : mappers)
            {
                if (mapper)
                {
                    mapper->isDrag = true;
                }
            }
        }
        else
        {
            DEBUG_LOG("method not supported %s", method.c_str());
        }
    }
    else
    {
        ERROR_LOG("");
    }
}

yModule::RESULT OpenglRenderer::ImageChanged(_tsptr<yCMD> cmd)
{
    for (auto mapper : mappers)
    {
        if (mapper)
        {
            mapper->isImageChanged = true;
            sliderValue = (*cmd->request)["value"].asInt();
        }
    }
    return yModule::SUCCESS;
}

yModule::RESULT OpenglRenderer::AdjustifyWindows(_tsptr<yCMD> cmd)
{
    for (auto mapper : mappers)
    {
        if (mapper)
        {
            mapper->isAdjustifyWindow = true;
        }
    }
    return yModule::SUCCESS;
}



yModule::RESULT OpenglRenderer::MouseLeftBtnPressed(_tsptr<yCMD> cmd)
{
    for (auto mapper : mappers)
    {
        if (mapper)
        {
            mapper->isLeftPressed = true;

            x = (*cmd->request)["x"].asDouble();
            y = (*cmd->request)["y"].asDouble();
        }
    }
    return yModule::SUCCESS;
}

yModule::RESULT OpenglRenderer::MouseRightBtnPressed(_tsptr<yCMD> cmd)
{
    for (auto mapper : mappers)
    {
        if (mapper)
        {
            mapper->isRightPressed = true;
            x = (*cmd->request)["x"].asDouble();
            y = (*cmd->request)["y"].asDouble();
        }
    }
    return yModule::SUCCESS;
}


yModule::RESULT OpenglRenderer::addMapper(_tsptr<yCMD> cmd)
{
    _tsptr<OpenglMapper> mapper;
    if ((*cmd->request)["mapperType"].asString() == "OpenglMapper")
    {
        mapper = _tsptr<OpenglMapper>(new OpenglMapper);
    }
    else   if ((*cmd->request)["mapperType"].asString() == "OpenglImageMapper")
    {
        mapper = _tsptr<OpenglImageMapper>(new OpenglImageMapper);
    }
    else   if ((*cmd->request)["mapperType"].asString() == "OpenglAxeMapper")
    {
        mapper = _tsptr<OpenglAxeMapper>(new OpenglAxeMapper);
    }
    else   if ((*cmd->request)["mapperType"].asString() == "OpenglPolyMapper")
    {
        mapper = _tsptr<OpenglPolyMapper>(new OpenglPolyMapper);
    }
    else if ((*cmd->request)["mapperType"].asString() == "OpenglVolumeMapper")
    {
        hasVolume  = true;//暂时在这里标记
        mapper = _tsptr<OpenglVolumeMapper>(new OpenglVolumeMapper);    
        //mapper->isInit = false;
    }
    else if ((*cmd->request)["mapperType"].asString() == "OpenglSliceMapper")
    {   
         mapper = _tsptr<OpenglSliceMapper>(new OpenglSliceMapper);
        //mapper->isInit = false;
    }
    else
    {
        ERROR_LOG("mapper not support :%s", (*cmd->request)["mapperType"].asString());
    }

    
    mapper->setTempContext(cmd->request);//先设置renderer再setTempContexts
    mapper->renderer = this;
    //static int apple;
    //mappers.insert(mapper->title+QString("%1").arg(apple++), mapper);


    DEBUG_LOG("addMapper [%s]",mapper->title.toStdString().c_str());
    mappers.insert(mapper->title, mapper);
    //ySingletone::get()->removeModule(mapper->getInstanceName());
    ySingletone::get()->addModule(mapper);
    newest_mapper = mapper;
    return yModule::SUCCESS;
}




bool OpenglRenderer::createAndBindFrameBuffer()
{

    int rectx = framebufferObject()->width();
    int recty = framebufferObject()->height();
	if (!frame_buffer_id)//创建帧缓存
	{
		glFunc.glGenFramebuffers(1, &frame_buffer_id);
	}
	glFunc.glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

	if (!Framebuffercolor)
	{
		glFunc.glGenTextures(1, &Framebuffercolor);
	}
	glFunc.glBindTexture(GL_TEXTURE_2D, Framebuffercolor);
	glFunc.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rectx, recty, 0, GL_RGB, GL_FLOAT, 0);
	glFunc.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFunc.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	/*GLfloat borderColor[4]={1.0,0.0,1.0,1.0};
	glFunc.glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR, borderColor);*/
	glFunc.glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Framebuffercolor, 0);
	glFunc.glBindTexture(GL_TEXTURE_2D, 0);

	if (!Framebufferdepth)
	{
		glFunc.glGenTextures(1, &Framebufferdepth);
	}
	glFunc.glBindTexture(GL_TEXTURE_2D, Framebufferdepth);
	glFunc.glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, rectx, recty, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFunc.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFunc.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFunc.glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Framebufferdepth, 0);
	glFunc.glBindTexture(GL_TEXTURE_2D, 0);


	if (frame_buffer_id)
	{
		GLenum fbostatus = glFunc.glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fbostatus != GL_FRAMEBUFFER_COMPLETE)
		{
			MessageBox(NULL, _T(""), _T("createAndBindFrameBuffer()"), MB_OK);
		}
	}

//     // 方案二(Framebuffercolor获取不到深度信息)
//     QOpenGLFramebufferObjectFormat format;
//     format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
//     //format.setMipmap(false);
//     //format.setSamples(4);
//     //format.setTextureTarget(GL_TEXTURE_2D);
//     //format.setInternalTextureFormat(GL_RGBA32F_ARB);
//     //format.setInternalTextureFormat(GL_DEPTH_COMPONENT24);
//     QOpenGLFramebufferObject fbo(framebufferObject()->size(),format);

//     //QOpenGLFramebufferObject fbo(framebufferObject()->size(), QOpenGLFramebufferObject::Depth);
//     //fbo.addColorAttachment(framebufferObject()->width(), framebufferObject()->height(), GL_RGBA8);
//     //fbo.addColorAttachment(framebufferObject()->width(), framebufferObject()->height(), GL_RGBA8);

//     // // 获取并打印帧缓存对象的附件类型
//     // QOpenGLFramebufferObject::Attachment attachment = fbo.attachment();
//     // qDebug() << "attachment type:" << attachment;

  

//     auto textur_list = fbo.textures();
//     if(textur_list.size()>0)
//     {
//         DEBUG_LOG("textur_list %d %d %d ",textur_list.size(),framebufferObject()->width(),framebufferObject()->height());
//         Framebuffercolor = textur_list[0];
//     }
//     fbo.bind();
//     // GLenum nothing[1] = {GL_NONE};
//     // glFunc.glDrawBuffers(1,nothing);
//     // glFunc.glReadBuffer(GL_NONE);
//     // fbo.release();
//     // fbo.bind();
//      // 解除绑定新的帧缓存对象，并恢复到原来的帧缓存对象
//      if(depthTexture==0)
//      {
//         depthTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
//         depthTexture->setSize(framebufferObject()->width(), framebufferObject()->height());
//         depthTexture->setFormat(QOpenGLTexture::RGBA32F);
//         //depthTexture->setFormat(QOpenGLTexture::DepthFormat);
//         depthTexture->allocateStorage();
//      }
//     // 将深度纹理绑定到帧缓存对象的深度附件上
//     //GL_DEPTH_ATTACHMENT
//     //GL_DEPTH_STENCIL_ATTACHMENT
//     //GL_COLOR_ATTACHMENT0
//     glFunc.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthTexture->textureId(), 0);
//      Framebufferdepth = depthTexture->textureId(); 
//     // DEBUG_LOG("Framebufferdepth[%d]",Framebufferdepth);
//     //Framebufferdepth= fbo.textures()[0];
//     // // 解除绑定新的帧缓存对象，并恢复到原来的帧缓存对象
//     //  if(colorTexture==0)
//     //  {
//     //     colorTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
//     //     colorTexture->setSize(framebufferObject()->width(), framebufferObject()->height());
//     //     colorTexture->setFormat(QOpenGLTexture::RGBA32F);
//     //     colorTexture->allocateStorage();
//     //  }
//     // 将深度纹理绑定到帧缓存对象的深度附件上
//     //glFunc.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->textureId(), 0);
//     //Framebuffercolor = colorTexture->textureId(); 
//     //Framebuffercolor = fbo.texture();

    return true;
}

void OpenglRenderer::resetFrameBuffer()
{
    //    // QImage image = fbo.toImage(true, 0);
    //     //image.save("D:/fbo.png");
    //     fbo.release();
    glFunc.glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
}

void OpenglRenderer::updatePalette()
{
    std::lock_guard<std::recursive_mutex> lock(mtx); 
    if(color_table_changed == true)
    {
        color_table_changed = false;
        float table[1024*4];
        m_parent->color_palette->getColorTable(table,1024);
    
        for(auto mapper:mappers)
        {
            if(typeid(*mapper) == typeid(OpenglVolumeMapper))
            {
                //mapper.dynamicCast<OpenglVolumeMapper>()->setColorTable(table,1024);        
                _tdynamic_pointer_cast<OpenglVolumeMapper>(mapper)->setColorTable(table,1024);        
            }
        }
    }

    if(opacity_table_changed == true)
    {
        opacity_table_changed = false;
        float table[1024];
        m_parent->opacity_palette->getOpacityTable(table,1024);
        for(auto mapper:mappers)
        {
            if(typeid(*mapper) == typeid(OpenglVolumeMapper))
            {
                //mapper.dynamicCast<OpenglVolumeMapper>()->setOpacityTable(table,1024);
                _tdynamic_pointer_cast<OpenglVolumeMapper>(mapper)->setOpacityTable(table,1024);
            }
        }
    }
}

void OpenglRenderer::render()
{
    //mModelMatrix.rotate(1, QVector3D(0.0f, 1.0f, 0.0f));
    if (inited == false)
    {
        glFunc.initializeOpenGLFunctions();
    }
    for (; new_cmd>0;new_cmd--)
    {
        forwardCommand();
    }
    updatePalette();

    GLint oldViewport[4] = {0};
    //方案三，不用QOpenGLFramebufferObject，
    if(hasVolume == true)
    {
        glFunc.glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);
        glFunc.glGetIntegerv(GL_VIEWPORT, oldViewport);
        createAndBindFrameBuffer();
    }
    

    // 设置视口，投影矩阵和模型视图矩阵
    glFunc.glViewport(0, 0,framebufferObject()->width(), framebufferObject()->height());

    // 在新的帧缓存对象上进行一些渲染操作

    glFunc.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glFunc.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFunc.glEnable(GL_DEPTH_TEST);

    for (auto mapper : mappers)
    {
        if (mapper->isInit == false)
            mapper->initTemp();

        if (mapper->isImageChanged == true)
            mapper->ImageChanged(sliderValue);
                
        if (mapper->isAdjustifyWindow == true)
            mapper->AdjustifyWindows();

        if (mapper->isLeftPressed == true)
            mapper->MouseLeftBtnPressed(x, y);

        if (mapper->isRightPressed == true)
            mapper->MouseRightBtnPressed(x, y);

        if (mapper->isReset == true)
            mapper->Reset();

        if (mapper->isContrast == true)
            mapper->Contrast();

        if (mapper->isDrag == true)
            mapper->Drag();


        if(typeid(*mapper) != typeid(OpenglVolumeMapper) )
        {
            mapper->paint();
        }
    }
   


    if(hasVolume == true)
    {
        resetFrameBuffer();
       
         // 恢复视口，投影矩阵和模型视图矩阵
        glFunc.glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
        glFunc.glDepthMask(true);
        glFunc.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glFunc.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFunc.glEnable(GL_DEPTH_TEST);

        for (auto mapper : mappers)
        {
           
            if(typeid(*mapper) == typeid(OpenglVolumeMapper))
            {
                mapper->paint();
            }
        }
    }
    
    // m_render.paint();
    // m_window->resetOpenGLState();
}

QOpenGLFramebufferObject* OpenglRenderer::createFramebufferObject(const QSize& size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    //m_render.resize(size.width(), size.height());
    return new QOpenGLFramebufferObject(size, format);
}

void OpenglRenderer::synchronize(QQuickFramebufferObject* item)
{
    OSGItem* pItem = qobject_cast<OSGItem*>(item);
    if (pItem)
    {
        if (!m_window)
        {
            m_window = pItem->window();
        }
    }
}

//************OSGItem************//
OSGItem::OSGItem(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
{
    //setFlag(ItemHasContents);
    //setPolishOnResize(true);

    //按每秒60帧的帧率更新界面
    startTimer(1000 / 60);
    m_current_layer = new OpenglRenderer(this);
}

void OSGItem::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    update();
}

void OSGItem::addMapper(QString title,QString mappertype)
{
    _tcmd(cmd)
    cmd->method = "addMapper";
    (*cmd->request)["objectName"] = title.toStdString();
    (*cmd->request)["mapperType"] = mappertype.toStdString();
    m_current_layer->doCommand(cmd);//mapper相关的操作很多需要opengl，opengl的函数必须在opengl线程调用，利用doCommand将指令推送到opengl线程
}

void OSGItem::itemChange(ItemChange change, const ItemChangeData &value)
{
    QQuickItem::itemChange(change, value); // call base class method
    if (change == ItemChildAddedChange) {

        qDebug() << "A child item was added:" << value.item;

    } else if (change == ItemChildRemovedChange) {
        qDebug() << "A child item was removed:" << value.item;
    }
    // handle other change types as needed
}

void OSGItem::geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    qreal w = newGeometry.width();
    qreal h = newGeometry.height();

    if(m_current_layer)
    {
       // glFunc.glViewport(0, 0, w, h);
        m_current_layer->left = -1.0 ;
        m_current_layer->right = 1.0 ;
        m_current_layer->top = (float)h / w ;
        m_current_layer->bottom = -(float)h / w ;
    

        float n = -1.0f;
        float f = 1.0f;

        //透视投影(测试不充分，不知道这两句对不对)
        //m_current_layer->mViewProjectionMatrix.setToIdentity();
        //m_current_layer->mViewProjectionMatrix.frustum(m_current_layer->left, m_current_layer->right, m_current_layer->top, m_current_layer->bottom, n, f);

        //正交投影
        m_current_layer->mViewProjectionMatrix.setToIdentity();
        m_current_layer->mViewProjectionMatrix.ortho(m_current_layer->left, m_current_layer->right, m_current_layer->top, m_current_layer->bottom, n, f);//正交   


        //DEBUG_LOG(" left[%f] right[%f] top[%f] bottom[%f]",m_current_layer->left, m_current_layer->right, m_current_layer->top, m_current_layer->bottom);
    }
}



void OSGItem::ZoomIn()
{
    m_current_layer->mPolyModelMatrix.scale(11.0/10.0);
    m_current_layer->mVolumeModelMatrix.scale(11.0/10.0);
}

void OSGItem::ZoomOut()
{
    m_current_layer->mPolyModelMatrix.scale(10.0/11.0);
    m_current_layer->mVolumeModelMatrix.scale(10.0/11.0);
}




void OSGItem::AdjustifyWindows()
{
    DEBUG_LOG("method AdjustifyWindows");
    _tcmd(cmd)  
    cmd->method = "AdjustifyWindows";
    (*cmd->request)["title"] = "AdjustifyWindows";
    (*cmd->request)["mapperType"] = "OpenglVolumeMapper";
    m_current_layer->doCommand(cmd);
}

void OSGItem::MouseRightDrag(float x,float y)
{
    auto offsetx = x/(width()/(m_current_layer->right-m_current_layer->left));
    auto offsety = y/(height()/(m_current_layer->top - m_current_layer->bottom));
    
    m_current_layer->mPolyModelMatrix.translate(QVector3D(-offsetx,offsety,0.0));
    m_current_layer->mVolumeModelMatrix.translate(QVector3D(-offsetx,offsety,0.0));
}


void OSGItem::MouseLeftDrag(float x,float y)
{
    m_current_layer->mPolyModelMatrix.rotate(-y/5.0,QVector3D(1.0,0.0,0.0));
    m_current_layer->mPolyModelMatrix.rotate(-x/5.0,QVector3D(0.0,1.0,0.0));


    m_current_layer->mVolumeModelMatrix.rotate(y/5.0,QVector3D(1.0,0.0,0.0));
    m_current_layer->mVolumeModelMatrix.rotate(x/5.0,QVector3D(0.0,1.0,0.0));

}

void OSGItem::MouseLeftBtnPressed(float x, float y)
{
    DEBUG_LOG("MouseLeftBtnPressed x %f, y %f", x, y);
    _tcmd(cmd)
    cmd->method = "MouseLeftBtnPressed";
    (*cmd->request)["title"] = "MouseLeftBtnPressed";
    (*cmd->request)["mapperType"] = "OpenglVolumeMapper";
    (*cmd->request)["x"] = x;
    (*cmd->request)["y"] = y;
    m_current_layer->doCommand(cmd);
}

void OSGItem::MouseRightBtnPressed(float x, float y)
{
    DEBUG_LOG("MouseRightBtnPressed x %f, y %f", x, y);
    _tcmd(cmd)
    cmd->method = "MouseRightBtnPressed";
    (*cmd->request)["title"] = "MouseRightBtnPressed";
    (*cmd->request)["mapperType"] = "OpenglVolumeMapper";
    (*cmd->request)["x"] = x;
    (*cmd->request)["y"] = y;
    m_current_layer->doCommand(cmd);
}

void OSGItem::ImageChanged(int value)
{
    if (value <= 1)
        return;
    DEBUG_LOG("method ImageChanged %d", value);
    _tcmd(cmd)
    cmd->method = "ImageChanged";
    (*cmd->request)["title"] = "ImageChanged";
    (*cmd->request)["mapperType"] = "OpenglVolumeMapper";
    (*cmd->request)["value"] = value;
    m_current_layer->doCommand(cmd);
}

void OSGItem::Reset()
{
    _tcmd(cmd)
    cmd->method = "Reset";
    (*cmd->request)["title"] = "Reset";
    (*cmd->request)["mapperType"] = "OpenglVolumeMapper";
    m_current_layer->doCommand(cmd);
}

void OSGItem::Contrast()
{
    _tcmd(cmd)
    cmd->method = "Contrast";
    (*cmd->request)["title"] = "Contrast";
    (*cmd->request)["mapperType"] = "OpenglVolumeMapper";
    m_current_layer->doCommand(cmd);
}

void OSGItem::Drag()
{
    _tcmd(cmd)
    cmd->method = "Drag";
    (*cmd->request)["title"] = "Drag";
    (*cmd->request)["mapperType"] = "OpenglVolumeMapper";
    m_current_layer->doCommand(cmd);
}

QQuickFramebufferObject::Renderer* OSGItem::createRenderer() const
{
    return m_current_layer;
}


// yModule::RESULT OpenglRenderer::Contrast(_tsptr<yCMD> cmd)
// {
//     //for (auto mapper : mappers)
//     //{
//     //    if (mapper)
//     //    {
//     //        mapper->isContrast = true;
//     //        //mapper->Contrast();
//     //    }
//     //}
//     return yModule::SUCCESS;
// }

// yModule::RESULT OpenglRenderer::Drag(_tsptr<yCMD> cmd)
// {
//     //for (auto mapper : mappers)
//     //{
//     //    if (mapper)
//     //    {
//     //        mapper->Drag();
//     //    }
//     //}
//     return yModule::SUCCESS;
// }

// void OSGItem::clearOpacityStops()
// {
//     std::lock_guard<std::recursive_mutex> lock( m_current_layer->mtx); 
//     //m_current_layer-> color_table_changed = true;
// 	opacity_palette = _tsptr<PiecewisePalette>(new PiecewisePalette);
//     opacity_palette->setMinMax(OpenglVolumeMapper::minCTValue,OpenglVolumeMapper::maxCTValue);
//     opacity_palette->clearTable();
// 	//opacity_palette->removeAllPoints();
// }

// void OSGItem::addOpacityStop(float x,float y)
// {
//     std::lock_guard<std::recursive_mutex> lock( m_current_layer->mtx); 
//     m_current_layer->opacity_table_changed = true;
//     opacity_palette->addPoint(x, y, 0.5, 0.5);
// }


// QVariantList  OSGItem::getOpacityTable()
// {
//      float table[256];
//      opacity_palette->getOpacityTable(table,256);
//     QVariantList testba; 
//     for(int i = 0 ; i<256; i++)
//     {
//         //float apple = i%25 *0.04*0.3;
//         //testba.append(apple);
//         testba.append(table[i]);
//     }
//     return testba;
// }


// QVariantList  OSGItem::getOpacityTable()
// {
//     float table[256];
//     opacity_palette->getOpacityTable(table,256);
//     QVariantList  testba; 
//     for(int i = 0 ;i<256;i++)
//     {
//         float apple = 1.0;
//         testba.append(table[i]);
//     }
//     return testba; 
// }


// void OSGItem::clearColorStops()
// {
//     std::lock_guard<std::recursive_mutex> lock( m_current_layer->mtx); 
//     //m_current_layer-> color_table_changed = true;
// 	color_palette = _tsptr<ColorPalette>(new ColorPalette);
//     color_palette->setMinMaxValue(OpenglVolumeMapper::minCTValue,OpenglVolumeMapper::maxCTValue);
// 	color_palette->removeAllPoints();
// }


// void OSGItem::addColorStop(float pos,QColor color)
// {
//     std::lock_guard<std::recursive_mutex> lock( m_current_layer->mtx); 
//     m_current_layer->color_table_changed = true;
//     //DEBUG_LOG("%f(%f %f %f)",pos,(float)(color.red()) / 255.0, (float)(color.green()) / 255.0, (float)(color.blue()) / 255.0);
//     color_palette->addRGBPoint(pos*(OpenglVolumeMapper::maxCTValue-OpenglVolumeMapper::minCTValue)+OpenglVolumeMapper::minCTValue, (float)(color.red()) / 255.0, (float)(color.green()) / 255.0, (float)(color.blue()) / 255.0, 0.5, 1.0);//start color
// }

//  QVariantList OSGItem::getColorTable()
//  {

//     float table[256*4];
//     color_palette->getColorTable(table,256);

//     QVariantList  testba; 

//     for(int i = 0 ;i<256*4;i++)
//     {
//         testba.append(table[i]);
//     }


//     return testba;
//  }


// QVariantList  OSGItem::getHistTable()
// {
//     QVariantList  testba; 
    
//     if(OpenglVolumeMapper::data_hist)
//     {
//          for(int i = 0 ;i<OpenglVolumeMapper::hist_count;i++)
//          {
//             //testba.append(OpenglVolumeMapper::data_hist[i]);
//             testba.append(0.001*(i%20));
//          }
//     }
//     else
//     {  
//         for(int i = 0 ;i<100;i++)
//         {
//             testba.append(0.001*(i%20));
//         }
//     }
//     return testba;
// }

// MapperItem::MapperItem(QQuickItem* parent )
// {
//     if(parent ==NULL)
//     { 
//         ERROR_LOG("no parent --------------------------------"); 
//     }
//     else if(typeid(*parent) == typeid(OSGItem))
//     {
//         osg_parent = static_cast<OSGItem*>(parent);
//     }
//     else
//     {
//         _tstring apple = typeid(*parent).name();
//         ERROR_LOG(" parent typeid [%s]",apple.c_str());
//     }
// }

// MapperItem::~MapperItem()
// {
//     unRegisterYModule();
// }
//  void MapperItem::unRegisterYModule()
//  {
//     if (m_mapper)
//     {
//         DEBUG_LOG("unregister:%s", m_mapper->getInstanceName().c_str());    
//         ySingletone::get()->removeModule(m_mapper);
//         m_mapper.reset();
//     }
// }

// QVariantList MapperItem::getHistTable()
// {
//     QVariantList  testba; 
    
//     if(OpenglVolumeMapper::data_hist)
//     {
//          for(int i = 0 ;i<OpenglVolumeMapper::hist_count;i++)
//          {
//             //testba.append(OpenglVolumeMapper::data_hist[i]);
//             testba.append(0.001*(i%20));
//          }
//     }
//     else
//     {  
//         for(int i = 0 ;i<100;i++)
//         {
//             testba.append(0.001*(i%20));
//         }
//     }
//     return testba;
// }

// QVariantList  MapperItem::getColorTable()
// {
//     QVariantList result;
//      if(m_mapper&&typeid(*m_mapper)==typeid(OpenglVolumeMapper))
//      {
//          _tsptr<OpenglVolumeMapper>volume = _tdynamic_pointer_cast<OpenglVolumeMapper>(m_mapper);
//          if( volume->color_data&&volume->color_data->cpu_memory)
//          {
//             float* apple = (float*)volume->color_data->cpu_memory;
//             for(int i = 0; i<volume->color_data->dimX*4; i++)
//             {
//                 result.append(apple[i]);
//             }
//          }     
//      }   
//      return result;
// }


// QVariantList  MapperItem::getOpacityTable()
// {
//      QVariantList result;
//      if(m_mapper&&typeid(*m_mapper)==typeid(OpenglVolumeMapper))
//      {
//          _tsptr<OpenglVolumeMapper>volume = _tdynamic_pointer_cast<OpenglVolumeMapper>(m_mapper);
//          if( volume->opacity_data&&volume->opacity_data->cpu_memory)
//          {
//             float* apple = (float*)volume->opacity_data->cpu_memory;
//             for(int i = 0; i<volume->opacity_data->dimX; i++)
//             {
//                 result.append(apple[i]);
//             }
//          }     
//      }   
//      return result;
//  }


// void MapperItem::createMapper()
// {
//     //QString id = property("id").toString();

//     //QQmlProperty(this, "id");
//    // QString id ="";

//     QString id = QQmlProperty(this, "id").read().toString();//还没找到方法可以获取到id
//     // QQmlContext *context = QQmlEngine::contextForObject(this);
//     // if (context) {
//     //     id = context->contextProperty("myapple").toString(); // "myQuickItemID"
//     // }
   
//     QString objectName = property("objectName").toString();
//     QString mapperType = property("mapperType").toString();

//     if(!objectName.isEmpty())
//     {
//          QObject* p = parentItem();
//         if(p==NULL)
//         {
//             ERROR_LOG("no parent");
//         }
//         else if(QString(p->metaObject()->className())== "OSGItem")
//         {
//             osg_parent = static_cast<OSGItem*>(p);
//         }
//         else
//         {
//             ERROR_LOG("no parent [%s]",p->metaObject()->className());
//         }
//         if(osg_parent)
//         {    
//             _tcmd(cmd)
//             cmd->method = "addMapper";
//             (*cmd->request)["objectName"] = objectName.toStdString();
//             (*cmd->request)["mapperType"] = mapperType.toStdString();
//             osg_parent->m_current_layer->addMapper(cmd);//注意这里不是opengl线程

//            m_mapper = osg_parent->m_current_layer->newest_mapper;
//            if(m_mapper->title != objectName)
//            {
//                ERROR_LOG("title check failed [%s] [%s] ",objectName.toStdString().c_str(),m_mapper->title.toStdString().c_str());
//            }
//         }
//     }
// }