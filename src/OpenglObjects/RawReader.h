#pragma once


class RawReader :public yModule
{
public:
	RawReader();
	~RawReader();

    RESULT doSlot(_tsptr<yCMD> cmd)override;
    RESULT load(_tsptr<yCMD>  cmd);

    _tsptr<OpenglData> output;
};