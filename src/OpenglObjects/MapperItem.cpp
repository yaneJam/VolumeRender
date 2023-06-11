//#include "OSGItem.h"
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QQmlProperty>
#include <QQmlContext>

MapperItem::MapperItem(QQuickItem* parent )
{
    if(parent ==NULL)
    { 
        ERROR_LOG("no parent --------------------------------"); 
    }
    else if(typeid(*parent) == typeid(OSGItem))
    {
        osg_parent = static_cast<OSGItem*>(parent);
    }
    else
    {
        _tstring apple = typeid(*parent).name();
        ERROR_LOG(" parent typeid [%s]",apple.c_str());
    }
}

MapperItem::~MapperItem()
{
    unRegisterYModule();
}
 void MapperItem::unRegisterYModule()
 {
    if (m_mapper)
    {
        DEBUG_LOG("unregister:%s", m_mapper->getInstanceName().c_str());    
        ySingletone::get()->removeModule(m_mapper);
        m_mapper.reset();
    }
}

QVariantList MapperItem::getHistTable()
{
    QVariantList  testba; 
    if(m_mapper&&typeid(*m_mapper)==typeid(OpenglVolumeMapper))
    {
        _tsptr<OpenglVolumeMapper>volume = _tdynamic_pointer_cast<OpenglVolumeMapper>(m_mapper);
        if(volume->hist)
        {
            for(int i = 0 ;i<volume->hist_count;i++)
            {
                testba.append(volume->hist[i]);
                //testba.append(0.001*(i%20));
            }
            return testba;
        }
    }
    for(int i = 0 ;i<100;i++)
    {
     testba.append(0.001*(i%20));
    }

    return testba;

}

QVariantList  MapperItem::getColorTable()
{
    QVariantList result;
     if(m_mapper&&typeid(*m_mapper)==typeid(OpenglVolumeMapper))
     {
         _tsptr<OpenglVolumeMapper>volume = _tdynamic_pointer_cast<OpenglVolumeMapper>(m_mapper);
         if( volume->color_data&&volume->color_data->cpu_memory)
         {
            float* apple = (float*)volume->color_data->cpu_memory;
            for(int i = 0; i<volume->color_data->dimX*4; i++)
            {
                result.append(apple[i]);
            }
         }     
     }   
     return result;
}


QVariantList  MapperItem::getOpacityTable()
{
     QVariantList result;
     if(m_mapper&&typeid(*m_mapper)==typeid(OpenglVolumeMapper))
     {
         _tsptr<OpenglVolumeMapper>volume = _tdynamic_pointer_cast<OpenglVolumeMapper>(m_mapper);
         if( volume->opacity_data&&volume->opacity_data->cpu_memory)
         {
            float* apple = (float*)volume->opacity_data->cpu_memory;
            for(int i = 0; i<volume->opacity_data->dimX; i++)
            {
                result.append(apple[i]);
            }
         }     
     }   
     return result;
 }


void MapperItem::createMapper()
{
    //QString id = property("id").toString();

    //QQmlProperty(this, "id");
   // QString id ="";

    QString id = QQmlProperty(this, "id").read().toString();//还没找到方法可以获取到id
    // QQmlContext *context = QQmlEngine::contextForObject(this);
    // if (context) {
    //     id = context->contextProperty("myapple").toString(); // "myQuickItemID"
    // }
   
    QString objectName = property("objectName").toString();
    QString mapperType = property("mapperType").toString();

    if(!objectName.isEmpty())
    {
         QObject* p = parentItem();
        if(p==NULL)
        {
            ERROR_LOG("no parent");
        }
        else if(QString(p->metaObject()->className())== "OSGItem")
        {
            osg_parent = static_cast<OSGItem*>(p);
        }
        else
        {
            ERROR_LOG("no parent [%s]",p->metaObject()->className());
        }
        if(osg_parent)
        {    
            _tcmd(cmd)
            cmd->method = "addMapper";
            (*cmd->request)["objectName"] = objectName.toStdString();
            (*cmd->request)["mapperType"] = mapperType.toStdString();
            osg_parent->m_current_layer->addMapper(cmd);//注意这里不是opengl线程

           m_mapper = osg_parent->m_current_layer->newest_mapper;
           if(m_mapper->title != objectName)
           {
               ERROR_LOG("title check failed [%s] [%s] ",objectName.toStdString().c_str(),m_mapper->title.toStdString().c_str());
           }
        }
    }
}