#include "OpenglSliceMapper.h"

#include <QOpenGLPixelTransferOptions>

QOpenGLTexture* OpenglSliceMapper::mTex3D = 0;
OpenglSliceMapper::OpenglSliceMapper()
{
    qWarning() << __FUNCTION__;
    bottom = -1.0f;
    top = 2.0f;
    n = 0.5f;
    f = 100.0f;
    toolFlag = Image_None;
}

OpenglSliceMapper::~OpenglSliceMapper()
{
    qWarning() << __FUNCTION__;
 
}

void OpenglSliceMapper::setTempContext(_tsptr<yValue> context)
{
    title = QString::fromStdString((*context)["objectName"].asString());
}

void OpenglSliceMapper::initTemp()
{
    qWarning() << __FUNCTION__;
    initializeOpenGLFunctions();
    glDepthMask(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    initShader();

    static bool bOk = true;
    if (bOk == true)
    {
        initTexture();
        bOk = false;
    }
    initGeometry();
    resize(340, 480);
    isInit = true;
}


// void OpenglSliceMapper::ZoomIn() // 放大
// {
//     isZoomOut = false;
//     //bottom = -1.0f;
//     // 如果大于限制值时，需要限定等于限制值
//     if (isZoomIn)
//     {
//         //top += 1.0f;
//         //bottom += 1.0f; // 位置和图片大小上下移动和小幅度放大缩小
//         n += 0.1f; // 图像整体放大缩小，加减不能超过1，会消失

//         if (n > 1.0)
//             n = 1.0;
//         DEBUG_LOG("%f", n);
//         //f += 100.0f; // 没啥反应
//         isZoomIn = false;
//     }
//     //n = 1.0f;
//     //f = 100.0f;
//     mProjectionMatrix.setToIdentity();
//     mProjectionMatrix.frustum(-1.0, 1.0, bottom, top, n, f);
// }

// void OpenglSliceMapper::ZoomOut() // 缩小
// {
//     isZoomIn = false;
//     if (isZoomOut)
//     {
//         //top -= 1.0f; // 上下位置和图片上下距离的改变
//         //bottom -= 1.0f;
//         n -= 0.1f;
//         DEBUG_LOG("%f", n);

//         if (n < 0.1)
//             n = 0.1;
//         //f -= 100.0f;
//         isZoomOut = false;
//     }
//     //bottom = -1.0f;
//     //n = 1.0f;
//     //f = 100.0f;
//     mProjectionMatrix.setToIdentity();
//     mProjectionMatrix.frustum(-1.0, 1.0, bottom, top, n, f);
// }


// 这里最好也不要每次都刷，刷一次就行
void OpenglSliceMapper::ImageChanged(int value)
{
    isImageChanged = false;
    if (value > 0)
    {
        imageIndex = 1/(float)value;
    }
}

// 这个考虑去掉
void OpenglSliceMapper::AdjustifyWindows()
{
    isAdjustifyWindow = false;

    if (fScale > 1)
        fScale = 1;
    else
        fScale += 0.001;
    mProgram.setUniformValue(scaleHandle, fScale);
    if (fShift > 2000)
        fShift = 2000;
    else
        fShift += 10;
    mProgram.setUniformValue(shiftHandle, fShift);
    mProgram.setUniformValue(dataTypeHandle, fDataType);
}

void OpenglSliceMapper::MouseLeftBtnPressed(float x, float y)
{
    isLeftPressed = false;
    if (toolFlag == Image_Contrast)
    {
        wc *=  (1.0 + x / 200);
        ww *= (1.0 + y / 200);
        fScale = 256.0 / wc;
        fShift = wc - ww / 2.0;
        //fScale += x/renderer->m_parent->width();

        mProgram.setUniformValue(scaleHandle, fScale);
        mProgram.setUniformValue(shiftHandle, fShift);
        mProgram.setUniformValue(dataTypeHandle, fDataType);
    }
    //toolFlag = Image_None;
}

void OpenglSliceMapper::wnd2view(float& x, float& y,float& z)
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


void OpenglSliceMapper::view2wnd(float& x, float& y, float& z)
{
    int rectx = renderer->m_parent->width();
    int recty = renderer->m_parent->height();

    float scalex = rectx / 2.0 * n;
    float scaly = recty / 2.0 * n;

    
    x = x * scalex;
    y = y * scaly;

   /* x = x / 10000.0;
    y = y / 10000.0;*/
    z = 0;
}

// 窗宽窗位和拖动共用一个函数
void OpenglSliceMapper::MouseRightBtnPressed(float  x, float y)
{
    // 需要保存旧的XY坐标么，值限制
    isRightPressed = false;
    //if (toolFlag == Image_Contrast)
    //{
    //    //float wc = ((y - x) + y) / 2.0; // ?
    //    //float ww = x;
    //    //fShift = wc - ww / 2.0;
    //    //mProgram.setUniformValue(shiftHandle, fShift);
    //    //mProgram.setUniformValue(dataTypeHandle, fDataType);
    //}
    //else 
    if (toolFlag == Image_Drag)
    {
        float view_x = x;
        float view_y = y;
        float view_z = 0;
        wnd2view(view_x, view_y, view_z);

        mViewMatrix.translate(QVector3D(view_x, view_y, view_z));
        mProgram.setUniformValue(mViewMatHandle, mViewMatrix);
    }
    //toolFlag = Image_None;
}

void OpenglSliceMapper::Reset()
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

void OpenglSliceMapper::Contrast()
{
    isContrast = false;
    toolFlag = Image_Contrast;
}

void OpenglSliceMapper::Drag()
{
    isDrag = false;
    toolFlag = Image_Drag;
}

void OpenglSliceMapper::resize(int width, int height)
{
    qWarning() << __FUNCTION__;

    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.frustum(-1.0, 1.0, bottom, top, n, f);
}

void OpenglSliceMapper::initShader()
{
   //_tstring vertex =  ySingletone::get()->getConfigDir() + "Shaders/slicemapper.vsh.c";
   _tstring vertex = _tstring(":")+"/Shaders/slicemapper.ssh.c";

    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vertex.c_str()))
    {
        qWarning() << " add vertex shader file failed.";
        return;
    }
    //_tstring fragment = ySingletone::get()->getConfigDir() + "Shaders/slicemapper.fsh.c";
    _tstring fragment = _tstring(":")+"/Shaders/slicemapper.fsh.c";
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fragment.c_str()))
    {
        qWarning() << " add fragment shader file failed.";
        return;
    }
   /* mProgram.bindAttributeLocation("qt_Vertex", 0);
    mProgram.bindAttributeLocation("texCoord", 1);*/
    mProgram.link();
    mProgram.bind();
}

