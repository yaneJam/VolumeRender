#include "stdafx.h"
#include <algorithm>
#include <functional>
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


//class /*PiecewiseFunctionNode*/PFNode
//{
//public:
//	float X;
//	float Y;
//	float Sharpness;
//	float Midpoint;
//};


class PFNodeCompareNodes
{
public:
	bool operator () (const PFNode *node1,
		const PFNode *node2)
	{
		return node1->X < node2->X;
	}
};


class PFNodeFindNodeEqual
{
public:
	double X;
	bool operator () (const PFNode *node)
	{
		return node->X == this->X;
	}
};



PiecewisePalette::PiecewisePalette()
{
	maxValue = 4500;
	minValue = 0;
	table = new float[table_size];

	//default point
	addPoint(0.0, 0.0, 0.5, 0.5);//start 
	addPoint(661.500000, 0.003096, 0.5, 0.6);
	addPoint(1309.500000, 0.0, 0.5, 0.25);
	addPoint(2344.500000, 0.232198, 0.5, 0.2);
	addPoint(3388.500000, 0.089783, 0.5, 0.8);
	addPoint(4500.0, 0.0, 0.5, 0.5);// end 
}

PiecewisePalette::~PiecewisePalette()
{
	for (unsigned int i = 0; i<tempvector.size(); i++)
	{
		delete tempvector[i];
	}
	tempvector.clear();
	delete[] table;
}

void PiecewisePalette::clearTable()
{
	for (unsigned int i = 0; i<tempvector.size(); i++)
	{
		delete tempvector[i];
	}
	tempvector.clear();
}

int PiecewisePalette::addPoint(float x, float y,
	float midpoint, float sharpness)
{
	if (midpoint < 0.0 || midpoint > 1.0)
	{
		return -1;
	}

	if (sharpness < 0.0 || sharpness > 1.0)
	{
		return -1;
	}


	this->removePoint(x);

	PFNode*node = new PFNode;
	node->X = x;
	node->Y = y;
	node->Sharpness = sharpness;
	node->Midpoint = midpoint;


	tempvector.push_back(node);
	this->updateTable();




	return 1;
}


yModule::RESULT PiecewisePalette::doSlot(_tsptr<yCMD> cmd)
{
	
	MACRO_doSlot(setInput);
	return yModule::FAILED;
}

yModule::RESULT PiecewisePalette::setInput(_tsptr<yCMD>  cmd)
{
	if(cmd&&cmd->request&&cmd->request->hasNode("points"))
	{
		yValue::ConfigArray points;
		(*cmd->request)["points"].asArray(points);

		//DEBUG_LOG("%s",(*cmd->request)["points"].asContent(false).c_str());
		setMinMax(OpenglVolumeMapper::minCTValue,OpenglVolumeMapper::maxCTValue);
		clearTable();


		for(int i = 0;i<points.size();i++)
		{
			auto x = points[i]["x"].asDouble();
			auto y = points[i]["y"].asDouble();	
			addPoint(x,y, 0.5, 0.5);
		}
		updateTable();

		m_modified_time += 1.0;
		_tcmd(modified)
		(*modified->request)["modifytime"] = m_modified_time;
		_tsptr<OpenglData> data(new OpenglData);
		int size = 256;
		float* ptr = (float*)data->allocateCpuMemory(size,1,1,OpenglData::FLOAT);
		getOpacityTable(ptr,size);
		modified->data["data"] = _tdynamic_pointer_cast<DataObject>( data);
	 	//DEBUG_LOG("modified");
		emitSignal("modified",modified);


		return yModule::SUCCESS;
	}
	return yModule::FAILED;

}

// yModule::RESULT PiecewisePalette::getOutput(_tsptr<yCMD>  cmd)
// {
// 	_tsptr<OpenglData> data(new OpenglData);
// 	float* ptr = (float*)data->allocateCpuMemory(256,1,1,OpenglData::FLOAT);
// 	getOpacityTable(ptr,256);

// 	cmd->data["data"] = _tdynamic_pointer_cast<DataObject>( data);

