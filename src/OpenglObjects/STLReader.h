#pragma once

class PolyData:public yObject
{
public:
	PolyData(void){};
	virtual ~PolyData(void){};

    QVector<QVector3D> mVertices;
    QVector<QVector3D> mNormals;
};

class STLReader :public yModule
{
public:
	STLReader();
	~STLReader();

    RESULT doSlot(_tsptr<yCMD> cmd)override;
    RESULT load(_tsptr<yCMD>  cmd);


    void setParam(int xyz[3], float offset[3], float scale[3]);
    virtual bool LoadFile(const char* filename);
    _tsptr<PolyData> output;
    // QVector<QVector3D> mVertices;
    // QVector<QVector3D> mNormals;
    
protected:
    bool stlReadVertex(char* buf, float vertCoord[3]);
    bool stlReadNormal(char* buf, float  normalCoord[3]);

    int m_xyz[3];
    float m_offset[3];
    float m_scale[3];   
    float m_modified_time = 0.0;
};