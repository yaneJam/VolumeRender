#include "stdafx.h"
#include <algorithm>
#include <functional>
#include <cstdlib>
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


class CTFFindNodeEqual
{
public:
	double X;
	bool operator () (const CTFNode *node)
	{
		return node->X == this->X;
	}
};

class CTFCompareNodes
{
public:
	bool operator () (const CTFNode *node1,
		const CTFNode *node2)
	{
		return node1->X < node2->X;
	}
};


ColorPalette::ColorPalette()
{
	maxValue = 4500;
	minValue = 0;
	tablesize = 64;
	table = new float[tablesize * 4];


	//default 
	addRGBPoint(0.0, 0.0, 0.0, 0.0, 0.5, 1.0);//start color
	addRGBPoint(180.0, 0.0, 0.0, 0.0, 0.5, 1.0);
	addRGBPoint(1215.0, 145.0 / 255.0, 139.0 / 255.0, 108.0 / 255.0, 0.5, 1.0);
	addRGBPoint(1791.0, 115.0 / 255.0, 85.0 / 255.0, 74 / 255.0, 0.5, 1.0);
	addRGBPoint(3078.0, 1.0, 1.0, 1.0, 0.5, 1.0);
	addRGBPoint(4500.0, 1.0, 1.0, 1.0, 0.5, 1.0);// end color
}
ColorPalette::~ColorPalette()
{
	for (unsigned int i = 0; i<tempvector.size(); i++)
	{
		delete tempvector[i];
	}
	tempvector.clear();
	delete[] table;
}


yModule::RESULT ColorPalette::doSlot(_tsptr<yCMD> cmd)
{
	
	MACRO_doSlot(setInput);
	return yModule::FAILED;
}


yModule::RESULT ColorPalette::setInput(_tsptr<yCMD>  cmd)
{
	if(cmd&&cmd->request&&cmd->request->hasNode("points"))
	{
		DEBUG_LOG("%s",(*cmd->request)["points"].asContent(false).c_str());

		setMinMaxValue(OpenglVolumeMapper::minCTValue,OpenglVolumeMapper::maxCTValue);
		removeAllPoints();

		yValue::ConfigArray points;
		(*cmd->request)["points"].asArray(points);
		
		for(int i = 0;i<points.size();i++)
		{
			auto pos = points[i]["pos"].asDouble();
			auto r = points[i]["r"].asDouble();
			auto g = points[i]["g"].asDouble();
			auto b = points[i]["b"].asDouble();
			//addRGBPoint(pos*(OpenglVolumeMapper::maxCTValue-OpenglVolumeMapper::minCTValue)+OpenglVolumeMapper::minCTValue, (float)(r) / 255.0, (float)(g) / 255.0, (float)(b) / 255.0, 0.5, 1.0);
			addRGBPoint(pos*(OpenglVolumeMapper::maxCTValue-OpenglVolumeMapper::minCTValue)+OpenglVolumeMapper::minCTValue, r,g,b, 0.5, 1.0);
		}
		//float table[256*4];

		m_modified_time += 1.0;
		_tcmd(modified)
		(*modified->request)["modifytime"] = m_modified_time;
		_tsptr<OpenglData> data(new OpenglData);
		int size = 256;
		float* ptr = (float*)data->allocateCpuMemory(size,1,1,OpenglData::RGBA);
		getColorTable(ptr,256);
		modified->data["data"] = _tdynamic_pointer_cast<DataObject>( data);

		emitSignal("modified",modified);


		return yModule::SUCCESS;
	}
	return yModule::FAILED;
}

void ColorPalette::setMinMaxValue(int min, int max)
{
	maxValue = max;
	minValue = min;
}
void ColorPalette::getMinMaxValue(int& min, int& max)
{
	max = maxValue;
	min = minValue;
}
std::vector<CTFNode*> ColorPalette::getPoints()
{
	return tempvector;
}

bool ColorPalette::setPointColor(int index, float x,float r, float g, float b)
{
	int size = tempvector.size();
	if (size - 1 < index)
	{
		return false;
	}
	tempvector[index]->X = x;
	tempvector[index]->R = r;
	tempvector[index]->G = g;
	tempvector[index]->B = b;

	updateTable();
}
bool  ColorPalette::movePoint(int index, float x)
{
	int size = tempvector.size();
	if (size - 1 < index)
	{
		return false;
	}
	 tempvector[index]->X = x;
	 updateTable();

	 return true;
}

void ColorPalette::removePoint(int index)
{
	int size = tempvector.size();
	if (size - 1 < index)
	{
		return;
	}
	float x = tempvector[index]->X;
	removePoint(x);
}


void ColorPalette::removeAllPoints()
{
	for (unsigned int i = 0; i<tempvector.size(); i++)
	{
		delete tempvector[i];
	}
	tempvector.clear();
}

