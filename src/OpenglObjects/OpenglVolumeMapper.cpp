#include "OpenglVolumeMapper.h"

#include <QOpenGLPixelTransferOptions>

//QOpenGLTexture* OpenglVolumeMapper::mTex3D = 0;
//short* OpenglVolumeMapper::m_g_data=0;
//float* OpenglVolumeMapper::data_hist=0;
int OpenglVolumeMapper::hist_count = 40;
short OpenglVolumeMapper::maxCTValue = 2500;
short OpenglVolumeMapper::minCTValue = 0;

OpenglVolumeMapper::OpenglVolumeMapper()
{
    qWarning() << __FUNCTION__;
    bottom = -1.0f;
    top = 2.0f;
    n = 0.5f;
    f = 100.0f;

    wc = 1200.0;
    ww = 2500;

    fScale = 256.0 / wc;
    fShift = wc - ww / 2.0;
   
    toolFlag = Image_None;
}

OpenglVolumeMapper::~OpenglVolumeMapper()
{
    //qWarning() << __FUNCTION__;
}


void OpenglVolumeMapper::setOpacityTable(float* table,int size)
{
    if(mTexOpacity)
    {
        delete mTexOpacity;
    }

    // _tsptr<JConfiger> configer(new JConfiger);
    // for(int i = 0;i <size;i++)
    // {
    //     float value = table[i];
    //     (*configer).pushBack(value);
    // }
    // _tstring apple ="D://temp//opacity.txt"; 
    // configer->save(apple,false);
    // DEBUG_LOG("%s saved",apple.c_str());
    mTexOpacity = new QOpenGLTexture(QOpenGLTexture::Target1D);
    mTexOpacity->setFormat(QOpenGLTexture::LuminanceFormat);
    mTexOpacity->setSize(size); // 纹理的长度为size像素
    mTexOpacity->allocateStorage();
    mTexOpacity->setMinificationFilter(QOpenGLTexture::Linear);
    mTexOpacity->setMagnificationFilter(QOpenGLTexture::Linear);
    mTexOpacity->setWrapMode(QOpenGLTexture::ClampToEdge);
   
    mTexOpacity->setData( QOpenGLTexture::Red, QOpenGLTexture::Float32, table);
}

void OpenglVolumeMapper::setColorTable(float* table,int size)
{
    if(mTexColor)
    {
        delete mTexColor;
    }  
    mTexColor = new QOpenGLTexture(QOpenGLTexture::Target1D);
    mTexColor->setFormat(QOpenGLTexture::RGBA32F);
    mTexColor->setSize(size); // 纹理的长度为size像素
    mTexColor->allocateStorage();
    mTexColor->setMinificationFilter(QOpenGLTexture::Linear);
    mTexColor->setMagnificationFilter(QOpenGLTexture::Linear);
    mTexColor->setWrapMode(QOpenGLTexture::ClampToEdge);
    mTexColor->setData(QOpenGLTexture::RGBA, QOpenGLTexture::Float32, table); 
}