void OpenglSliceMapper::initTexture()
{
    if (mTex3D == NULL)
    {
        mTex3D = new QOpenGLTexture(QOpenGLTexture::Target3D);
        mTex3D->setFormat(QOpenGLTexture::LuminanceFormat);
        //    mTexV->setFixedSamplePositions(false);
        mTex3D->setMinificationFilter(QOpenGLTexture::Nearest);
        mTex3D->setMagnificationFilter(QOpenGLTexture::Nearest);
        mTex3D->setWrapMode(QOpenGLTexture::ClampToBorder);

        updateTextureInfo(256, 256, QOpenGLTexture::Red);
    }
   
}

void OpenglSliceMapper::initGeometry()
{
    //读取stl文件

    //STLReader reader;
    //reader.LoadFile((ySingletone::get()->getConfigDir() + "STl/sittingcat.stl").c_str());
    //mVertices = reader.mVertices;
    //mNormals = reader.mNormals;
    //正方形
   mVertices 
       << QVector3D(-0.5, 0.5, 0.0f) 
       << QVector3D(0.5, 0.5, 0.0f) 
       << QVector3D(0.5, -0.5, 0.0f) 
       << QVector3D(-0.5, -0.5, 0.0f) 
       << QVector3D(-0.5, 0.5, 0.0f);
   mTexcoords
       << QVector2D(0, 1.0) 
       << QVector2D(1.0, 1.0) 
       << QVector2D(1.0, 0) 
       << QVector2D(0, 0) 
       << QVector2D(0, 1.0);
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
}


