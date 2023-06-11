#pragma once

#include <memory>
#include <string>
#include <mutex>


#define DEBUG_LOG yLogger::writer(DEBUG_LEVEL_DEBUG,__FUNCTION__)->write
#define INFO_LOG yLogger::writer(DEBUG_LEVEL_INFO,__FUNCTION__)->write
#define WARN_LOG yLogger::writer(DEBUG_LEVEL_WARN,__FUNCTION__)->write
#define ERROR_LOG yLogger::writer(DEBUG_LEVEL_ERROR,__FUNCTION__)->write


#define UTF8_DEBUG_LOG yLogger::writer(DEBUG_LEVEL_DEBUG,__FUNCTION__)->writeUTF8
#define UTF8_INFO_LOG yLogger::writer(DEBUG_LEVEL_INFO,__FUNCTION__)->writeUTF8
#define UTF8_WARN_LOG yLogger::writer(DEBUG_LEVEL_WARN,__FUNCTION__)->writeUTF8
#define UTF8_ERROR_LOG yLogger::writer(DEBUG_LEVEL_ERROR,__FUNCTION__)->writeUTF8


//��־����
enum DEBUG_LEVEL{
	DEBUG_LEVEL_DEBUG = 0,
	DEBUG_LEVEL_INFO,
	DEBUG_LEVEL_USERDEF1,
	DEBUG_LEVEL_USERDEF2,
	DEBUG_LEVEL_USERDEF3,
	DEBUG_LEVEL_USERDEF4,
	DEBUG_LEVEL_WARN,
	DEBUG_LEVEL_ERROR,
	DEBUG_LEVEL_FATAL
};
static const char *DEBUG_LEVEL_NAME[] = {
	"debug",
	"info",
	"userdef1",
	"userdef2",
	"userdef3",
	"userdef4",
	"warn",
	"error",
	"fatal"
};

//��־�����ʽ
enum yLogger_OUTPUTMODE
{
	yLogger_OUTPUTMODE_file = 0,//���浽��־�ļ�
	yLogger_OUTPUTMODE_file_console,//���浽��־�ļ������������̨
	yLogger_OUTPUTMODE_file_debugger,//���浽��־�ļ��������������
};

class yLogger;

//���ַ��� 
#ifdef _UNICODE
#define logger_string std::wstring
#define logger_char wchar_t
#else
#define logger_char char
#define logger_string std::string
#endif



#if defined(__cplusplus) || defined(c_plusplus)//��ƽ̨���巽��
extern "C"{
	void IO_API yLogger_Thread_Test(int thread_count);//�����ٸ��߳�
	void IO_API yLogger_Thread_Test_End();
};
#endif


class IO_API HDLogWriter
{
	friend class yLogger;
public:
	void write(const char* pszFormat, ...);//���ֽ�
	void write(logger_string string);
	

	void write(const wchar_t* pszFormat, ...);//��ת���ɶ��ֽ��ٴ�ӡ
	void writeUTF8(const char* pszFormat, ...);//UTF8��ת���ɶ��ֽ��ٴ�ӡ
protected:
	HDLogWriter(yLogger* logger, int level, const char* func);
	yLogger* m_owner;
	int m_logging_level;
	std::string  m_func;
};

class IO_API yLogger
{
	friend class HDLogWriter;
public:
	~yLogger();
	static std::shared_ptr<HDLogWriter> writer(int level, const char* func);
	static yLogger* singleton(const logger_char* logname);
	static yLogger* singleton();
	static void destroy();
	static void setOutputMode(yLogger_OUTPUTMODE m);


	void setLevel(DEBUG_LEVEL level);//����ļ���
protected:

	yLogger();
	yLogger(const logger_char* logname, yLogger_OUTPUTMODE output_mode = yLogger_OUTPUTMODE_file);

	void OpenFile(const logger_char* strFile, bool bAppend = true);
	void ChangeFile(const logger_char* strFile, bool bAppend = true);
	void CloseFile();


	FILE*	m_pLogFile;
	FILE*	m_pConsoleFile;

	yLogger_OUTPUTMODE	m_output_mode;//0:������־�ļ� 1:������־�ļ�����ӡ��һ����ɫ����̨��2:������־�ļ�����ӡ�����Դ���
	logger_string m_filename;
#ifdef MSVC
	CRITICAL_SECTION m_cs;
#else
	std::recursive_mutex mtx;

#endif
	char* m_temp_szLine;
	char* m_temp_arg;
	wchar_t* m_temp_warg;
	int sz_max_size;
	//int m_logging_level;
	int log_level;
	//std::string m_func;
	static yLogger* s_singleton;
};
