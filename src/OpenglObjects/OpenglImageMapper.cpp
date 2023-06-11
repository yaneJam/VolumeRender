﻿#include "OpenglImageMapper.h"

#include <QOpenGLPixelTransferOptions>




//static void safeDeleteTexture(QOpenGLTexture* texture)
//{
//    if (texture)
//    {
//        if (texture->isBound())
//        {
//            texture->release();
//        }
//        if (texture->isCreated())
//        {
//            texture->destroy();
//        }
//        delete texture;
//        texture = nullptr;
//    }
//}

OpenglImageMapper::OpenglImageMapper()
{
    qWarning() << __FUNCTION__;
}

OpenglImageMapper::~OpenglImageMapper()
{
    qWarning() << __FUNCTION__;
    safeDeleteTexture(mTexY);
    safeDeleteTexture(mTexU);
    safeDeleteTexture(mTexV);
}

void OpenglImageMapper::setTempContext(_tsptr<yValue> context)
{
    title = QString::fromStdString((*context)["objectName"].asString());
 
}

void OpenglImageMapper::initTemp()
{
    qWarning() << __FUNCTION__;
    initializeOpenGLFunctions();
    glDepthMask(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    initShader();
    initTexture();
    initGeometry();
    resize(340, 480);
    isInit = true;
}
void OpenglImageMapper::resize(int width, int height)
{
    qWarning() << __FUNCTION__;
   // glViewport(0, 0, width, height);
    float bottom = -1.0f;
    float top = 1.0f;
    float n = 1.0f;
    float f = 100.0f;
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.frustum(-1.0, 1.0, bottom, top, n, f);
}
void OpenglImageMapper::initShader()
{
   _tstring vertex =  ySingletone::get()->getConfigDir() + "Shaders/vertex.vsh";
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vertex.c_str()))
    {
        qWarning() << " add vertex shader file failed.";
        return;
    }
    _tstring fragment = ySingletone::get()->getConfigDir() + "Shaders/fragment.fsh";
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fragment.c_str()))
    {
        qWarning() << " add fragment shader file failed.";
        return;
    }
    mProgram.bindAttributeLocation("qt_Vertex", 0);
    mProgram.bindAttributeLocation("texCoord", 1);
    mProgram.link();
    mProgram.bind();

}
void OpenglImageMapper::initTexture()
{
    // yuv420p
    mTexY = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexY->setFormat(QOpenGLTexture::LuminanceFormat);
    //    mTexY->setFixedSamplePositions(false);
    mTexY->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexY->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexY->setWrapMode(QOpenGLTexture::ClampToEdge);

    mTexU = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexU->setFormat(QOpenGLTexture::LuminanceFormat);
    //    mTexU->setFixedSamplePositions(false);
    mTexU->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexU->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexU->setWrapMode(QOpenGLTexture::ClampToEdge);

    mTexV = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexV->setFormat(QOpenGLTexture::LuminanceFormat);
    //    mTexV->setFixedSamplePositions(false);
    mTexV->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexV->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexV->setWrapMode(QOpenGLTexture::ClampToEdge);


    mTexRGBA = new QOpenGLTexture(QOpenGLTexture::Target2D);
    mTexRGBA->setFormat(QOpenGLTexture::LuminanceFormat);
    //    mTexY->setFixedSamplePositions(false);
    mTexRGBA->setMinificationFilter(QOpenGLTexture::Nearest);
    mTexRGBA->setMagnificationFilter(QOpenGLTexture::Nearest);
    mTexRGBA->setWrapMode(QOpenGLTexture::ClampToEdge);

}

