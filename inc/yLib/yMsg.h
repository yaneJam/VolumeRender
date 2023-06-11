#pragma once
#include <functional>
class IO_API yMSG
{
public:

	_tstring sType;
	WPARAM wParam;//不建议使用，传一些指针可能要用到
	LPARAM lParam;//不建议使用，传一些指针可能要用到
	_tsptr<yValue> vParam = NULL;//建议使用
	yObject* Sender;//要不要改成弱指针？

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
	void* ptr=NULL;//在多线程的时候注意会指针冲突
};

