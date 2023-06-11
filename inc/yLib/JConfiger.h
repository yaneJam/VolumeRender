#pragma once

#define JsonConfiger JConfiger


class IO_API JConfiger :public yConfiger
{
public:
	JConfiger();
	~JConfiger();
	virtual bool loadFile(_tstring filePath)override;
	virtual bool loadContent(_tstring content);
	virtual bool save(bool enc = true)override;
	virtual bool save(_tstring filepath, bool enc = true);
	static bool save(yValue value, _tstring filepath, bool enc = true);
	
	_tsptr<yConfiger> getChildConfiger(_tstring configerName);

	yValue &operator=(yValue& other);
private:

	_tsptr<yConfiger> owner;
	
	bool styled;
};