void OpenglVolumeMapper::initTemp()
{
    qWarning() << typeid(this).name() << __FUNCTION__;
    initializeOpenGLFunctions();
    glDepthMask(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    initShader();

    // static bool bOk = true;
    // if (bOk == true)
    // {
    //     initTexture();
    //     bOk = false;
    // }

    if(mTexOpacity==NULL)
    {
        mTexOpacity = new QOpenGLTexture(QOpenGLTexture::Target1D);
        mTexOpacity->setFormat(QOpenGLTexture::LuminanceFormat);
        mTexOpacity->setSize(256); // 纹理的长度为256像素
        mTexOpacity->allocateStorage();
        mTexOpacity->setMinificationFilter(QOpenGLTexture::Linear);
        mTexOpacity->setMagnificationFilter(QOpenGLTexture::Linear);
        mTexOpacity->setWrapMode(QOpenGLTexture::ClampToEdge);
        // 上传像素数据，这里使用一个QImage作为数据源
        // QImage image("some_image.png");
        // mTexOpacity->setData(0, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, image.constBits());
        char* opacity_table = new  char[256];
        for(int i = 0 ;i<256;i++)
        {
            if(i>128)
            {
                opacity_table[i] = 50;
            }
            else
            {
                opacity_table[i] = 0;
            }
        }
        mTexOpacity->setData( QOpenGLTexture::Red, QOpenGLTexture::Int8, opacity_table);
    }
    
    
    if(mTexColor == NULL)
    {
        mTexColor = new QOpenGLTexture(QOpenGLTexture::Target1D);
        mTexColor->setFormat(QOpenGLTexture::RGBA32F);
        mTexColor->setSize(256); // 纹理的长度为256像素
        mTexColor->allocateStorage();
        mTexColor->setMinificationFilter(QOpenGLTexture::Linear);
        mTexColor->setMagnificationFilter(QOpenGLTexture::Linear);
        mTexColor->setWrapMode(QOpenGLTexture::ClampToEdge);

        float* color_table = new float[256*4];
        for(int i = 0;i < 256 ; i++)
        {
            color_table[i * 4 +0] = 1.0/256*i;
            color_table[i * 4 +1] = 1.0/256*i;
            color_table[i * 4 +2] = 1.0/256*i;
            color_table[i * 4 +3] = 1.0;
        }
        mTexColor->setData( QOpenGLTexture::RGBA, QOpenGLTexture::Float32, color_table);
    }
    
    initGeometry();
    //resize(340, 480);
    isInit = true;
    qWarning() << typeid(this).name() << __FUNCTION__<<" done" ;
}

// 这里最好也不要每次都刷，刷一次就行
void OpenglVolumeMapper::ImageChanged(int value)
{
    isImageChanged = false;
    if (value > 0)
    {
        imageIndex = 1/(float)value;
    }
}

// 这个考虑去掉
void OpenglVolumeMapper::AdjustifyWindows()
{
    isAdjustifyWindow = false;

    if (fScale > 1)
        fScale = 1;
    else
        fScale += 0.001;
    mProgram->setUniformValue(scaleHandle, fScale);
    if (fShift > 2000)
        fShift = 2000;
    else
        fShift += 10;
    mProgram->setUniformValue(shiftHandle, fShift);
    mProgram->setUniformValue(dataTypeHandle, fDataType);
}

void OpenglVolumeMapper::MouseLeftBtnPressed(float x, float y)
{
    isLeftPressed = false;
    if (toolFlag == Image_Contrast)
    {
        wc *=  (1.0 + x / 200);
        ww *= (1.0 + y / 200);
        fScale = 256.0 / wc;
        fShift = wc - ww / 2.0;
        //fScale += x/renderer->m_parent->width();

        mProgram->setUniformValue(scaleHandle, fScale);
        mProgram->setUniformValue(shiftHandle, fShift);
        mProgram->setUniformValue(dataTypeHandle, fDataType);
    }
    //toolFlag = Image_None;
}

void OpenglVolumeMapper::wnd2view(float& x, float& y,float& z)
{
    int rectx = renderer->m_parent->width();
    int recty = renderer->m_parent->height();

    float scalex = rectx / 2.0  * n;
    float scaly = recty / 2.0 * n;


    x = x / scalex;
    y = y / scaly;

   /* x = x / 10000.0;
    y = y / 10000.0;*/
    z = 0;
}


void OpenglVolumeMapper::view2wnd(float& x, float& y, float& z)
{
    int rectx = renderer->m_parent->width();
    int recty = renderer->m_parent->height();

    float scalex = rectx / 2.0 * n;
    float scaly = recty / 2.0 * n;

    
    x = x * scalex;
    y = y * scaly;

    z = 0;
}

// 窗宽窗位和拖动共用一个函数
void OpenglVolumeMapper::MouseRightBtnPressed(float  x, float y)
{
    // 需要保存旧的XY坐标么，值限制
    isRightPressed = false;
    //if (toolFlag == Image_Contrast)
    //{
    //    //float wc = ((y - x) + y) / 2.0; // ?
    //    //float ww = x;
    //    //fShift = wc - ww / 2.0;
    //    //mProgram->setUniformValue(shiftHandle, fShift);
    //    //mProgram->setUniformValue(dataTypeHandle, fDataType);
    //}
    //else 
    if (toolFlag == Image_Drag)
    {
        float view_x = x;
        float view_y = y;
        float view_z = 0;
        wnd2view(view_x, view_y, view_z);

        mViewMatrix.translate(QVector3D(view_x, view_y, view_z));
        mProgram->setUniformValue(mViewMatHandle, mViewMatrix);
    }
    //toolFlag = Image_None;
}

void OpenglVolumeMapper::Reset()
{
    toolFlag = Image_Reset;
    isReset = false;
    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(
        QVector3D(0.0f, 0.0f, 1.001f),
        QVector3D(0.0f, 0.0f, -5.0f),
        QVector3D(0.0f, 1.0f, 0.0f));

    // 纹理也需要复位
}

void OpenglVolumeMapper::Contrast()
{
    isContrast = false;
    toolFlag = Image_Contrast;
}

void OpenglVolumeMapper::Drag()
{
    isDrag = false;
    toolFlag = Image_Drag;
}

void OpenglVolumeMapper::resize(int width, int height)
{
    qWarning() << __FUNCTION__;

    //透视投影
    //mProjectionMatrix.setToIdentity();
    //mProjectionMatrix.frustum(-1.0, 1.0, bottom, top, n, f);


    //正交投影
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.ortho(-1.0, 1.0, 1.0, -1.0, -1.0, 1.0);//正交   
}

void OpenglVolumeMapper::initShader()
{
    if(mProgram)
    {
        delete mProgram;
    }
    mProgram = new QOpenGLShaderProgram;//似乎要在opengl线程创建
    //_tstring vertex =  ySingletone::get()->getConfigDir() + "Shaders/volumemapper.vsh.c";
    _tstring vertex = _tstring(":")+"/Shaders/volumemapper.vsh.c";

    if (!mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex.c_str()))
    {
        qWarning() << " add vertex shader file failed.";
        return;
    }

    //_tstring fragment = ySingletone::get()->getConfigDir() + "Shaders/volumemapper.fsh.c";
    _tstring fragment = _tstring(":")+"/Shaders/volumemapper.fsh.c";
    if (!mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment.c_str()))
    {
        qWarning() << " add fragment shader file failed.";
        return;
    }
   /* mProgram->bindAttributeLocation("qt_Vertex", 0);
    mProgram->bindAttributeLocation("texCoord", 1);*/
    mProgram->link();
    mProgram->bind();
}


