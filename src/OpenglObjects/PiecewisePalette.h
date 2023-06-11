#ifndef PIECEWISEPALETTE_H
#define PIECEWISEPALETTE_H

class  PFNode
{
public:
	float X;
	float Y;
	float Sharpness;
	float Midpoint;
};

class PiecewisePalette:public yModule
{
public:
	PiecewisePalette();
	~PiecewisePalette();
	
	RESULT doSlot(_tsptr<yCMD> cmd)override;
	RESULT setInput(_tsptr<yCMD>  cmd);
	//RESULT getOutput(_tsptr<yCMD>  cmd);

	int addPoint(float x, float y, float midpoint, float sharpness);
	
	bool getOpacityTable(float* table, int size);
	bool getOpacityTable(float** ptable, int* psize);

	void setMinMax(int min, int max);
	int removePoint(float x);
	void removePoint(int index);
	void updateTable();
	void clearTable();
	std::vector<PFNode*>& getPoints();
protected:
	int minValue, maxValue;
	float* table;
	int table_size = 512;
	std::vector<PFNode*> tempvector;
	float m_modified_time = 0;
};
#endif