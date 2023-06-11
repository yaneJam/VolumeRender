#ifndef COLORPALETTE_H
#define COLORPALETTE_H

class  CTFNode
{
public:
	float X;
	float R;
	float G;
	float B;
	float Sharpness;
	float Midpoint;
};
class  ColorPalette:public yModule
{
public:
	ColorPalette();
	~ColorPalette();
	
	RESULT doSlot(_tsptr<yCMD> cmd)override;
	RESULT setInput(_tsptr<yCMD>  cmd);
	bool setPointColor(int index,  float x,float r, float g, float b);
	bool movePoint(int index,float x);
	int removePoint(float x);
	void removeAllPoints();
	void removePoint(int index);
	bool getColorTable(float* table, int size);
	//bool getColorTable(float** ptable, int* psize);
	int addRGBPoint(float x, float r, float g, float b, float midpoint, float sharpness);
	std::vector<CTFNode*> getPoints();


	void getMinMaxValue(int& min, int& max);
	void setMinMaxValue(int min, int max);


protected:
	void updateTable();
	int minValue, maxValue;
	float* table;
	int tablesize;
	std::vector<CTFNode*> tempvector;
	float m_modified_time = 0;
};
#endif