void OpenglVolumeMapper::initTexture()
{
    // if (mTex3D == NULL)
    // {
    //     mTex3D = new QOpenGLTexture(QOpenGLTexture::Target3D);
    //     mTex3D->setFormat(QOpenGLTexture::LuminanceFormat);
    //     //    mTexV->setFixedSamplePositions(false);
    //     mTex3D->setMinificationFilter(QOpenGLTexture::Nearest);
    //     mTex3D->setMagnificationFilter(QOpenGLTexture::Nearest);
    //     mTex3D->setWrapMode(QOpenGLTexture::ClampToBorder);


    //     // mTex2D = new QOpenGLTexture(QOpenGLTexture::Target2D);
    //     // // //mTex2D->setFormat(QOpenGLTexture::RGBAFormat);//纹理内部的格式，就是说显存里边数据的格式
    //     // // //    mTexV->setFixedSamplePositions(false);
    //     // // mTex2D->setMinificationFilter(QOpenGLTexture::Nearest);
    //     // // mTex2D->setMagnificationFilter(QOpenGLTexture::Nearest);
    //     // // mTex2D->setWrapMode(QOpenGLTexture::ClampToBorder);

    //     // //mTex2D->create();
    //     // //mTex2D->allocateStorage(QOpenGLTexture::PixelFormat::RGBA, QOpenGLTexture::PixelType::UInt8);
    //     // QImage image("D:/Disk/git/yardbirds/20230418/config/test_texture.png");
    //     // DEBUG_LOG("[%d,%d]",image.width(),image.height());
    //     // mTex2D->setData(image);

    //     updateTextureInfo(256, 256, QOpenGLTexture::Red);
    // }   
}

