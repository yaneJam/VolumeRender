#pragma once
class ObjectEqual;

class QOpenGLTexture;
class OpenglData:public yObject
{
public:
	OpenglData(void);
	virtual ~OpenglData(void);
	// virtual LPVOID getInterface(LPCTSTR pstrName)
	// {
	// 	if (_tcscmp(pstrName, _T("OpenglData")) == 0) return this;
	// 	return NULL;
	// };
	enum DataFormat//准备改名成DATAFORMAT
	{
		RGB,
		RGBA,
		SHORT,
		UNSIGNEDSHORT,//投影数据之类的
		FLOAT,
		UCHAR
	} dataformat;

	int dimX=0;
	int dimY=0;
	int dimZ=0;
	int gl_filter = 0 ;//0:要插值，1:最临近值

	void* allocateCpuMemory(int x, int y, int z, DataFormat format,int filter = 0);
	void loadTextureFromMemory();

	QOpenGLTexture* getOpenglTexture();
	bool isInOpenglThread();
	

	_tstring filepath = "";//磁盘地址
	void* cpu_memory=0;//内存地址
	QOpenGLTexture* opengl_texture=0;//显存地址

	float filepath_modified_time = 0.0;
	float cpu_memory_modified_time = 0.0;
	float gpu_memory_modified_time = 0.0;
	float pbo_modified_time = 0.0;//cuda联动,先预留一下
	
};
