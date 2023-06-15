#include "OpenglPolyMapper.h"
#include <QOpenGLPixelTransferOptions>

OpenglPolyMapper::OpenglPolyMapper()
{
    qWarning() << __FUNCTION__;
}

OpenglPolyMapper::~OpenglPolyMapper()
{
    qWarning() << __FUNCTION__;
}


yModule::RESULT OpenglPolyMapper::doSlot(_tsptr<yCMD> cmd)
{
    MACRO_doSlot(matrixUpdated);
    MACRO_doSlot(imageUpdated);
}

yModule::RESULT OpenglPolyMapper::imageUpdated(_tsptr<yCMD>  cmd)
{    
    if(cmd->data.find("data")!=cmd->data.end())
    {
        _tsptr<DataObject> tempobject = cmd->data["data"];
        if(tempobject!=nullptr)
        {
            auto temp = _tdynamic_pointer_cast<PolyData>(tempobject);
            if(temp!=nullptr)
            {
                DEBUG_LOG("im the motherfucking star boy");
                poly_data = temp;
            }
        }
        return yModule::SUCCESS;    
    }
    return yModule::FAILED;    
}

yModule::RESULT OpenglPolyMapper::matrixUpdated(_tsptr<yCMD>  cmd)
{
    //DEBUG_LOG("maxtrixUpdated:yeah what a long time ");
    if(cmd->data.find("data")!=cmd->data.end())
    {
        _tsptr<DataObject> tempobject = cmd->data["data"];
        if(tempobject!=nullptr)
        {
            auto temp = _tdynamic_pointer_cast<MatrixData>(tempobject);
            if(temp!=nullptr)
            {
                //DEBUG_LOG("step2");
                matrix_data = temp;
            }
        }
        return yModule::SUCCESS;    
    }
    return yModule::FAILED;    
}

// void OpenglPolyMapper::setTempContext(_tsptr<yValue> context)
// {
//     title = QString::fromStdString((*context)["objectName"].asString());
// }

void OpenglPolyMapper::initTemp()
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

void OpenglPolyMapper::resize(int width, int height)
{
    qWarning() << __FUNCTION__;
    float bottom = -1.0f;
    float top = 1.0f;
    float n = 1.0f;
    float f = 100.0f;
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.frustum(-1.0, 1.0, bottom, top, n, f);
}

void OpenglPolyMapper::initShader()
{
    if(mProgram )
    {
        delete mProgram;

    }
    mProgram = new QOpenGLShaderProgram;
   //_tstring vertex =  ySingletone::get()->getConfigDir() + "Shaders/polymapper.vsh.c";
    _tstring vertex = _tstring(":")+"/Shaders/polymapper.vsh.c";

    if (!mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertex.c_str()))
    {
        qWarning() << " add vertex shader file failed. d d df d fd fd ";
        return;
    }
    //_tstring fragment = ySingletone::get()->getConfigDir() + "Shaders/polymapper.fsh.c";
    _tstring fragment = _tstring(":")+"/Shaders/polymapper.fsh.c";

    if (!mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragment.c_str()))
    {
        qWarning() << " add fragment shader file failed.fd fd df df ";
        return;
    }
   /* mProgram->bindAttributeLocation("qt_Vertex", 0);
    mProgram->bindAttributeLocation("texCoord", 1);*/
    mProgram->link();
    mProgram->bind();
}


void OpenglPolyMapper::initTexture()
{
}

void OpenglPolyMapper::initGeometry()
{
    // //读取stl文件
    // STLReader reader;
    // reader.LoadFile((ySingletone::get()->getConfigDir() + "STl/human_head.stl").c_str());
    // if( reader.output)
    // {
    //     mVertices = reader.output->mVertices;
    //     mNormals = reader.output->mNormals;
    // }
    
    //正方形
   // mVertices << QVector3D(-0.1, 0.1, 0.0f) << QVector3D(0.1, 0.1, 0.0f) << QVector3D(0.1, -0.1, 0.0f) << QVector3D(-0.1, -0.1, 0.0f) << QVector3D(-0.1, 0.1, 0.0f);
    mTexcoords << QVector2D(0, 1) << QVector2D(1, 1) << QVector2D(1, 0) << QVector2D(0, 0);
    mViewMatrix.setToIdentity();
    //mViewMatrix.translate(0.0f, 0.0f, -1.0f);
    mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 2.001f), QVector3D(0.0f, 0.0f, -2.0f), QVector3D(0.0f, 1.0f, 0.0f));

    //mViewMatrix.lookAt(QVector3D(0.0f, 0.0f, 1.001f), QVector3D(0.0f, 0.0f, -5.0f), QVector3D(0.0f, 1.0f, 0.0f));
    mModelMatrix.setToIdentity();
    mModelMatrix.rotate(90, QVector3D(1.0, 0.0, 0.0));
}


