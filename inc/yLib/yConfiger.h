#pragma once

#define BaseConfiger yConfiger


	class IO_API yConfiger : public yValue
	{
	public:
		yConfiger();
		~yConfiger();

		virtual _tstring getObjectName() const
		{
			return _tstring(_T("yConfiger"));
		};

		virtual yConfiger* getInterface(const _tchar* pstrName)
		{
			if (_tcscmp(pstrName, _T("yConfiger")) == 0) return this;
			return NULL;
		};
		void setParent(_tsptr<yConfiger> p)
		{
			Parent = p;
		}

		virtual bool loadFile(_tstring filePath) = 0;
		virtual bool save(bool enc = true) = 0;
		virtual bool save(_tstring filepath, bool enc = true) = 0;
		_tstring getFilePath();
		/*
		@m ��ȡ��������
		@p ��������
		@r ������������
		*/
		virtual _tsptr<yConfiger> getChildConfiger(_tstring configerName) = 0;//������
		/*
		@m ��ȡ��������
		@p ��������
		@r ���ظ�������
		*/
		virtual _tsptr<yConfiger> getParentConfiger(_tstring configerName)
		{
			return Parent;
		}


		#ifdef MSVC
			static HDRecursionLock g_ThreadLock;
		#else
			static std::recursive_mutex g_ThreadLock;
		#endif


		static _tstring getContent(_tstring filePath);
		static _tstring encodeString(_tstring content);
		static _tstring decodeString(_tstring content);
	protected:
		
		/*static bool encodeFile(_tstring filePath);
		static bool decodeFile(_tstring filePath);*/


		_tstring configFilePath;
		_tsptr<yConfiger> Parent;

	};
