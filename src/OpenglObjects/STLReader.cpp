#include "stdafx.h"
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
STLReader::STLReader()
{
    m_xyz[0] = 0;
    m_xyz[1] = 1;
    m_xyz[2] = 2;
    m_offset[0] = 0.0;
    m_offset[1] = 0.0;
    m_offset[2] = 0.0;
    m_scale[0] = 1.0;
    m_scale[1] = 1.0;
    m_scale[2] = 1.0;

}

STLReader::~STLReader()
{
}

yModule::RESULT STLReader::doSlot(_tsptr<yCMD> cmd)
{
    MACRO_doSlot(load);
}

yModule::RESULT STLReader::load(_tsptr<yCMD>  cmd)
{
    if(cmd)
    {
        _tstring filename = (*cmd->request)["filename"].asString();
        if(!filename.empty())
        {
            if(LoadFile(filename.c_str()))
            {
                m_modified_time += 1.0;
                _tcmd(modified)
                (*modified->request)["modifytime"] = m_modified_time;
                modified->data["data"] = _tdynamic_pointer_cast<DataObject>(output);
                DEBUG_LOG("modified");
                emitSignal("modified",modified);
                return yModule::SUCCESS;
            }
            
        }       
    }
    return yModule::FAILED;
}

void STLReader::setParam(int xyz[3], float offset[3], float scale[3])
{
    m_xyz[0] = xyz[0];
    m_xyz[1] = xyz[1];
    m_xyz[2] = xyz[2];
    m_offset[0] = offset[0];
    m_offset[1] = offset[1];
    m_offset[2] = offset[2];
    m_scale[0] = scale[0];
    m_scale[1] = scale[1];
    m_scale[2] = scale[2];

}

//bool STLReader::get_output(_tsptr<DataObject>* data, int port)
//{
//    if (port == 0)
//    {
//        
//        *data = output;
//    }
//    return true;
//}

//
//STLReader::FileTypeEnum STLReader::DetectFileType(const char* filename,
//    unsigned long length,
//    double percent_bin)
//{
//    if (!filename || percent_bin < 0) {
//        return STLReader::FileTypeUnknown;
//    }
//
//   /* if (STLReader::FileIsDirectory(filename)) {
//        return STLReader::FileTypeUnknown;
//    }*/
//
//    FILE* fp = fopen(filename, "rb");
//    if (!fp) {
//        return STLReader::FileTypeUnknown;
//    }
//
//    // Allocate buffer and read bytes
//
//    unsigned char* buffer = new unsigned char[length];
//    size_t read_length = fread(buffer, 1, length, fp);
//    fclose(fp);
//    if (read_length == 0) {
//        delete[] buffer;
//        return STLReader::FileTypeUnknown;
//    }
//
//    // Loop over contents and count
//
//    size_t text_count = 0;
//
//    const unsigned char* ptr = buffer;
//    const unsigned char* buffer_end = buffer + read_length;
//
//    while (ptr != buffer_end) {
//        if ((*ptr >= 0x20 && *ptr <= 0x7F) || *ptr == '\n' || *ptr == '\r' ||
//            *ptr == '\t') {
//            text_count++;
//        }
//        ptr++;
//    }
//
//    delete[] buffer;
//
//    double current_percent_bin = (static_cast<double>(read_length - text_count) /
//        static_cast<double>(read_length));
//
//    if (current_percent_bin >= percent_bin) {
//        return STLReader::FileTypeBinary;
//    }
//
//    return STLReader::FileTypeText;
//}

inline std::string stlParseEof(const std::string& expected)
{
    return "Premature EOF while reading '" + expected + "'";
}

inline std::string stlParseExpected(const std::string& expected, const std::string& found)
{
    return "Parse error. Expecting '" + expected + "' found '" + found + "'";
}


