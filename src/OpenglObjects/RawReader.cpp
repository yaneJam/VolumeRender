#include "stdafx.h"
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

RawReader::RawReader()
{
}

RawReader::~RawReader()
{
}

yModule::RESULT RawReader::doSlot(_tsptr<yCMD> cmd)
{
    MACRO_doSlot(load);
}

yModule::RESULT RawReader::load(_tsptr<yCMD>  cmd)
{
    if(cmd)
    {
        _tstring filename = (*cmd->request)["filename"].asString();
        int dimX = (*cmd->request)["dimX"].asInt();
        int dimY = (*cmd->request)["dimY"].asInt();
        int dimZ = (*cmd->request)["dimZ"].asInt();

        float spacingX = (*cmd->request)["spacingX"].asDouble();
        float spacingY = (*cmd->request)["spacingY"].asDouble();
        float spacingZ = (*cmd->request)["spacingZ"].asDouble();

        if(!filename.empty())
        {

            output = _tsptr<OpenglData>(new OpenglData);
            short* m_g_data =  (short*)output->allocateCpuMemory(dimX,dimY,dimZ,OpenglData::SHORT);            

    
            FILE* fHandle = fopen(filename.c_str(), "rb");

            if (fHandle == NULL)
            {
                ERROR_LOG("reading %s", filename.c_str());
                return yModule::FAILED;
            }

            fseek(fHandle, 0, SEEK_END);
            int file_size = ftell(fHandle);
            fseek(fHandle, 0, SEEK_SET);
            if (file_size < dimX * dimY * dimZ*sizeof(short))
            {
                ERROR_LOG("file_size error");
                return yModule::FAILED;
            }


            fread(m_g_data , file_size, 1, fHandle);
            fclose(fHandle);

            for(int  i = 0 ;i < dimX * dimY * dimZ;i++)
            {
                //m_g_data[i] += 1024;//负值传进opengl会变成0 ,假设CT的最小值是-1024
                m_g_data[i] += 1000;//负值传进opengl会变成0 ,假设CT的最小值是-1024
                //m_g_data[i] =i%1500+1000;//实验看一下可视化是否正确
            }

           
            _tcmd(modified)
            
            modified->data["data"] = _tdynamic_pointer_cast<DataObject>(output);
            DEBUG_LOG("modified");
            emitSignal("modified",modified);

            return yModule::SUCCESS;
        }       
    }
    return yModule::FAILED;
}