void OpenglVolumeMapper::updateTextureInfo(int width, int height, int format)
{
    // int dimx = 410;
    // int dimy = 410;
    // int dimz = 264;

    // m_g_data = new  short[dimx * dimy * dimz];

    // char filename[256];

    // //sprintf(filename, "\\\\192.168.1.200\\Public\\DWData\\FL\\%d.raw", i);
    // //sprintf(filename, "D:/Disk/git/yardbirds/20230418/config/512x512x125.raw");
    // sprintf(filename, "D:/Disk/git/yardbirds/20230418/config/410x410x264.raw");

    // FILE* fHandle = fopen(filename, "rb");

    // if (fHandle == NULL)
    // {
    //     ERROR_LOG("error: reading %s", filename);
    //     return;
    // }

    // fseek(fHandle, 0, SEEK_END);
    // int file_size = ftell(fHandle);
    // fseek(fHandle, 0, SEEK_SET);
    // if (file_size < dimx * dimy * dimz*sizeof(short))
    // {
    //     ERROR_LOG("file_size error");
    //     return;
    // }


    // fread(m_g_data , file_size, 1, fHandle);
    // fclose(fHandle);

    // for(int  i = 0 ;i < dimx * dimy * dimz;i++)
    // {
    //     //m_g_data[i] += 1024;//负值传进opengl会变成0 ,假设CT的最小值是-1024

    //     m_g_data[i] += 1000;//负值传进opengl会变成0 ,假设CT的最小值是-1024

    //     //m_g_data[i] =i%1500+1000;//实验看一下可视化是否正确
    // }
   
    // mTex3D->setSize(dimx, dimy, dimz);
    // mTex3D->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::Int16);//data的格式，就是说内存里边数据的格式
    // mTex3D->setData(QOpenGLTexture::Red, QOpenGLTexture::Int16, m_g_data);
    // mTextureAlloced = true;
   
    // data_hist = new float[hist_count];
    // float hist_sum = 0;
    // float hist_width = (maxCTValue-minCTValue) / hist_count;
    // for (int i = 0; i < dimz; i++)
    // {
    //     for (int j = 0; j < dimy; j++)
    //     {
    //         for (int k = 0; k < dimx; k++)
    //         {
    //             short value = m_g_data[i * dimx * dimy + j * dimx + k] ;
    //             if (value <= minCTValue)
    //             {
    //                 //value = 0;
    //                 continue;//小于等于1000的空气太多，无视算了
    //             }
    //             else if (value >= maxCTValue)
    //             {
    //                 value = maxCTValue;
    //             }
    //             value = (value-minCTValue) / hist_width ;
    //             int index = value;
    //             if (index >= hist_count)
    //             {
    //                 index = hist_count - 1;
    //             }
    //             data_hist[index] += 1.0;
    //             hist_sum++;
    //         }
    //     }
    // }
    // DEBUG_LOG("hist_sum[%f]",hist_sum);
    // for (int i = 0; i < hist_count; i++)
    // {
    //     data_hist[i] = data_hist[i] / hist_sum ;
    // }

}



