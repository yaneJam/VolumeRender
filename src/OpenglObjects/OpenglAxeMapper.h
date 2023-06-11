#ifndef OPENGLAXEMAPPER_H
#define OPENGLAXEMAPPER_H

//#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickFramebufferObject>
#include <memory>


class OpenglAxeMapper : public OpenglMapper
{
public:
    OpenglAxeMapper();
    ~OpenglAxeMapper();
    virtual void setTempContext(_tsptr<yValue> context);

    void initTemp() override;
    void paint() override;
   // void resize(int width, int height)override;
    void updateTextureInfo(int width, int height, int format);
    void updateTextureData(/*const YUVData& data*/);


protected:
    void initTexture()override;
    void initShader()override;
    void initGeometry()override;

private:

    QOpenGLShaderProgram mProgram;
   


    float lastLeft = -1.0;
    float lastBottom = -1.0;
    QVector<QVector3D> mNormals;
    QVector<QVector3D> mVertices;

    QVector<QVector2D> mTexcoords;
    int mModelMatHandle, mViewMatHandle, mProjectMatHandle;
    int mVerticesHandle;
    int mNormalsHandle;
    int mTexCoordHandle;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    //QMatrix4x4 mProjectionMatrix;
    GLint mPixFmt = 0;
    bool mTextureAlloced = false;
    int height = 512;
    int width = 512;
    BYTE* data = NULL;
};

#endif