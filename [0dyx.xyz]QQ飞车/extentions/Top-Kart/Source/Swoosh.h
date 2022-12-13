////////////////////////////////////////////////
//FileName:    Swoosh.h
//Author:      PigHuang
//Date:        2007-01-16
//Description: 本特效改自FullDemos里的Eturnum例子，需要包括shader程序swoosh.nsb
//				通过client.ini的CarSwoosh开关控制，effect\swoosh.lua设置参数
////////////////////////////////////////////////
#ifndef SWOOSH_H
#define SWOOSH_H

class NiTriShape;
class NiAVObject;
class NiNode;
class NiGeometryGroup;

#include <NiPoint3.h>
#include <NiPoint2.h>
#include "RenderBase.h"

#define NUM_QUADS_LIMITE 75 //可以容纳的最大贴图矩阵数

class Swoosh : public NiMemObject, public CRenderBase3D
{
public:
    Swoosh(NiAVObject* pkTop, float fInitialTime);
	~Swoosh();
    //Sample points for the next quad.
    void TakeSample(float fTime);
	//绘制
	virtual void Draw(NiRendererPtr spRenderer);
	//更新
	virtual void Update(float fTime);

    //These methods are needed by the app to allow alt-tab to work correctly.
    NiGeometryGroup* GetGeometryGroup(){return m_pkGeomGroup;};
    NiGeometryData* GetSwooshData(){return m_pkTriShape->GetModelData();};
    NiTriShape* m_pkTriShape;

protected:
    //Circular arrays to hold the data for repacking each frame.
    NiPoint3* m_kVertices;
    float* m_afVelocityScales;
    NiPoint2* m_kTexCoords;
    int m_iStartingIndex;
    int m_iNumberOfQuads;
    NiAVObject* m_pkTopObject;

    //Values needed to calculate the velocity/opacity scaling.
    float m_fLastTime;
    NiPoint3 m_kLastPoint;
	//特效调节参数
    float	m_fVelocityMinimum;	///产生swoosh效果的最低速度阀值
    float	m_fVelocityMaximum;	///产生swoosh效果的最高速度阀值
	float	m_fWidthRef;		///swoosh宽度的修正参数
	UINT	m_nQuadCount;		///swoosh效果最大的矩形贴图单元总数
    NiGeometryGroup* m_pkGeomGroup;
};

#endif