bool STLReader::stlReadNormal(char* buf, float  normalCoord[3])
{
    char* begptr = buf;
    char* endptr = nullptr;



    while (*begptr && !isspace(*begptr))
    {
        ++begptr;
    }
    while (isspace(*begptr))
    {
        ++begptr;
    }


   
    for (int i = 0; i < 3; ++i)
    {
        // We really should use: vertCoord[i] = std::strtof(begptr, &endptr);
        // instead of strtod below but Apple Clang 9.0.0.9000039 doesn't
        // recognize strtof as part of the C++11 standard
        //normalCoord[i] = static_cast<float>(std::strtod(begptr, &endptr));

        normalCoord[m_xyz[i]] = static_cast<float>(std::strtod(begptr, &endptr))*m_scale[m_xyz[i]];
        if (begptr == endptr)
        {
            return false;
        }

        begptr = endptr;
    }
    //��һ������
    float apple = 1.0/sqrtf(pow(normalCoord[0], 2) + pow(normalCoord[1], 2) + pow(normalCoord[2], 2));
    normalCoord[0] * apple;
    normalCoord[1] * apple;
    normalCoord[2] * apple;

    return true;
}
// Get three space-delimited floats from string.
bool STLReader::stlReadVertex(char* buf, float vertCoord[3])
{
    char* begptr = buf;
    char* endptr = nullptr;

    for (int i = 0; i < 3; ++i)
    {
        // We really should use: vertCoord[i] = std::strtof(begptr, &endptr);
        // instead of strtod below but Apple Clang 9.0.0.9000039 doesn't
        // recognize strtof as part of the C++11 standard
        //vertCoord[i] = static_cast<float>(std::strtod(begptr, &endptr))*2.0;//��ʱ�Ȱ�������ݷŴ�
        //if (i == 2)
        //{
        //    vertCoord[i] = vertCoord[i] - 10.0;//��ʱ��ʮ����֤����ϵ����
        //}


        vertCoord[m_xyz[i]] = static_cast<float>(std::strtod(begptr, &endptr))* m_scale[m_xyz[i]]+ m_offset[m_xyz[i]];//��ʱ�Ȱ�������ݷŴ�
    


        if (begptr == endptr)
        {
            return false;
        }

        begptr = endptr;
    }

    return true;
}