void OpenglPolyMapper::updateTextureInfo(int width, int height, int format)
{
    
    BYTE* data = new BYTE[width * height * 4];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i % 50 < 25)
            {
                data[4 * (j + i * width) + 0] = 100;
                data[4 * (j + i * width) + 1] = 100;
                data[4 * (j + i * width) + 2] = 100;
                data[4 * (j + i * width) + 3] = 255;
            }
            else
            {
                data[4 * (j + i * width) + 0] = 100;
                data[4 * (j + i * width) + 1] = 100;
                data[4 * (j + i * width) + 2] = 100;
                data[4 * (j + i * width) + 3] = 255;
            }
        }
    }

    mTextureAlloced = true;
}

void OpenglPolyMapper::updateTextureData(/*const YUVData& data*/)
{
   

}
void OpenglPolyMapper::paint()
{
    //mViewMatrix.rotate(1, QVector3D(0.0f, 1.0f, 0.0f));
   //mModelMatrix.rotate(1, QVector3D(0.0f, 1.0f, 0.0f));
    /*if (!mTextureAlloced)
    {
        return;
    }*/
     if(poly_data == NULL)
     {
         return;
     }
    mProgram->bind();

    mModelMatHandle = mProgram->uniformLocation("u_modelMatrix");
    mViewMatHandle = mProgram->uniformLocation("u_viewMatrix");
    mProjectMatHandle = mProgram->uniformLocation("u_projectMatrix");
    mVerticesHandle = mProgram->attributeLocation("qt_Vertex");
    mNormalsHandle = mProgram->attributeLocation("qt_Normal");
    mTexCoordHandle = mProgram->attributeLocation("texCoord");
    //顶点
    
     
    mProgram->enableAttributeArray(mVerticesHandle);
    mProgram->setAttributeArray(mVerticesHandle, poly_data->mVertices.constData());
    mProgram->enableAttributeArray(mNormalsHandle);
    mProgram->setAttributeArray(mNormalsHandle, poly_data->mNormals.constData());

    //  mProgram->enableAttributeArray(mVerticesHandle);
    //     mProgram->setAttributeArray(mVerticesHandle, mVertices.constData());
    //     mProgram->enableAttributeArray(mNormalsHandle);
    //     mProgram->setAttributeArray(mNormalsHandle, mNormals.constData());

    //纹理坐标
    mProgram->enableAttributeArray(mTexCoordHandle);
    mProgram->setAttributeArray(mTexCoordHandle, mTexcoords.constData());

    // MVP矩阵
    //mProgram->setUniformValue(mModelMatHandle, mModelMatrix);
    //mProgram->setUniformValue(mViewMatHandle, mViewMatrix);
    //mProgram->setUniformValue(mProjectMatHandle, mProjectionMatrix);

    
     if(matrix_data)
    {
        mProgram->setUniformValue(mModelMatHandle,  matrix_data->matrix);
    }
    else
    {
        mProgram->setUniformValue(mModelMatHandle, renderer->mPolyModelMatrix);
    }

    
    mProgram->setUniformValue(mViewMatHandle,  renderer->mCamaraMatrix);
    mProgram->setUniformValue(mProjectMatHandle, renderer->mViewProjectionMatrix);

    glDrawArrays(GL_TRIANGLES, 0, poly_data->mVertices.size());

    mProgram->disableAttributeArray(mVerticesHandle);
    mProgram->disableAttributeArray(mTexCoordHandle);
    mProgram->release();
}