void OpenglVolumeMapper::initGeometry()
{
    //正方形
    mVertices 
       << QVector3D(-1.0, 1.0, 0.0f) 
       << QVector3D(1.0, 1.0, 0.0f) 
       << QVector3D(1.0, -1.0, 0.0f) 
       << QVector3D(-1.0, -1.0, 0.0f) 
       << QVector3D(-1.0, 1.0, 0.0f);

    mTexcoords
        << QVector2D(-1.0, 1.0) 
        << QVector2D(1.0, 1.0) 
        << QVector2D(1.0, -1.0) 
        << QVector2D( -1.0, -1.0) 
        << QVector2D( -1.0, 1.0);

    //    mVertices 
    //        << QVector3D(renderer->left, renderer->top, 0.0f) 
    //        << QVector3D(renderer->right, renderer->top, 0.0f) 
    //        << QVector3D(renderer->right, renderer->bottom, 0.0f) 
    //        << QVector3D(renderer->left, renderer->bottom, 0.0f) 
    //        << QVector3D(renderer->left,  renderer->top, 0.0f);

        
    // mTexcoords
    //     << QVector2D(renderer->left, renderer->top) 
    //     << QVector2D(renderer->right, renderer->top) 
    //     << QVector2D(renderer->right, renderer->bottom) 
    //     << QVector2D( renderer->left, renderer->bottom) 
    //     << QVector2D( renderer->left,  renderer->top);
    


   mViewMatrix.setToIdentity();
   mViewMatrix.lookAt(
       QVector3D(0.0f, 0.0f, 1.0f),
       QVector3D(0.0f, 0.0f, 0.0f),
       QVector3D(0.0f, 1.0f, 0.0f));


   //mViewMatrix.lookAt(
   //    QVector3D(0.0f, 0.0f, 1.001f), 
   //    QVector3D(0.0f, 0.0f, -5.0f), 
   //    QVector3D(0.0f, 1.0f, 0.0f));


   mModelMatrix.setToIdentity();
   mDataMatrix.setToIdentity();
   //mDataMatrix.rotate(180,QVector3D(1.0,0.0,0.0));//和poly的y轴相反，使用这个Matrix抵消
    //mDataMatrix.rotate(180,QVector3D(0.0,1.0,0.0));//和poly的y轴相反，使用这个Matrix抵消
}



void OpenglVolumeMapper::updateTextureData(/*const YUVData& data*/)
{
   
}


yModule::RESULT OpenglVolumeMapper::doSlot(_tsptr<yCMD> cmd)
{
    MACRO_doSlot(opacityUpdated);
    MACRO_doSlot(colorUpdated);
    MACRO_doSlot(imageUpdated);
    MACRO_doSlot(matrixUpdated);
}

yModule::RESULT OpenglVolumeMapper::opacityUpdated(_tsptr<yCMD>  cmd)
{
    //DEBUG_LOG("updated [%s]",title.toStdString().c_str());
    if(cmd->data.find("data")!=cmd->data.end())
    {
        _tsptr<DataObject> tempobject = cmd->data["data"];
        if(tempobject!=nullptr)
        {
            auto tempopengl = _tdynamic_pointer_cast<OpenglData>(tempobject);
            if(tempopengl!=nullptr)
            {
                opacity_data = tempopengl;
            }
        }
        return yModule::SUCCESS;    
    }

}

yModule::RESULT OpenglVolumeMapper::colorUpdated(_tsptr<yCMD>  cmd)
{
    //DEBUG_LOG("updated [%s]",title.toStdString().c_str());
    if(cmd->data.find("data")!=cmd->data.end())
    {
        _tsptr<DataObject> tempobject = cmd->data["data"];
        if(tempobject!=nullptr)
        {
            auto tempopengl = _tdynamic_pointer_cast<OpenglData>(tempobject);
            if(tempopengl!=nullptr)
            {
                color_data = tempopengl;
            }
        }
        return yModule::SUCCESS;    
    }

}

