#pragma once
#include <functional>
class IO_API yMSG
{
public:

	_tstring sType;
	WPARAM wParam;//������ʹ�ã���һЩָ�����Ҫ�õ�
	LPARAM lParam;//������ʹ�ã���һЩָ�����Ҫ�õ�
	_tsptr<yValue> vParam = NULL;//����ʹ��
	yObject* Sender;//Ҫ��Ҫ�ĳ���ָ�룿

};

#define _tcmd(x) \
_tsptr<yCMD> x = _tsptr<yCMD>(new yCMD);\
x->request = _tsptr<yValue>(new yValue);

class IO_API yCMD
{
public:
	enum ErrorCode{
		SUCCESS = 200,
		FAILED = 403,
		TIMEOUT = 638,
	};
	_tstring caller;
	_tstring method;
	_tsptr<yValue> request;
	_tsptr<yValue> response;
	int resultCode = FAILED;
	int cmdID = -1;
	int timerID = -1;
	_tstring uuid;
	std::function<void(int resultCode, _tsptr<yValue> response)> callback;
	std::map<_tstring, _tsptr<DataObject>> data;
	void* ptr=NULL;//�ڶ��̵߳�ʱ��ע���ָ���ͻ
};

