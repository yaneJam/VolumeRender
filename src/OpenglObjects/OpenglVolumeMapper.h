#ifndef OPENGLVOLUMEMAPPER_H
#define OPENGLVOLUMEMAPPER_H
//#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickFramebufferObject>
#include <memory>


class OpenglVolumeMapper : public OpenglMapper
{
public:
    OpenglVolumeMapper();
    ~OpenglVolumeMapper();


    RESULT doSlot(_tsptr<yCMD> cmd)override;
	RESULT opacityUpdated(_tsptr<yCMD>  cmd);
    RESULT colorUpdated(_tsptr<yCMD>  cmd);
    RESULT imageUpdated(_tsptr<yCMD>  cmd);
    RESULT matrixUpdated(_tsptr<yCMD>  cmd);
    
    //virtual void setTempContext(_tsptr<yValue> context)override;

    //暂时用static
    //static QOpenGLTexture* mTex3D;
    //static short* m_g_data;
    //static float* data_hist;
    static int hist_count;
    static short maxCTValue ;
    static short minCTValue;


    _tsptr<OpenglData> opacity_data;
    _tsptr<OpenglData> color_data;
    _tsptr<MatrixData> matrix_data;
    _tsptr<OpenglData> image_data;
    float* hist=NULL;
    
    void view2wnd(float& x, float& y, float& z);
    void wnd2view(float& x, float& y, float& z);

    void initTemp() override;
    void paint() override;
    void resize(int width, int height)override;
    void updateTextureInfo(int width, int height, int format);
    void updateTextureData(/*const YUVData& data*/);

    void setColorTable(float* table,int size);
    void setOpacityTable(float* table,int size);
    

    void ImageChanged(int value) override;
    void AdjustifyWindows() override;
    void MouseLeftBtnPressed(float x, float y) override;
    void MouseRightBtnPressed(float x, float y) override;
    void Reset() override;
    void Contrast() override;
    void Drag() override;
protected:
    void initShader()override;
    void initGeometry()override;

private:
    ImageToolFlag toolFlag;
    QOpenGLShaderProgram* mProgram = NULL;
    
    //static QOpenGLTexture* mTex2D;

    QOpenGLTexture* mTexColor = NULL;
    QOpenGLTexture* mTexOpacity= NULL;;


   

    QVector<QVector3D> mNormals;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mTexcoords;
    int mModelMatHandle, mViewMatHandle, mProjectMatHandle;
    int mDataMatHandle;
    int mVerticesHandle;
    int mNormalsHandle;
    int mTexCoordHandle;
    float fImageIndex;
    float scaleHandle, shiftHandle, dataTypeHandle;
    float fScale = 0.01, fShift = 1632.0, fDataType = 1.0;
    float wc = 11005.0;
    float ww = 18765;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;

    QMatrix4x4 mDataMatrix;
    
    GLint mPixFmt = 0;
    bool mTextureAlloced = false;
    int height = 512;
    int width = 512;
    //BYTE* data = NULL;

    float imageIndex = 0;


    float bottom, top, n, f;
    float x = 0.0, y = 0.0;

    
    
};
#endif