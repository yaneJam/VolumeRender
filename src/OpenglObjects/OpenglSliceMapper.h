#ifndef OPENGLSLICEMAPPER_H
#define OPENGLSLICEMAPPER_H
//#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickFramebufferObject>
#include <memory>


class OpenglSliceMapper : public OpenglMapper
{
public:
    OpenglSliceMapper();
    ~OpenglSliceMapper();
    virtual void setTempContext(_tsptr<yValue> context);


    void view2wnd(float& x, float& y, float& z);
    void wnd2view(float& x, float& y, float& z);

    void initTemp() override;
    void paint() override;
    void resize(int width, int height)override;
    void updateTextureInfo(int width, int height, int format);
    void updateTextureData(/*const YUVData& data*/);

    // void ZoomIn() override;
    // void ZoomOut() override;
    void ImageChanged(int value) override;
    void AdjustifyWindows() override;
    void MouseLeftBtnPressed(float x, float y) override;
    void MouseRightBtnPressed(float x, float y) override;
    void Reset() override;
    void Contrast() override;
    void Drag() override;
protected:
    void initTexture()override;
    void initShader()override;
    void initGeometry()override;

private:
    ImageToolFlag toolFlag;
    QOpenGLShaderProgram mProgram;
    static QOpenGLTexture* mTex3D;
    QVector<QVector3D> mNormals;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mTexcoords;
    int mModelMatHandle, mViewMatHandle, mProjectMatHandle;
    int mVerticesHandle;
    int mNormalsHandle;
    int mTexCoordHandle;
    float fImageIndex;
    float scaleHandle, shiftHandle, dataTypeHandle;
    float fScale = 0.01, fShift = 1632.0, fDataType = 0.0;
    float wc = 11005.0;
    float ww = 18765;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;
    GLint mPixFmt = 0;
    bool mTextureAlloced = false;
    int height = 512;
    int width = 512;
    BYTE* data = NULL;

    float imageIndex = 0;


    float bottom, top, n, f;
    float x = 0.0, y = 0.0;
};
#endif