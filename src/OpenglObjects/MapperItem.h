#ifndef MAPPERITEM_H
#define MAPPERITEM_H
class OSGItem;
class MapperItem:public QQuickItem
{
    Q_OBJECT
public:
    MapperItem(QQuickItem* parent = nullptr);
    ~MapperItem();
public slots:
    void createMapper();
    QVariantList  getOpacityTable();
    QVariantList  getColorTable();
    QVariantList getHistTable();
    void unRegisterYModule();
    
protected:
    OSGItem* osg_parent;
    _tsptr<OpenglMapper> m_mapper;
};

#endif //MAPPERITEM_H