yModule::RESULT OpenglVolumeMapper::imageUpdated(_tsptr<yCMD>  cmd)
{
    //DEBUG_LOG("updated [%s]",title.toStdString().c_str());


    //DEBUG_LOG("updated [%s]",title.toStdString().c_str());


    if(cmd->data.find("data")!=cmd->data.end())
    {
        _tsptr<DataObject> tempobject = cmd->data["data"];
        if(tempobject!=nullptr)
        {
            auto tempopengl = _tdynamic_pointer_cast<OpenglData>(tempobject);
            if(tempopengl!=nullptr)
            {
                //计算hist
                if(hist)
                {
                    delete[] hist;
                }
                hist = new float[hist_count];
                memset(hist,0,sizeof(float)*hist_count);
               

                float hist_sum = 0;
                float hist_width = (maxCTValue-minCTValue) / hist_count;
                int dimx = tempopengl->dimX;
                int dimy = tempopengl->dimY;
                int dimz = tempopengl->dimZ;
                short * data =  (short* )tempopengl->cpu_memory;
                for (int i = 0; i < dimz; i++)
                {
                    for (int j = 0; j < dimy; j++)
                    {
                        for (int k = 0; k < dimz; k++)
                        {
                            short value = data[i * dimx * dimy + j * dimx + k] ;
                           
                            if (value <= minCTValue)
                            {
                                //value = 0;
                                continue;//小于等于1000的空气太多，无视算了
                            }
                            else if (value >= maxCTValue)
                            {
                                value = maxCTValue;
                            }
                            float temp = (value-minCTValue) / hist_width ;
                            int index = floor(temp);
                            if (index >= hist_count)
                            {
                                index = hist_count - 1;
                            }
                            hist[index] += 1.0;
                            hist_sum+=1.0;
                        }
                    }
                }
          
                
                for (int i = 0; i < hist_count; i++)
                {
                    hist[i]  = hist[i] / hist_sum ;
                }

                image_data = tempopengl;
            }
        }
        return yModule::SUCCESS;    
    }
}

yModule::RESULT OpenglVolumeMapper::matrixUpdated(_tsptr<yCMD>  cmd)
{
   
    if(cmd->data.find("data")!=cmd->data.end())
    {
        _tsptr<DataObject> tempobject = cmd->data["data"];
        if(tempobject!=nullptr)
        {
            auto temp = _tdynamic_pointer_cast<MatrixData>(tempobject);
            if(temp!=nullptr)
            {
                matrix_data = temp;
            }
        }
        return yModule::SUCCESS;    
    }
    return yModule::FAILED;    
}


