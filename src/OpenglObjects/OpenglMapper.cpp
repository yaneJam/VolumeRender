#include "OpenglMapper.h"

#include <QOpenGLPixelTransferOptions>

//实验用
#include "gl/GLU.h" // windows操作系统提供的窗口管理功能
#pragma comment(lib,"Opengl32.lib")
#pragma comment(lib,"glu32.lib")



void safeDeleteTexture(QOpenGLTexture* texture)
{
    if (texture)
    {
        if (texture->isBound())
        {
            texture->release();
        }
        if (texture->isCreated())
        {
            texture->destroy();
        }
        delete texture;
        texture = nullptr;
    }
}

OpenglMapper::OpenglMapper()
{
    qWarning() << __FUNCTION__;
}

OpenglMapper::~OpenglMapper()
{
   // qWarning() << __FUNCTION__;
   /* safeDeleteTexture(mTexY);
    safeDeleteTexture(mTexU);
    safeDeleteTexture(mTexV);*/
}

void OpenglMapper::setTempContext(_tsptr<yValue> context)
{
    title = QString::fromStdString((*context)["objectName"].asString());
    yModule::setInstanceName((*context)["objectName"].asString().c_str());



    fragment_file =(*context)["fragment_file"].asString().c_str();
    vertex_file =(*context)["vertex_file"].asString().c_str();
}

void OpenglMapper::initTemp()
{
    qWarning() << __FUNCTION__;
    initializeOpenGLFunctions();
    glDepthMask(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    initShader();
    initTexture();
    initGeometry();
    isInit = true;
}
void OpenglMapper::resize(int width, int height)
{
    qWarning() << __FUNCTION__;
    glViewport(0, 0, width, height);
    float bottom = -1.0f;
    float top = 1.0f;
    float n = 1.0f;
    float f = 100.0f;
   /* mProjectionMatrix.setToIdentity();
    mProjectionMatrix.frustum(-1.0, 1.0, bottom, top, n, f);*/
}



void OpenglMapper::initShader()
{
   /* mProgram.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
        "attribute highp vec4 vertices;"
        "varying highp vec2 coords;"
        "void main() {"
        "    gl_Position = vertices;"
        "    coords = vertices.xy;"
        "}");
    mProgram.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
        "uniform lowp float t;"
        "varying highp vec2 coords;"
        "void main() {"
        "    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
        "    i = smoothstep(t - 0.8, t + 0.8, i);"
        "    i = floor(i * 20.) / 20.;"
        "    gl_FragColor = vec4(coords * .5 + .5, i, i);"
        "}");

    mProgram.bindAttributeLocation("vertices", 0);
    mProgram.link();*/

}
void OpenglMapper::initTexture()
{
 
}

void OpenglMapper::initGeometry()
{

}

void OpenglMapper::paint()
{
    

   // float rotAngle = 0.0f;
   // // 初始化
   // glEnable(GL_LINE_SMOOTH); // 启用或禁止服务器**功能
   // glEnable(GL_BLEND);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   // glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
   // glLineWidth(1.5);

 


   // // 重定义窗口大小
   //// int w = m_window->width(), h = m_window->height();
   //// glViewport(0, 0, w, h);
   // int w = 680;
   // int h = 240;
   // glMatrixMode(GL_PROJECTION);
   // glLoadIdentity();
   // if (w <= h)
   //     gluOrtho2D(-1.0, 1.0,
   //         -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w);
   // else
   //     gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h,
   //         1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
   // glMatrixMode(GL_MODELVIEW);
   // glLoadIdentity();


   // // 显示内容
   // //glClear(GL_COLOR_BUFFER_BIT);

   // glColor3f(0.0, 1.0, 0.0);
   // glPushMatrix();
   // glRotatef(-rotAngle, 0.0, 0.0, 0.1);
   // glBegin(GL_LINES);
   // glVertex2f(-0.5, 0.5);
   // glVertex2f(0.5, -0.5);
   // glEnd();
   // glPopMatrix();

   // glColor3f(0.0, 0.0, 1.0);
   // glPushMatrix();
   // glRotatef(rotAngle, 0.0, 0.0, 0.1);
   // glBegin(GL_LINES);
   // glVertex2f(0.5, 0.5);
   // glVertex2f(-0.5, -0.5);
   // glEnd();
   // glPopMatrix();

   // glFlush();
}