// 	return SUCCESS;
// }

void PiecewisePalette::setMinMax(int min, int max)
{
	minValue = min;
	maxValue = max;
}

std::vector<PFNode*>& PiecewisePalette::getPoints()
{
	return tempvector;
}

void PiecewisePalette::updateTable()
{
	PFNodeCompareNodes CompareNodes;
	std::sort(tempvector.begin(),
		tempvector.end(),
		CompareNodes);
}


int PiecewisePalette::removePoint(float x)
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

	PFNodeFindNodeEqual FindNodeEqual;
	FindNodeEqual.X = x;


	std::vector<PFNode*>::iterator iter =
		std::find_if(tempvector.begin(),
		tempvector.end(),
		FindNodeEqual);


	if (iter != tempvector.end())
	{
		delete *iter;
		this->tempvector.erase(iter);

		bool modifiedInvoked = false;
	}
	else
	{

		return -1;
	}


	return retVal;
}


void PiecewisePalette::removePoint(int index)
{
	int size = tempvector.size();
	if (size - 1 < index)
	{
		return;
	}
	float x = tempvector[index]->X;
	removePoint(x);
}



bool PiecewisePalette::getOpacityTable(float** ptable, int* psize)
{
	*psize = table_size;
	*ptable = table;
	getOpacityTable(table, *psize);
	return true;

}
bool PiecewisePalette::getOpacityTable(float* tb, int size)
{
	if (size == 1)//��ʱ�Ȳ���
	{
		return false;
	}

	int i;
	int idx = 0;
	int numNodes = static_cast<int>(tempvector.size());
	float lastValue = 1.0;



	float *tptr = NULL;
	float x = 0.0;
	float x1 = 0.0;
	float x2 = 0.0;
	float y1 = 0.0;
	float y2 = 0.0;
	float midpoint = 0.0;
	float sharpness = 0.0;




	for (int i = 0; i< size; i++)
	{
		tptr = tb + i;
		x = static_cast<float>(minValue)+(static_cast<float>(i) / static_cast<float>(size - 1))*maxValue;//CT��Χӳ�䵽minValue~maxValue��
		while (idx < numNodes && x > tempvector[idx]->X)
		{
			idx++;
			if (idx < numNodes)
			{
				x1 = tempvector[idx - 1]->X;
				x2 = tempvector[idx]->X;

				y1 = tempvector[idx - 1]->Y;
				y2 = tempvector[idx]->Y;
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
			*tptr = 0.0;
		}
		else if (idx == 0)
		{
			*tptr = tempvector[0]->Y;
		}
		else
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

			if (sharpness > 0.99)
			{
				if (s < 0.5)
				{
					*tptr = y1;
					continue;
				}

				else
				{
					*tptr = y2;
					continue;
				}
			}
			if (sharpness < 0.01)
			{

				*tptr = (1 - s)*y1 + s*y2;
				continue;
			}

			if (s < .5)
			{
				s = 0.5 * pow((float)(s * 2), (float)(1.0 + 10 * sharpness));
			}
			else if (s > .5)
			{
				s = 1.0 - 0.5 * pow((float)((1.0 - s) * 2), (float)(1 + 10 * sharpness));
			}

			// Compute some coefficients we will need for the hermite curve
			float ss = s*s;
			float sss = ss*s;

			float h1 = 2 * sss - 3 * ss + 1;
			float h2 = -2 * sss + 3 * ss;
			float h3 = sss - 2 * ss + s;
			float h4 = sss - ss;

			float slope;
			float t;

			// Use one slope for both end points
			slope = y2 - y1;
			t = (1.0 - sharpness)*slope;

			// Compute the value
			*tptr = h1*y1 + h2*y2 + h3*t + h4*t;

			// Final error check to make sure we don't go outside
			// the Y range
			float min = (y1<y2) ? (y1) : (y2);
			float max = (y1>y2) ? (y1) : (y2);

			*tptr = (*tptr < min) ? (min) : (*tptr);
			*tptr = (*tptr > max) ? (max) : (*tptr);


		}
	}


	
	

	return true;
}