int ColorPalette::removePoint(float x)
{
	unsigned int i;
	for (i = 0; i < tempvector.size(); i++)
	{
		if (tempvector[i]->X == x)
		{
			break;
		}
	}
	int retVal;
	if (i < tempvector.size())
	{
		retVal = i;
	}
	else
	{
		return -1;
	}

	CTFFindNodeEqual FindNodeEqual;
	FindNodeEqual.X = x;
	std::vector<CTFNode*>::iterator iter =
		std::find_if(tempvector.begin(),
		tempvector.end(),
		FindNodeEqual);
	if (iter != tempvector.end())
	{
		delete *iter;
		tempvector.erase(iter);
		bool modifiedInvoked = false;
		return retVal;
	}
	else
	{
		return -1;
	}
}


void ColorPalette::updateTable()
{
	CTFCompareNodes CompareNodes;
	std::sort(tempvector.begin(),
		tempvector.end(),
		CompareNodes);
}



int ColorPalette::addRGBPoint(float x, float r,
	float g, float b,
	float midpoint,
	float sharpness)
{
	if (midpoint < 0.0 || midpoint > 1.0)
	{
		return -1;
	}
	if (sharpness < 0.0 || sharpness > 1.0)
	{
		return -1;
	}
	removePoint(x);
	CTFNode *node = new CTFNode;
	node->X = x;
	node->R = r;
	node->G = g;
	node->B = b;
	node->Midpoint = midpoint;
	node->Sharpness = sharpness;

	tempvector.push_back(node);
	updateTable();
	int index = 0;
	for (int i = 0; i < tempvector.size(); i++)
	{
		if (tempvector[i]->X == x)
		{
			index = i;
			break;
		}
	}

	return index;//����������index
}

// bool ColorPalette::getColorTable(float** ptable, int* psize)
// {
// 	*psize = tablesize;
// 	*ptable = table;
// 	getColorTable(table, *psize);
// 	return true;
// }

bool ColorPalette::getColorTable(float* table, int size)
{
	if (size == 1)
	{
		return false;
	}
	int idx = 0;
	int numNodes = tempvector.size();
	float lastR = 0.0;
	float lastG = 0.0;
	float lastB = 0.0;

	float *tptr = NULL;
	float x = 0.0;
	float x1 = 0.0;
	float x2 = 0.0;
	float rgb1[3] = { 0.0, 0.0, 0.0 };
	float rgb2[3] = { 0.0, 0.0, 0.0 };
	float midpoint = 0.0;
	float sharpness = 0.0;

	for (int i = 0; i< size; i++)
	{
		tptr = table + 4 * i;
		x = static_cast<float>(minValue)+(static_cast<float>(i) / static_cast<float>(size - 1))*maxValue;//CT��Χӳ�䵽minValue~maxValue��
		while (idx < numNodes && x > tempvector[idx]->X)
		{
			idx++;
			if (idx < numNodes)
			{
				x1 = tempvector[idx - 1]->X;
				x2 = tempvector[idx]->X;
				rgb1[0] = tempvector[idx - 1]->R;
				rgb2[0] = tempvector[idx]->R;

				rgb1[1] = tempvector[idx - 1]->G;
				rgb2[1] = tempvector[idx]->G;

				rgb1[2] = tempvector[idx - 1]->B;
				rgb2[2] = tempvector[idx]->B;

				midpoint = tempvector[idx - 1]->Midpoint;
				sharpness = tempvector[idx - 1]->Sharpness;
				if (midpoint < 0.00001)
				{
					midpoint = 0.00001;
				}

				if (midpoint > 0.99999)
				{
					midpoint = 0.99999;
				}
			}
		}
		if (idx >= numNodes)
		{
			tptr[0] = lastR;
			tptr[1] = lastG;
			tptr[2] = lastB;
		}
		else if (idx == 0)
		{
			tptr[0] = tempvector[0]->R;
			tptr[1] = tempvector[0]->G;
			tptr[2] = tempvector[0]->B;
		}
		else//��ɫ��ֵ
		{
			float s = 0.0;
			s = (x - x1) / (x2 - x1);
			if (s < midpoint)
			{
				s = 0.5 * s / midpoint;
			}
			else
			{
				s = 0.5 + 0.5*(s - midpoint) / (1.0 - midpoint);
			}
			//��ʱ����sharpness��������ֻ��ΪsharpnessΪ0�����
			tptr[0] = (1 - s)*rgb1[0] + s*rgb2[0];
			tptr[1] = (1 - s)*rgb1[1] + s*rgb2[1];
			tptr[2] = (1 - s)*rgb1[2] + s*rgb2[2];
		}
	}

	return true;
}