void OpenglSliceMapper::updateTextureInfo(int width, int height, int format)
{
  
    int dimx = 1280;
    int dimy = 1024;
    int dimz = 10;

    unsigned short* data = new unsigned short[dimx * dimy * dimz];
    

    for (int i = 0; i < dimz; i++)
    {
        char filename[256];
        sprintf(filename, "\\\\192.168.1.200\\Public\\DWData\\FL\\%d.raw", i);
        FILE* fHandle = fopen(filename, "rb");

        if (fHandle == NULL)
        {
            ERROR_LOG("error: reading %s", filename);
            break;
        }

        fseek(fHandle, 0, SEEK_END);
        int file_size = ftell(fHandle);
        fseek(fHandle, 0, SEEK_SET);
        if (file_size < dimx * dimy * sizeof(unsigned short))
        {
            ERROR_LOG("file_size error");
            break;
        }


        fread(data + i * (dimx * dimy), file_size, 1, fHandle);
        fclose(fHandle);
       /* for (int j = 0; j < dimy; j++)
        {
            for (int k = 0; k < dimx; k++)
            {

                data[i * dimy * dimx + j * dimx + k] = k*10;
            }
        }*/
    }
   /* QOpenGLPixelTransferOptions options;
    options.setImageHeight(256);
    options.setRowLength(256 * 256 * 256);
    mTex3D->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data, &options);*/

    mTex3D->setSize(dimx, dimy, dimz);
    mTex3D->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt16);
    mTex3D->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt16, data);
    mTextureAlloced = true;
}

void OpenglSliceMapper::updateTextureData(/*const YUVData& data*/)
{
   
}

void OpenglSliceMapper::paint()
{
    //mViewMatrix.rotate(1, QVector3D(0.0f, 1.0f, 0.0f));
    mProgram.bind();

    mModelMatHandle = mProgram.uniformLocation("u_modelMatrix");
    mViewMatHandle = mProgram.uniformLocation("u_viewMatrix");
    mProjectMatHandle = mProgram.uniformLocation("u_projectMatrix");
    mVerticesHandle = mProgram.attributeLocation("qt_Vertex");
    mNormalsHandle = mProgram.attributeLocation("qt_Normal");
    mTexCoordHandle = mProgram.attributeLocation("texCoord");
    fImageIndex = mProgram.uniformLocation("tmpValue");

    // 窗宽窗位
    scaleHandle = mProgram.uniformLocation("scale");
    mProgram.setUniformValue(scaleHandle, fScale);
    shiftHandle = mProgram.uniformLocation("shift");
    mProgram.setUniformValue(shiftHandle, fShift);
    dataTypeHandle = mProgram.uniformLocation("dataType");
    mProgram.setUniformValue(dataTypeHandle, fDataType);

    //顶点
    mProgram.enableAttributeArray(mVerticesHandle);
    mProgram.setAttributeArray(mVerticesHandle, mVertices.constData());
    mProgram.enableAttributeArray(mNormalsHandle);
    mProgram.setAttributeArray(mNormalsHandle, mNormals.constData());

    //纹理坐标
    mProgram.enableAttributeArray(mTexCoordHandle);
    mProgram.setAttributeArray(mTexCoordHandle, mTexcoords.constData());

    // MVP矩阵
    mProgram.setUniformValue(mModelMatHandle, mModelMatrix);
    mProgram.setUniformValue(mViewMatHandle, mViewMatrix);
    mProgram.setUniformValue(mProjectMatHandle, mProjectionMatrix);

    mProgram.setUniformValue(fImageIndex, imageIndex);

    glActiveTexture(GL_TEXTURE0);
    mTex3D->bind();
    mProgram.setUniformValue("tex_3d", 0);


    glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertices.size());

    // 绘制文字

    //drawString("Hello");

    mProgram.disableAttributeArray(mVerticesHandle);
    mProgram.disableAttributeArray(mTexCoordHandle);
    mProgram.release();
}
