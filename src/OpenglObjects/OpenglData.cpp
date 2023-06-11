#include "stdafx.h"
#include <map>
#include <QOpenGLTexture>
//#include <set>
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif



OpenglData::OpenglData(void)
{
}

OpenglData::~OpenglData(void)
{

}

QOpenGLTexture* OpenglData::getOpenglTexture()
{
	if(pbo_modified_time>cpu_memory_modified_time&&pbo_modified_time>gpu_memory_modified_time)//最新数据在cuda中
	{
		ERROR_LOG("pbo not supported");
	}
	else
	{
		if(filepath_modified_time>cpu_memory_modified_time)//最新数据在磁盘中
		{
			ERROR_LOG("filepath not supported");
		}
		if(cpu_memory_modified_time>gpu_memory_modified_time)//最新数据在内存中
		{
			loadTextureFromMemory();
		}
	
	}
	return opengl_texture;
}


void OpenglData::loadTextureFromMemory()
{
	if(isInOpenglThread())
	{
		if(dimZ>1)
		{
			// for(int  i = 0 ;i < dimX * dimY * dimZ;i++)
            // {
            //     //m_g_data[i] += 1024;//负值传进opengl会变成0 ,假设CT的最小值是-1024
            //     m_g_data[i] += 1000;//负值传进opengl会变成0 ,假设CT的最小值是-1024
            //     //m_g_data[i] =i%1500+1000;//实验看一下可视化是否正确
            // }


			opengl_texture = new QOpenGLTexture(QOpenGLTexture::Target3D);
			opengl_texture->setFormat(QOpenGLTexture::LuminanceFormat);
			//opengl_texture->setFixedSamplePositions(false);
			opengl_texture->setMinificationFilter(QOpenGLTexture::Nearest);
			opengl_texture->setMagnificationFilter(QOpenGLTexture::Nearest);
			opengl_texture->setWrapMode(QOpenGLTexture::ClampToBorder);
			opengl_texture->setSize(dimX, dimY, dimZ);
  			opengl_texture->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::Int16);//data的格式，就是说内存里边数据的格式
			if(dataformat == SHORT)
			{
				opengl_texture->setData(QOpenGLTexture::Red, QOpenGLTexture::Int16, cpu_memory);
			}
			else
			{
				ERROR_LOG("not supported yet");
			}
    		
		}
		else if (dimY > 1)
		{
			ERROR_LOG("not supported yet");
		}
		else if(dimX>0)
		{
			opengl_texture = new QOpenGLTexture(QOpenGLTexture::Target1D);
			
			if(dataformat == FLOAT)
			{
				opengl_texture->setFormat(QOpenGLTexture::LuminanceFormat);
			}
			else if(dataformat == RGBA)
			{
				opengl_texture->setFormat(QOpenGLTexture::RGBA32F);
			}

			opengl_texture->setSize(dimX); // 纹理的长度为size像素
			opengl_texture->allocateStorage();
			
			opengl_texture->setMinificationFilter(QOpenGLTexture::Linear);
			opengl_texture->setMagnificationFilter(QOpenGLTexture::Linear);
			opengl_texture->setWrapMode(QOpenGLTexture::ClampToEdge);

			if(dataformat == FLOAT)
			{	
				opengl_texture->setData( QOpenGLTexture::Red, QOpenGLTexture::Float32, cpu_memory);
			}
			else if(dataformat == RGBA)
			{
				opengl_texture->setData( QOpenGLTexture::RGBA, QOpenGLTexture::Float32, cpu_memory);
			}
			
		}
		else
		{
			ERROR_LOG("X Y Z == 0");
			return;
		}
		if(opengl_texture)
		{
			gpu_memory_modified_time = cpu_memory_modified_time;
		}
		
	}
	else
	{
		ERROR_LOG("thread not right");	
	}
	
}

bool OpenglData::isInOpenglThread()
{
	//ERROR_LOG("isInOpenglThread() not supported");
	return true;
}

void* OpenglData::allocateCpuMemory(int x, int y, int z, DataFormat format,int filter )
{
	if(cpu_memory && x == dimX && y == dimY && z == dimZ && format == dataformat)
	{
	}
	else
	{
		if(cpu_memory)
		{
			delete[] cpu_memory;
		}
		size_t size = x*y*z;

		//todo:可能会size溢出,也可能分配失败，后面再想办法处理

		switch(format)
		{
			case FLOAT:
			cpu_memory = new float[size];
			memset(cpu_memory, 0, size*sizeof(float));		
			break;
			case RGB:
				cpu_memory = new unsigned char[size*3];
				memset(cpu_memory, 0, size * 3 * sizeof(unsigned char));
				break;
			case RGBA:
				cpu_memory = new float[size*4];
				memset(cpu_memory, 0, size*4*sizeof(float));

				break;
			case SHORT:
				cpu_memory = new short[size];
				memset(cpu_memory, 0, size*sizeof(short));
				break;
			case UCHAR:
				cpu_memory = new unsigned char[size];
				memset(cpu_memory, 0, size*sizeof(unsigned char));
				break;
			case UNSIGNEDSHORT:
				cpu_memory = new unsigned short[size];
				memset(cpu_memory, 0, size*sizeof(unsigned short));
				break;
		}
		cpu_memory_modified_time +=1.0;
		dimX = x;
		dimY = y;
		dimZ = z;
		dataformat = format;
		gl_filter = filter;
	}
	//cpu_memory_modified_time +=1.0;没有重新分配需不需要modify？
	return cpu_memory;
}