void OpenglVolumeMapper::paint()
{
    if(image_data==NULL)
    {
        return;//这里return会导致stlmapper显示不出来
    }
    //mModelMatrix.rotate(1, QVector3D(0.0f, 1.0f, 0.0f));
    mProgram->bind();
    mDataMatHandle= mProgram->uniformLocation("u_dataMatrix");
    mModelMatHandle = mProgram->uniformLocation("u_modelMatrix");
    mViewMatHandle = mProgram->uniformLocation("u_viewMatrix");
    mProjectMatHandle = mProgram->uniformLocation("u_projectMatrix");
    mVerticesHandle = mProgram->attributeLocation("qt_Vertex");
    mNormalsHandle = mProgram->attributeLocation("qt_Normal");
    mTexCoordHandle = mProgram->attributeLocation("texCoord");
  

    // 窗宽窗位
    scaleHandle = mProgram->uniformLocation("scale");
    mProgram->setUniformValue(scaleHandle, fScale);
    shiftHandle = mProgram->uniformLocation("shift");
    mProgram->setUniformValue(shiftHandle, fShift);
    dataTypeHandle = mProgram->uniformLocation("dataType");
    mProgram->setUniformValue(dataTypeHandle, fDataType);

    float rectx = renderer->right-renderer->left;
    float recty = renderer->top-renderer->bottom;
    float tex_color_width = mProgram->uniformLocation("tex_color_width");
    mProgram->setUniformValue(tex_color_width, rectx);
    float tex_color_height = mProgram->uniformLocation("tex_color_height");
    mProgram->setUniformValue(tex_color_height, recty);


    //顶点
    mProgram->enableAttributeArray(mVerticesHandle);
    mProgram->setAttributeArray(mVerticesHandle, mVertices.constData());
    mProgram->enableAttributeArray(mNormalsHandle);
    mProgram->setAttributeArray(mNormalsHandle, mNormals.constData());

    //纹理坐标
    mProgram->enableAttributeArray(mTexCoordHandle);
    mProgram->setAttributeArray(mTexCoordHandle, mTexcoords.constData());

    // MVP矩阵
    //mProgram->setUniformValue(mModelMatHandle, mModelMatrix);
   // mProgram->setUniformValue(mViewMatHandle, mViewMatrix);
    //mProgram->setUniformValue(mProjectMatHandle, mProjectionMatrix);


    
    mProgram->setUniformValue(mDataMatHandle,  mDataMatrix);
    if(matrix_data)
    {
        mProgram->setUniformValue(mModelMatHandle,  matrix_data->matrix);
    }
    else
    {
        mProgram->setUniformValue(mModelMatHandle,  renderer->mVolumeModelMatrix);
    }
   
    mProgram->setUniformValue(mViewMatHandle,  renderer->mCamaraMatrix);
    
    mProgram->setUniformValue(mProjectMatHandle, renderer->mViewProjectionMatrix);
    

    glActiveTexture(GL_TEXTURE0);
    //mTex2D->bind();
    // DEBUG_LOG("renderer->textureHandle[%d]",renderer->textureHandle);
    glBindTexture(GL_TEXTURE_2D, renderer->Framebuffercolor);
    mProgram->setUniformValue("tex_2d", 0);

    glActiveTexture(GL_TEXTURE1);
    if(image_data)
    {
         QOpenGLTexture* mTexImage = image_data->getOpenglTexture();
        if(mTexImage)
        {
            glBindTexture(GL_TEXTURE_3D,  mTexImage->textureId());
            //DEBUG_LOG("using mTexOpacity");
        }
        else
        {
            ERROR_LOG("no mTexImage");
        }
    }
    else
    {
        //mTex3D->bind();
    }
    
    mProgram->setUniformValue("tex_3d", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, renderer->Framebufferdepth);
    mProgram->setUniformValue("tex_depth", 2);

    glActiveTexture(GL_TEXTURE3);
    if(color_data)
    {
         QOpenGLTexture* mTexColor = color_data->getOpenglTexture();
        if(mTexColor)
        {
            glBindTexture(GL_TEXTURE_1D,  mTexColor->textureId());
            //DEBUG_LOG("using mTexOpacity");
        }
        else
        {
            ERROR_LOG("no mTexOpacity");
        }
    }
    else
    {
        glBindTexture(GL_TEXTURE_1D, mTexColor->textureId());
    }
   
    mProgram->setUniformValue("tex_color", 3);

    glActiveTexture(GL_TEXTURE4);
    if(opacity_data)
    {
        QOpenGLTexture* mTexOpacity= opacity_data->getOpenglTexture();
        if(mTexOpacity)
        {
            glBindTexture(GL_TEXTURE_1D,  mTexOpacity->textureId());
            //DEBUG_LOG("using mTexOpacity");
        }
        else
        {
            ERROR_LOG("no mTexOpacity");
        }
        
    }
    else
    {
        glBindTexture(GL_TEXTURE_1D,  mTexOpacity->textureId());

        // ERROR_LOG("no mTexOpacity 2");
    }
    
    mProgram->setUniformValue("tex_opacity", 4);

   
    glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertices.size());

    // 绘制文字
    //drawString("Hello");

    mProgram->disableAttributeArray(mVerticesHandle);
    mProgram->disableAttributeArray(mTexCoordHandle);
    mProgram->release();
}
