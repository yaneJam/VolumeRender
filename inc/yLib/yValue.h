#pragma once

#include <set>
#include <mutex>


	class IO_API yValue
	{
		friend class JConfiger;
		friend class yValuePrivate;
	public:
		typedef std::vector<yValue> ConfigArray;
		yValue();
		yValue(const yValue& v)  ;

		yValue(int num);
		yValue(double num);
		yValue(bool num);
		yValue(const _tchar *cstr);
		yValue(std::string str);

		yValue(Json::Value v, _tstring nodeName = "", yValue* parent = NULL);
		yValue(Json::Value v, int index, yValue* parent = NULL);
		~yValue();


		void clear();
		bool isString();
		std::string asString();

		bool isInt();
		int asInt();

		bool isBool();
		bool asBool();
		bool isDouble();
		double asDouble();

		std::vector<_tstring> getAllNodeName();

		yValue getNode(const std::string &key);
		bool hasNode(const std::string &key);
		yValue removeNode(const std::string &key);
		yValue appendNode(_tstring key, yValue value);

		bool isObject();
		bool isArray();
		void asArray(ConfigArray & arrayvalue);
		void setAt(unsigned int index, yValue v);//
		void pushBack(yValue v);//��������һ���ڵ�
		yValue getAt(unsigned int index);//
		bool removeAt(unsigned int index);//������



		yValue operator[](unsigned int index)
		{
			return getAt(index);
		};

		yValue operator[](const std::string &key)
		{
			return getNode(key);
		};

		yValue &operator=(const yValue& other);

		yValue &operator=(int other);
		yValue &operator=(double other);
		yValue& operator=(bool other);
		yValue &operator=(_tstring key);
		yValue &operator=(const _tchar *cstr);

		void lock();
		void unLock();

		_tstring asContent(bool style = true);
		const _tstring getNodeName();
		const yValue* getParent();
		long getModifyTime();
		void setModifyTime(long);

		const Json::Value& asJsonValue() const;
		//CValuePrivate*  imp;
	protected:
		void parentDied(yValue* parent);
		void childDied(yValue* child);
		void addChild(yValue* child);
		std::set<yValue*> m_childs;

		
		yValue* m_parent;
		_tstring NodeName;
		int arrayIndex;
		long modifytime = 0;
	private:
		Json::Value json_value;

		#ifdef MSVC
			HDRecursionLock* pRecursionLock;
		#else
			std::recursive_mutex mtx;
			std::unique_lock<std::recursive_mutex>* pRecursionLock;
		#endif

	};