bool STLReader::LoadFile(const char* filename)
{
	

	bool ASCII = true;
	if (!ASCII)
	{
		return false;
	}

	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		DEBUG_LOG(_T(" can not open file:%s"), filename);
		return false;
	}

    char line[256];         // line buffer
    int solidId = -1;
    int vertOff = 0;
    int lineNum = 0;
    float vertCoord[3];     // scratch space when parsing "vertex %f %f %f"
    float normalCoord[3];     // scratch space when parsing "vertex %f %f %f"
    std::string header;
	enum StlAsciiScanState
	{
		scanSolid = 0,
		scanFacet,
		scanLoop,
		scanVerts,
		scanEndLoop,
		scanEndFacet,
		scanEndSolid
	};

    std::string errorMessage;
    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    int count = file_size / 180 * 3;//�Զ��ж�������������һ���������ַ���һ�����200��

    // mVertices.clear();
    // mNormals.clear();

    output = _tsptr<PolyData>(new PolyData);
   /* _tsptr<PolyData> polydata =  _tsptr<PolyData>(new PolyData);
    output = polydata;
    polydata->Begin(POLY_DATASHAPE_TRIANGLE_STRIP, count);*/
    for (StlAsciiScanState state = scanSolid; errorMessage.empty() == true; /*nil*/)
    {
        char* cmd = fgets(line, 255, fp);

        if (!cmd)
        {
            // fgets() failed (eg EOF).
            // If scanning for the next "solid" this is a valid way to exit,
            // but is an error if scanning for the initial "solid" or any other token

            switch (state)
            {
            case scanSolid:
            {
                // Emit error if EOF encountered without having read anything
                if (solidId < 0) errorMessage = stlParseEof("solid");
                break;
            }
            case scanFacet: { errorMessage = stlParseEof("facet"); break; }
            case scanLoop: { errorMessage = stlParseEof("outer loop"); break; }
            case scanVerts: { errorMessage = stlParseEof("vertex"); break; }
            case scanEndLoop: { errorMessage = stlParseEof("endloop"); break; }
            case scanEndFacet: { errorMessage = stlParseEof("endfacet"); break; }
            case scanEndSolid: { errorMessage = stlParseEof("endsolid"); break; }
            }

            // Terminate the parsing loop
            break;
        }
        // Cue to the first non-space.
        while (isspace(*cmd))
        {
            ++cmd;
        }

        // An empty line - try again
        if (!*cmd)
        {
            // Increment line-number, but not while still in the header
            if (lineNum) ++lineNum;
            continue;
        }

        // Ensure consistent case on the first token and separate from
   // subsequent arguments

        char* arg = cmd;
        while (*arg && !isspace(*arg))
        {
            *arg = tolower(*arg);
            ++arg;
        }

        // Terminate first token (cmd)
        if (*arg)
        {
            *arg = '\0';
            ++arg;

            while (isspace(*arg))
            {
                ++arg;
            }
        }

        ++lineNum;


        // Handle all expected parsed elements
        switch (state)
        {
        case scanSolid:
        {
            if (!strcmp(cmd, "solid"))
            {
                ++solidId;
                state = scanFacet;  // Next state
                if (!header.empty())
                {
                    header += "\n";
                }
                header += arg;
                // strip end-of-line character from the end
                while (!header.empty() && (header.back() == '\r' || header.back() == '\n'))
                {
                    header.pop_back();
                }
            }
            else
            {
                errorMessage = stlParseExpected("solid", cmd);
            }
            break;
        }
        case scanFacet:
        {
            if (!strcmp(cmd, "color"))
            {
                // Optional 'color' entry (after solid) - continue looking for 'facet'
                continue;
            }

            if (!strcmp(cmd, "facet"))
            {
                stlReadNormal(arg, normalCoord);
               
                state = scanLoop;  // Next state
            }
            else if (!strcmp(cmd, "endsolid"))
            {
                // Finished with 'endsolid' - find next solid
                state = scanSolid;
            }
            else
            {
                errorMessage = stlParseExpected("facet", cmd);
            }
            break;
        }
        case scanLoop:
        {
            if (!strcmp(cmd, "outer"))  // More pedantic => && !strcmp(arg, "loop")
            {
                state = scanVerts;  // Next state
            }
            else
            {
                errorMessage = stlParseExpected("outer loop", cmd);
            }
            break;
        }
        case scanVerts:
        {
            if (!strcmp(cmd, "vertex"))
            {
                if (stlReadVertex(arg, vertCoord))
                {
                    output->mVertices << QVector3D(vertCoord[0], vertCoord[1], vertCoord[2]);
                    output->mNormals << QVector3D(normalCoord[0], normalCoord[1], normalCoord[2]);
                    //polydata->insert_next_vert(vertCoord,normalCoord);
                    ++vertOff;  // Next vertex
                    if (vertOff >= 3)
                    {
                       
                       // Finished this triangle.
                       vertOff = 0;
                       state = scanEndLoop;  // Next state
                    }
                    //pts[vertOff] = newPts->InsertNextPoint(vertCoord);
                    //++vertOff;  // Next vertex

                    
                    //if (vertOff >= 3)
                    //{
                    //    // Finished this triangle.
                    //    vertOff = 0;
                    //    state = scanEndLoop;  // Next state

                    //    // Save as cell
                    //    newPolys->InsertNextCell(3, pts);
                    //    if (scalars)
                    //    {
                    //        scalars->InsertNextValue(solidId);
                    //    }

                    //    if ((newPolys->GetNumberOfCells() % 5000) == 0)
                    //    {
                    //        this->UpdateProgress((newPolys->GetNumberOfCells() % 50000) / 50000.0);
                    //    }
                    //}
                }
                else
                {
                    errorMessage = "Parse error reading STL vertex";
                }
            }
            else
            {
                errorMessage = stlParseExpected("vertex", cmd);
            }
            break;
        }
        case scanEndLoop:
        {
            if (!strcmp(cmd, "endloop"))
            {
                state = scanEndFacet;  // Next state
            }
            else
            {
                errorMessage = stlParseExpected("endloop", cmd);
            }
            break;
        }
        case scanEndFacet:
        {
            if (!strcmp(cmd, "endfacet"))
            {
                state = scanFacet;  // Next facet, or endsolid
            }
            else
            {
                errorMessage = stlParseExpected("endfacet", cmd);
            }
            break;
        }
        case scanEndSolid:
        {
            if (!strcmp(cmd, "endsolid"))
            {
                state = scanSolid;  // Start over again
            }
            else
            {
                errorMessage = stlParseExpected("endsolid", cmd);
            }
            break;
        }
        }
    }

    //polydata->End();
    if (!errorMessage.empty())
    {
        DEBUG_LOG("%s", errorMessage.c_str());
        /*vtkErrorMacro("STLReader: error while reading file "
            << this->FileName << " at line " << lineNum << ": "
            << errorMessage);*/
        return false;
    }

    return true;

}
