#ifndef OPENGLIMAGEMAPPER_H
#define OPENGLIMAGEMAPPER_H
//#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickFramebufferObject>
#include <memory>

//#include "TaoDecoder.h"

class OpenglImageMapper : public OpenglMapper
{
public:
    OpenglImageMapper();
    ~OpenglImageMapper();
    virtual void setTempContext(_tsptr<yValue> context);

    void initTemp() override;
    void paint() override;
    void resize(int width, int height)override;
    void updateTextureInfo(int width, int height, int format);
    void updateTextureData(/*const YUVData& data*/);


protected:
    void initTexture()override;
    void initShader()override;
    void initGeometry()override;

private:

    QOpenGLShaderProgram mProgram;
    QOpenGLTexture* mTexY = nullptr;
    QOpenGLTexture* mTexU = nullptr;
    QOpenGLTexture* mTexV = nullptr;

    QOpenGLTexture* mTexRGBA = nullptr;

    QVector<QVector3D> mVertices;
    QVector<QVector2D> mTexcoords;
    int mModelMatHandle, mViewMatHandle, mProjectMatHandle;
    int mVerticesHandle;
    int mTexCoordHandle;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;
    GLint mPixFmt = 0;
    bool mTextureAlloced = false;
    int height = 512;
    int width = 512;
    BYTE* data = NULL;
};
#endif