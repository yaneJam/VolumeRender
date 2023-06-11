#pragma once
class ObjectEqual;

#define MACRO_Y_OBJECT(objectname) \
public:\
objectname();\
~objectname();\
virtual _tstring getObjectName() const override\
{\
return _tstring(_T(#objectname));\
};\

//virtual yObject* getInterface(const _tchar* pstrName)override\
//{\
//	if (_tcscmp(pstrName, _T(#objectname)) == 0) return this;\
//	return __super::getInterface(pstrName);\
//};\


class IO_API yObject
{
	friend class ObjectEqual;
public:

	yObject();
	~yObject();
	virtual _tstring getObjectName() const
	{
		return _tstring(_T("yObject"));
	};
	virtual yObject* getInterface(const _tchar* pstrName)
	{
		if (_tcscmp(pstrName, _T("yObject")) == 0) return this;
		return NULL;
	};

	void setInstanceName(const _tchar* name);
	const _tstring getInstanceName();
	virtual _tstring printSelf();
	
protected:
	_tstring InstanceName;
	long long instance_num;

};
#define DataObject yObject
// class IO_API DataObject
// {
// public:
// 	DataObject(void);
// 	virtual ~DataObject(void);
// 	virtual LPVOID getInterface(LPCTSTR pstrName)
// 	{
// 		if (_tcscmp(pstrName, _T("DataObject")) == 0) return this;
// 		return NULL;
// 	};
// 	void set_name(const char* n);
// 	std::string get_name();
// 	//virtual void setPBOModified();//��Ҫͬ���������ݵ�����
// 	//virtual bool is_need_to_synchronization();//��Ҫͬ���������ݵ�����

// protected:
// 	std::string m_name;
// 	//bool need_to_synchronization;//��Ҫ��pboͬ����texture
// };
