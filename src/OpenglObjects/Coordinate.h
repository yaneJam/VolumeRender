#ifndef COORDINATE_H
#define COORDINATE_H
#include <QMatrix4x4>

class MatrixData:public yObject
{
public:
	MatrixData(void);
	virtual ~MatrixData(void);
	void modified();
	QMatrix4x4 matrix;
	float pbo_modified_time = 0.0;
};


class Coordinate:public yModule
{
public:
	Coordinate();
	~Coordinate();
	
	RESULT doSlot(_tsptr<yCMD> cmd)override;
	RESULT rotate(_tsptr<yCMD> cmd);
	RESULT translate(_tsptr<yCMD> cmd);
	RESULT scale(_tsptr<yCMD> cmd);
	RESULT setInput(_tsptr<yCMD> cmd);


	_tsptr<MatrixData> local;
	_tsptr<MatrixData> input;
	_tsptr<MatrixData> output;//ouput = input*local

};
#endif
