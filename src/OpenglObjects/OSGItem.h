#ifndef OSGITEM_H
#define OSGITEM_H
//#pragma once
#include <QQuickFramebufferObject>
#include <QQuickItem>
#include <memory>
#include <QOpenGLExtraFunctions>
class OSGItem;
class OpenglRenderer : public QQuickFramebufferObject::Renderer//, public QOpenGLFunctions
{
public:
    OpenglRenderer( OSGItem* parent);

    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
    void synchronize(QQuickFramebufferObject*) override;

    void forwardCommand();
    void doCommand(_tsptr<yCMD> cmd, bool isTemp = false);//

    void updatePalette();
    std::recursive_mutex mtx;
    bool color_table_changed = false;
    bool opacity_table_changed = false;

    bool createAndBindFrameBuffer();
    void resetFrameBuffer();
    OSGItem* m_parent = nullptr;
    QQuickWindow* m_window = nullptr;

    QMatrix4x4 mViewProjectionMatrix;//View坐标系（left是-1.0,right是1.0）
    QMatrix4x4 mCamaraMatrix;//不知道这两个Matrix能不能合成一个Matrix来使用，以后再看
    QMatrix4x4 mPolyModelMatrix;

    QMatrix4x4 mVolumeModelMatrix;


    qreal left = -1.0;
    qreal right = 1.0;
    qreal top = 1.0;
    qreal bottom = -1.0;
    //GLuint textureHandle=0;
    GLuint frame_buffer_id = 0;//opengl帧缓存
	GLuint Framebuffercolor = 0;
	GLuint Framebufferdepth = 0;
    QOpenGLTexture *depthTexture=0;
    QOpenGLTexture *colorTexture=0;

    QList<_tsptr<yCMD>> cmds;
    int cmd_index = 0;
    int new_cmd = 0;
    _tsptr<OpenglMapper> newest_mapper;//最近add 的mapper
    QMap<QString, _tsptr<OpenglMapper>>mappers;
    yModule::RESULT addMapper(_tsptr<yCMD> cmd);
    yModule::RESULT ImageChanged(_tsptr<yCMD> cmd);
    // yModule::RESULT ZoomIn(_tsptr<yCMD> cmd);
    // yModule::RESULT ZoomOut(_tsptr<yCMD> cmd);
    yModule::RESULT AdjustifyWindows(_tsptr<yCMD> cmd);
    yModule::RESULT MouseLeftBtnPressed(_tsptr<yCMD> cmd);
    yModule::RESULT MouseRightBtnPressed(_tsptr<yCMD> cmd);
    // yModule::RESULT Contrast(_tsptr<yCMD> cmd);
    // yModule::RESULT Drag(_tsptr<yCMD> cmd);

private:
    GLint oldFbo = 0;
    int hasVolume = false;;
    int sliderValue = 0;
    float x = 0.0, y = 0.0;
    //QOpenGLFunctions glFunc;
    QOpenGLExtraFunctions glFunc;//OpenGLFunctions没有glFramebufferTexture
    bool inited = false;

    

};


class OSGItem : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    OSGItem(QQuickItem* parent = nullptr);
    void timerEvent(QTimerEvent* event) override;

    void geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry) override;
    void itemChange(ItemChange change, const ItemChangeData &value) override;

public slots:
    void addMapper(QString title, QString mappertype);

    void ZoomIn();
    void ZoomOut();
    void ImageChanged(int value);
    void AdjustifyWindows();
    void MouseLeftBtnPressed(float x, float y);
    void MouseRightBtnPressed(float x, float y);

    void MouseLeftDrag(float x,float y);
    void MouseRightDrag(float x,float y);
    void Reset();
    void Contrast();
    void Drag();

   // void clearColorStops();
    //void addColorStop(float pos,QColor color);
   
   //void clearOpacityStops();
   //void addOpacityStop(float x,float y);
   //QVariantList  getOpacityTable();
   //QVariantList  getColorTable();
  // QVariantList  getHistTable();

public:
    Renderer* createRenderer() const override;
    OpenglRenderer* m_current_layer = NULL;
    int  my_banana = -1;
    _tsptr<ColorPalette> color_palette;
     _tsptr<PiecewisePalette> opacity_palette;
 
};
// class MapperItem:public QQuickItem
// {
//     Q_OBJECT
// public:
//     MapperItem(QQuickItem* parent = nullptr);
//     ~MapperItem();
// public slots:
//     void createMapper();
//     QVariantList  getOpacityTable();
//     QVariantList  getColorTable();
//     QVariantList getHistTable();
//     void unRegisterYModule();
    
// protected:
//     OSGItem* osg_parent;
//     _tsptr<OpenglMapper> m_mapper;
// };

#endif //OSGITEM_H