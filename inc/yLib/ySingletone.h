//@md
#pragma once

#ifdef useUser
class UserIE;
class UserSqliteManager;
#endif




#define MACRO_ConfigCheckBegin(configname)\
_tsptr<BaseConfiger> configer = ySingletone::get()->getConfiger();\
configer = configer->getChildConfiger(configname);\
SYSTEMTIME lpCompileTime;\
char Mmm[4] = "Jan";\
sscanf_s(__DATE__, "%3s %hu %hu", Mmm, sizeof(Mmm),&lpCompileTime.wDay, &lpCompileTime.wYear);\
switch (*(DWORD*)Mmm)\
 {\
		case (DWORD)('naJ') : lpCompileTime.wMonth = 1; break; \
		case (DWORD)('beF') : lpCompileTime.wMonth = 2; break; \
		case (DWORD)('raM') : lpCompileTime.wMonth = 3; break; \
		case (DWORD)('rpA') : lpCompileTime.wMonth = 4; break; \
		case (DWORD)('yaM') : lpCompileTime.wMonth = 5; break; \
		case (DWORD)('nuJ') : lpCompileTime.wMonth = 6; break; \
		case (DWORD)('luJ') : lpCompileTime.wMonth = 7; break; \
		case (DWORD)('guA') : lpCompileTime.wMonth = 8; break; \
		case (DWORD)('peS') : lpCompileTime.wMonth = 9; break; \
		case (DWORD)('tcO') : lpCompileTime.wMonth = 10; break; \
		case (DWORD)('voN') : lpCompileTime.wMonth = 11; break; \
		case (DWORD)('ceD') : lpCompileTime.wMonth = 12; break; \
		case (DWORD)('Jan') : lpCompileTime.wMonth = 1; break; \
		case (DWORD)('Feb') : lpCompileTime.wMonth = 2; break; \
		case (DWORD)('Mar') : lpCompileTime.wMonth = 3; break; \
		case (DWORD)('Apr') : lpCompileTime.wMonth = 4; break; \
		case (DWORD)('May') : lpCompileTime.wMonth = 5; break; \
		case (DWORD)('Jun') : lpCompileTime.wMonth = 6; break; \
		case (DWORD)('Jul') : lpCompileTime.wMonth = 7; break; \
		case (DWORD)('Aug') : lpCompileTime.wMonth = 8; break; \
		case (DWORD)('Sep') : lpCompileTime.wMonth = 9; break; \
		case (DWORD)('Oct') : lpCompileTime.wMonth = 10; break; \
		case (DWORD)('Nov') : lpCompileTime.wMonth = 11; break; \
		case (DWORD)('Dec') : lpCompileTime.wMonth = 12; break; \
		default:lpCompileTime.wMonth = 0; \
}\
int compliedate = lpCompileTime.wYear * 10000 + lpCompileTime.wMonth * 100 + lpCompileTime.wDay; \
sscanf_s(__TIME__, "%hu:%hu:%hu", &lpCompileTime.wHour,&lpCompileTime.wMinute, &lpCompileTime.wSecond);\
int complietime = lpCompileTime.wHour * 10000 + lpCompileTime.wMinute * 100 + lpCompileTime.wSecond;\
if((*configer)["description"]["manual"].asInt()!=1)\
{\
	if ((*configer)["description"]["compliedate"].asInt() < compliedate || \
		(((*configer)["description"]["compliedate"].asInt() == compliedate)\
		&& ((*configer)["description"]["complietime"].asInt() < complietime)))\
	{\
		DEBUG_LOG("updating %s", configer->getFilePath().c_str());\
		(*configer).clear();\
		(*configer)["description"]["compliedate"] = compliedate;\
		(*configer)["description"]["complietime"] = complietime;\
		(*configer)["description"]["manual"] = 0;

#define MACRO_ConfigCheckEnd\
		configer->save(false);\
	}\
}


