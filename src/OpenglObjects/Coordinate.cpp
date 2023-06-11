#include "stdafx.h"
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif



MatrixData::MatrixData(void)
{
	matrix.setToIdentity();
}

MatrixData::~MatrixData(void)
{

}


Coordinate::Coordinate()
{
	input = _tsptr<MatrixData>(new MatrixData);
	local = _tsptr<MatrixData>(new MatrixData);
	output = _tsptr<MatrixData>(new MatrixData);
}

Coordinate::~Coordinate()
{
	
}

yModule::RESULT Coordinate::doSlot(_tsptr<yCMD> cmd)
{
	MACRO_doSlot(rotate);
	MACRO_doSlot(translate);
	MACRO_doSlot(scale);
	MACRO_doSlot(setInput);
	return yModule::FAILED;
}

yModule::RESULT Coordinate::setInput(_tsptr<yCMD> cmd)
{
	if(cmd&&cmd->data.find("data")!=cmd->data.end())
	{
		_tsptr<DataObject> tempobject = cmd->data["data"];
        if(tempobject!=nullptr)
        {
            auto temp = _tdynamic_pointer_cast<MatrixData>(tempobject);
            if(temp!=nullptr)
            {
                input = temp;
				output->matrix = input->matrix*local->matrix;
				_tcmd(modified);
				modified->data["data"] = _tdynamic_pointer_cast<DataObject>(output);
				emitSignal("modified",modified);
            }
        }
		return yModule::SUCCESS;
	}
	return yModule::FAILED;
}

yModule::RESULT Coordinate::rotate(_tsptr<yCMD> cmd)
{
	if(cmd&&cmd->request->hasNode("value"))
	{
		float value = (*cmd->request)["value"].asDouble();
		float x = (*cmd->request)["x"].asDouble();
		float y = (*cmd->request)["y"].asDouble();
		float z = (*cmd->request)["z"].asDouble();
		local->matrix.rotate(value,QVector3D(x,y,z));

		output->matrix = input->matrix*local->matrix;
		_tcmd(modified);
		modified->data["data"] = _tdynamic_pointer_cast<DataObject>(output);
		emitSignal("modified",modified);
		return yModule::SUCCESS;
	}
	return yModule::FAILED;
}

yModule::RESULT Coordinate::translate(_tsptr<yCMD> cmd)
{
	if(cmd&&cmd->request->hasNode("x"))
	{
		float x = (*cmd->request)["x"].asDouble();
		float y = (*cmd->request)["y"].asDouble();
		float z = (*cmd->request)["z"].asDouble();
		local->matrix.translate(QVector3D(x,y,z));


		output->matrix = input->matrix*local->matrix;
		_tcmd(modified);
		modified->data["data"] = _tdynamic_pointer_cast<DataObject>(output);
		emitSignal("modified",modified);
		return yModule::SUCCESS;
	}
	return yModule::FAILED;
}

yModule::RESULT Coordinate::scale(_tsptr<yCMD> cmd)
{
	if(cmd&&cmd->request->hasNode("value"))
	{
		float value = (*cmd->request)["value"].asDouble();
		local->matrix.scale(value);

		output->matrix = input->matrix*local->matrix;
		_tcmd(modified);
		modified->data["data"] = _tdynamic_pointer_cast<DataObject>(output);
		emitSignal("modified",modified);
		return yModule::SUCCESS;
	}
	return yModule::FAILED;
	
}


