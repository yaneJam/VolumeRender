#include "OpenglAxeMapper.h"

#include <QOpenGLPixelTransferOptions>

OpenglAxeMapper::OpenglAxeMapper()
{
    qWarning() << __FUNCTION__;
}

OpenglAxeMapper::~OpenglAxeMapper()
{
    qWarning() << __FUNCTION__;
 
}

void OpenglAxeMapper::setTempContext(_tsptr<yValue> context)
{
    title = QString::fromStdString((*context)["objectName"].asString());
 
}

void OpenglAxeMapper::initTemp()
{
    qWarning() << __FUNCTION__;
    initializeOpenGLFunctions();
    glDepthMask(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    initShader();
    initTexture();
    initGeometry();
    //resize(480, 480);
    isInit = true;
}


//void OpenglAxeMapper::resize(int width, int height)
//{
//   // qWarning() << __FUNCTION__;
//
//     qreal left = -1.0*2.0;
//     qreal right = 1.0 * 2.0;
//    qreal top = (float)height / width * 2.0;
//    qreal bottom = -(float)height / width * 2.0;
//    //DEBUG_LOG("width %f height %f", right, top);
//    float n = -5.0f;
//    float f = 5.0f;
//    mProjectionMatrix.setToIdentity();
//    mProjectionMatrix.ortho(left, right, top, bottom, n, f);//正交
//    //mProjectionMatrix.frustum(-1.0, 1.0, bottom, top, n, f);//透视
//}

void OpenglAxeMapper::initShader()
{
   _tstring vertex =  ySingletone::get()->getConfigDir() + "Shaders/polymapper.vsh.c";
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vertex.c_str()))
    {
        qWarning() << " add vertex shader file failed.";
        return;
    }
    _tstring fragment = ySingletone::get()->getConfigDir() + "Shaders/polymapper.fsh.c";
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
void OpenglAxeMapper::initTexture()
{
}

void OpenglAxeMapper::initGeometry()
{
    //读取stl文件
    //STLReader reader;
    //reader.LoadFile((ySingletone::get()->getConfigDir() + "STl/sittingcat.stl").c_str());
    //mVertices = reader.mVertices;
    //mNormals = reader.mNormals;

    mVertices.clear();
  
    int step = (renderer->top - renderer->bottom)*0.5 / 0.1 ;
    for (int i = 0; i <= step; i++) {
        // 计算刻度的位置
        float y = 0.5*renderer->bottom + i * 0.1f ;
        //竖向尺的刻度
        mVertices << QVector3D(0.9f * renderer->left, y, 0.0f);
        mVertices << QVector3D(0.85f * renderer->left, y, 0.0f);
    }
    mVertices  << QVector3D(0.9f * renderer->left, 0.5f * renderer->bottom+step*0.1, 0.0f)
        << QVector3D(0.9f * renderer->left, 0.5f * renderer->bottom, 0.0f);//竖向尺


    step = (renderer->right - renderer->left) * 0.5 / 0.1;
    for (int i = 0; i <= step; i++) {
        // 计算刻度的位置
        float x = 0.5f * renderer->left + i * 0.1;
        //横向尺的刻度
        mVertices << QVector3D(x, 0.85 * renderer->bottom , 0.0f);
        mVertices << QVector3D(x, 0.9 * renderer->bottom, 0.0f);
    }

    mVertices << QVector3D(0.5f * renderer->left, 0.9f * renderer->bottom, 0.0f)
        << QVector3D(0.5f * renderer->left + step * 0.1, 0.9f * renderer->bottom, 0.0f);//横向尺


    mNormals << QVector3D(-1, 1, 0.0f) << QVector3D(1, 1, 0.0f) << QVector3D(1, -1, 0.0f) << QVector3D(-1, -1, 0.0f) << QVector3D(-1, 1, 0.0f);
    mTexcoords << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0) << QVector2D(0, 0);
    mViewMatrix.setToIdentity();
    mModelMatrix.setToIdentity();
}


void OpenglAxeMapper::updateTextureInfo(int width, int height, int format)
{
    
    BYTE* data = new BYTE[width * height * 4];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i % 50 < 25)
            {
                data[4 * (j + i * width) + 0] = 100;
                data[4 * (j + i * width) + 1] = 100;
                data[4 * (j + i * width) + 2] = 100;
                data[4 * (j + i * width) + 3] = 255;
            }
            else
            {
                data[4 * (j + i * width) + 0] = 100;
                data[4 * (j + i * width) + 1] = 100;
                data[4 * (j + i * width) + 2] = 100;
                data[4 * (j + i * width) + 3] = 255;
            }
        }
    }

    mTextureAlloced = true;
}

void OpenglAxeMapper::updateTextureData(/*const YUVData& data*/)
{
   

}
void OpenglAxeMapper::paint()
{
    // mViewMatrix.rotate(1, QVector3D(0.0f, 1.0f, 0.0f));

     /*if (!mTextureAlloced)
     {
         return;
     }*/

    if (lastLeft != renderer->left || lastBottom != renderer->bottom)
    {
        lastLeft = renderer->left;
        lastBottom = renderer->bottom;
        initGeometry();//窗口变了，重置顶点
    }
    mProgram.bind();

    mModelMatHandle = mProgram.uniformLocation("u_modelMatrix");
    mViewMatHandle = mProgram.uniformLocation("u_viewMatrix");
    mProjectMatHandle = mProgram.uniformLocation("u_projectMatrix");
    mVerticesHandle = mProgram.attributeLocation("qt_Vertex");
    mNormalsHandle = mProgram.attributeLocation("qt_Normal");
    mTexCoordHandle = mProgram.attributeLocation("texCoord");
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
    mProgram.setUniformValue(mProjectMatHandle, renderer->mViewProjectionMatrix);

    glDrawArrays(GL_LINES, 0, mVertices.size());
    mProgram.disableAttributeArray(mVerticesHandle);
    mProgram.disableAttributeArray(mTexCoordHandle);
    mProgram.release();
}