#define MACRO_PluginConfigBegin(configname)\
_tsptr<BaseConfiger> configer = ySingletone::get()->getPluginConfiger();\
configer = configer->getChildConfiger(configname);\
SYSTEMTIME lpCompileTime;\
char Mmm[4] = "Jan";\
sscanf_s(__DATE__, "%3s %hu %hu", Mmm, sizeof(Mmm),&lpCompileTime.wDay, &lpCompileTime.wYear);\
switch (*(DWORD*)Mmm)\
 {\
		case (DWORD)('naJ') : lpCompileTime.wMonth = 1; break; \
		case (DWORD)('beF') : lpCompileTime.wMonth = 2; break; \
		case (DWORD)('raM') : lpCompileTime.wMonth = 3; break; \
		case (DWORD)('rpA') : lpCompileTime.wMonth = 4; break; \
		case (DWORD)('yaM') : lpCompileTime.wMonth = 5; break; \
		case (DWORD)('nuJ') : lpCompileTime.wMonth = 6; break; \
		case (DWORD)('luJ') : lpCompileTime.wMonth = 7; break; \
		case (DWORD)('guA') : lpCompileTime.wMonth = 8; break; \
		case (DWORD)('peS') : lpCompileTime.wMonth = 9; break; \
		case (DWORD)('tcO') : lpCompileTime.wMonth = 10; break; \
		case (DWORD)('voN') : lpCompileTime.wMonth = 11; break; \
		case (DWORD)('ceD') : lpCompileTime.wMonth = 12; break; \
		case (DWORD)('Jan') : lpCompileTime.wMonth = 1; break; \
		case (DWORD)('Feb') : lpCompileTime.wMonth = 2; break; \
		case (DWORD)('Mar') : lpCompileTime.wMonth = 3; break; \
		case (DWORD)('Apr') : lpCompileTime.wMonth = 4; break; \
		case (DWORD)('May') : lpCompileTime.wMonth = 5; break; \
		case (DWORD)('Jun') : lpCompileTime.wMonth = 6; break; \
		case (DWORD)('Jul') : lpCompileTime.wMonth = 7; break; \
		case (DWORD)('Aug') : lpCompileTime.wMonth = 8; break; \
		case (DWORD)('Sep') : lpCompileTime.wMonth = 9; break; \
		case (DWORD)('Oct') : lpCompileTime.wMonth = 10; break; \
		case (DWORD)('Nov') : lpCompileTime.wMonth = 11; break; \
		case (DWORD)('Dec') : lpCompileTime.wMonth = 12; break; \
		default:lpCompileTime.wMonth = 0; \
}\
int compliedate = lpCompileTime.wYear * 10000 + lpCompileTime.wMonth * 100 + lpCompileTime.wDay; \
sscanf_s(__TIME__, "%hu:%hu:%hu", &lpCompileTime.wHour,&lpCompileTime.wMinute, &lpCompileTime.wSecond);\
int complietime = lpCompileTime.wHour * 10000 + lpCompileTime.wMinute * 100 + lpCompileTime.wSecond;\
if((*configer)["description"]["manual"].asInt()!=1)\
{\
	if ((*configer)["description"]["compliedate"].asInt() < compliedate || \
		(((*configer)["description"]["compliedate"].asInt() == compliedate)\
		&& ((*configer)["description"]["complietime"].asInt() < complietime)))\
	{\
		DEBUG_LOG("updating %s", configer->getFilePath().c_str());\
		(*configer).clear();\
		(*configer)["description"]["compliedate"] = compliedate;\
		(*configer)["description"]["complietime"] = complietime;\
		(*configer)["description"]["manual"] = 0;

#define MACRO_PluginConfigEnd\
		configer->save(false);\
	}\
}


class IO_API ySingletone :public yObject
{
	friend class yService;
public:
	/*
	@m �̳иú�������ȡ�����ļ���·��
	*/
	virtual _tsptr<yConfiger> getConfiger() = 0;
	virtual _tstring getAppVersion() = 0;
	
	_tstring transferDirPath(_tstring filepath);//��ʱ�ȷ�������
	_tstring getProgramDir();//��ʱ�ȷ�������

	_tsptr<yConfiger> getPluginConfiger();
	_tsptr<yValue> getPluginInfo();
	_tstring getConfigDir();//�����ļ�·��
	void setConfigDir(_tstring p);



	_tstring getTempDir();//��ʱ�ļ�·��
	void setTempDir(_tstring p);

	//void getAllService(std::list<yService*>& list);
	//void getAllService();
	//virtual void initAllService();
	//void deleteAllService();

	static ySingletone* getApp();//retiring
	static ySingletone* get();
	void setSceneCode(_tstring c);
	_tstring  getSceneCode();

	//static yService* findService(const _tchar* name);

	virtual _tstring getObjectName() const override
	{
		return _tstring(_T("ySingletone"));
	} ;

	//virtual yObject* getInterface(const _tchar* pstrName)override
	//{
	//	if (_tcscmp(pstrName, _T("ySingletone")) == 0) return this;
	//	return __super::getInterface(pstrName);
	//};
#ifdef useUser
	_tsptr<UserIE>  getUser();
	_tsptr<UserSqliteManager> getUserManager();
#endif


	virtual _tsptr<yModule> createModule(_tstring seqType,_tstring instanceName);
	_tsptr<yModule> getModule(_tstring instanceName);
	bool addModule(_tsptr<yModule> a);
	bool removeModule(_tsptr<yModule>);
	bool removeModule(_tstring instancename);

	bool pushPlugin(_tstring dllName);
	void scanPlugins(_tstring path);
	bool scanPlugin(_tstring fliepath);

	bool isSubScene(std::string str);//����SceneCode�Ƿ����str
	std::string getSubScene();
	_tsptr<yValue> AppContex;
protected:
	
	#ifdef MSVC
	yRecursionLock module_lock;
	yRecursionLock g_ThreadLock;
	#else
	std::recursive_mutex g_ThreadLock;
	std::recursive_mutex module_lock;
	#endif

	
	std::set<_tsptr<yModule>> modules;


	std::list<createModuleFunc> funs;
	std::map<_tstring, HINSTANCE> dlls;

	ySingletone();
	~ySingletone();


	//static void addRefService(yService* service);
	//static void releaseService(yService* service); 

	_tstring config_dir;
	_tstring temp_dir;
	
	

	_tstring SceneCode;
#ifdef useUser
	_tsptr<UserSqliteManager> usermanager;
#endif
	_tsptr<JsonConfiger> m_pluginConfiger;
	_tsptr<yValue> pluginInfo;



};