#ifndef OPENGLPOLYMAPPER_H
#define OPENGLPOLYMAPPER_H

//#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickFramebufferObject>
#include <memory>


class OpenglPolyMapper : public OpenglMapper
{
public:
    OpenglPolyMapper();
    ~OpenglPolyMapper();
    //virtual void setTempContext(_tsptr<yValue> context);

    void initTemp() override;
    void paint() override;
    void resize(int width, int height)override;
    void updateTextureInfo(int width, int height, int format);
    void updateTextureData(/*const YUVData& data*/);


    RESULT doSlot(_tsptr<yCMD> cmd)override;
    RESULT matrixUpdated(_tsptr<yCMD>  cmd);

    RESULT imageUpdated(_tsptr<yCMD>  cmd);


protected:
    void initTexture()override;
    void initShader()override;
    void initGeometry()override;

private:
    _tsptr<MatrixData> matrix_data;
    _tsptr<PolyData> poly_data;
    
    QOpenGLShaderProgram* mProgram = NULL;
   

    QVector<QVector3D> mNormals;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mTexcoords;
    int mModelMatHandle, mViewMatHandle, mProjectMatHandle;
    int mVerticesHandle;
    int mNormalsHandle;
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