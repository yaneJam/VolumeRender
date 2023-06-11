#include "stdafx.h"


extern "C"
{
	OPENGLOBJECTS_API yModuleHandle createModule(_tstring mapperType);
	OPENGLOBJECTS_API void  registerType();
}

yModuleHandle createModule(_tstring mapperType)
{
	yModuleHandle handle;
	_tsptr<yModule> object;

	 if (mapperType == "PiecewisePalette")
	{
		object = _tsptr<PiecewisePalette>(new PiecewisePalette());
	}
	else if (mapperType == "Coordinate")
	{
		object = _tsptr<Coordinate>(new Coordinate());
	}
	else if (mapperType == "ColorPalette")
	{
		object = _tsptr<ColorPalette>(new ColorPalette());
	}
	else if (mapperType == "STLReader")
	{
		object = _tsptr<STLReader>(new STLReader());
	}
	else if (mapperType == "RawReader")
	{
		object = _tsptr<RawReader>(new RawReader());
	}

	


	handle.obj = object;
	return handle;
}
void  registerType()
{
	qmlRegisterType<OSGItem>("OSGItem", 1, 0, "OSGItem");
	qmlRegisterType<MapperItem>("MapperItem", 1, 0, "MapperItem");


	
}