void OpenglImageMapper::initGeometry()
{
    mVertices << QVector3D(-1, 1, 0.0f) << QVector3D(1, 1, 0.0f) << QVector3D(1, -1, 0.0f) << QVector3D(-1, -1, 0.0f);
    mTexcoords << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0) << QVector2D(0, 0);

    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 1.001f), QVector3D(0.0f, 0.0f, -5.0f), QVector3D(0.0f, 1.0f, 0.0f));
    mModelMatrix.setToIdentity();
}
void OpenglImageMapper::updateTextureInfo(int width, int height, int format)
{
    //if (format == AV_PIX_FMT_YUV420P)
    //{
    //    // yuv420p
    //    mTexY->setSize(width, height);
    //    mTexY->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);

    //    mTexU->setSize(width / 2, height / 2);
    //    mTexU->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);

    //    mTexV->setSize(width / 2, height / 2);
    //    mTexV->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);
    //}
    //else
    //{
    //    // 先按yuv444p处理
    //    mTexY->setSize(width, height);
    //    mTexY->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);

    //    mTexU->setSize(width, height);
    //    mTexU->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);

    //    mTexV->setSize(width, height);
    //    mTexV->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);
    //}

   
    BYTE* data = new BYTE[width * height * 4];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i % 50 < 25)
            {
                data[4 * (j + i * width) + 0] = 100;
                data[4 * (j + i * width) + 1] = 0;
                data[4 * (j + i * width) + 2] = 100;
                data[4 * (j + i * width) + 3] = 255;
            }
            else
            {
                data[4 * (j + i * width) + 0] = 100;
                data[4 * (j + i * width) + 1] = 0;
                data[4 * (j + i * width) + 2] = 100;
                data[4 * (j + i * width) + 3] = 255;
            }
        }
    }

    mTextureAlloced = true;
}

void OpenglImageMapper::updateTextureData(/*const YUVData& data*/)
{
   /* if (data.Y.size() <= 0)
    {
        qWarning() << "y array is empty";
        return;
    }
    if (data.U.size() <= 0)
    {
        qWarning() << "u array is empty";
        return;
    }
    if (data.V.size() <= 0)
    {
        qWarning() << "v array is empty";
        return;
    }

    QOpenGLPixelTransferOptions options;
    options.setImageHeight(data.height);
    options.setRowLength(data.yLineSize);
    mTexY->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data.Y.data(), &options);
    options.setRowLength(data.uLineSize);
    mTexU->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data.U.data(), &options);
    options.setRowLength(data.vLineSize);
    mTexV->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, data.V.data(), &options);*/

   
    QOpenGLPixelTransferOptions options;
    options.setImageHeight(height);
    options.setRowLength(width);
    mTexRGBA->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, data, &options);

}
void OpenglImageMapper::paint()
{
    glDepthMask(true);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*if (!mTextureAlloced)
    {
        return;
    }*/
    mProgram.bind();

    mModelMatHandle = mProgram.uniformLocation("u_modelMatrix");
    mViewMatHandle = mProgram.uniformLocation("u_viewMatrix");
    mProjectMatHandle = mProgram.uniformLocation("u_projectMatrix");
    mVerticesHandle = mProgram.attributeLocation("qt_Vertex");
    mTexCoordHandle = mProgram.attributeLocation("texCoord");
    //顶点
    mProgram.enableAttributeArray(mVerticesHandle);
    mProgram.setAttributeArray(mVerticesHandle, mVertices.constData());

    //纹理坐标
    mProgram.enableAttributeArray(mTexCoordHandle);
    mProgram.setAttributeArray(mTexCoordHandle, mTexcoords.constData());

    // MVP矩阵
    mProgram.setUniformValue(mModelMatHandle, mModelMatrix);
    mProgram.setUniformValue(mViewMatHandle, mViewMatrix);
    mProgram.setUniformValue(mProjectMatHandle, mProjectionMatrix);

    // pixFmt
    mProgram.setUniformValue("pixFmt", mPixFmt);

    //纹理
    // Y
    glActiveTexture(GL_TEXTURE0);
    mTexY->bind();

    // U
    glActiveTexture(GL_TEXTURE1);
    mTexU->bind();

    // V
    glActiveTexture(GL_TEXTURE2);
    mTexV->bind();

    // V
    glActiveTexture(GL_TEXTURE3);
    mTexRGBA->bind();


    mProgram.setUniformValue("tex_y", 0);
    mProgram.setUniformValue("tex_u", 1);
    mProgram.setUniformValue("tex_v", 2);
    mProgram.setUniformValue("tex_v", 3);

    glDrawArrays(GL_TRIANGLE_FAN, 0, mVertices.size());

    mProgram.disableAttributeArray(mVerticesHandle);
    mProgram.disableAttributeArray(mTexCoordHandle);
    mProgram.release();
}
