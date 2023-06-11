#ifndef OPENGLMAPPER_H
#define OPENGLMAPPER_H
//#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickFramebufferObject>
#include <memory>

class OpenglRenderer;
void safeDeleteTexture(QOpenGLTexture* texture);
class OpenglMapper : public QOpenGLFunctions,public yModule
{
public:
    OpenglMapper();
    ~OpenglMapper();
    enum ImageToolFlag { Image_Drag, Image_Reset, Image_Contrast, Image_None};
    virtual void setTempContext(_tsptr<yValue> context);

    virtual void initTemp();
    virtual void paint();
    virtual void resize(int width, int height);
    //void updateTextureInfo(int width, int height, int format);
   // void updateTextureData(const YUVData& data);

    virtual void ImageChanged(int value) {}
    virtual void AdjustifyWindows() {}
    virtual void MouseLeftBtnPressed(float x, float y) {}
    virtual void MouseRightBtnPressed(float x, float y) {}
    virtual void Reset() {}
    virtual void Contrast() {}
    virtual void Drag() {}
    bool isInit = false;
    // bool isZoomIn = false;
    // bool isZoomOut = false;
    bool isImageChanged = false;
    bool isAdjustifyWindow = false;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isReset = false;
    bool isContrast = false;
    bool isDrag = false;

    OpenglRenderer* renderer = NULL;
    QString title;
protected:
    virtual void initTexture();
    virtual void initShader();
    virtual void initGeometry();

private:
    QOpenGLShaderProgram mProgram;
    
};

#endif