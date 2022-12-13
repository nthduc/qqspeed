////////////////////////////////////////////////
//FileName:    Swoosh.h
//Author:      PigHuang
//Date:        2007-01-16
//Description: ����Ч����FullDemos���Eturnum���ӣ���Ҫ����shader����swoosh.nsb
//				ͨ��client.ini��CarSwoosh���ؿ��ƣ�effect\swoosh.lua���ò���
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

#define NUM_QUADS_LIMITE 75 //�������ɵ������ͼ������

class Swoosh : public NiMemObject, public CRenderBase3D
{
public:
    Swoosh(NiAVObject* pkTop, float fInitialTime);
	~Swoosh();
    //Sample points for the next quad.
    void TakeSample(float fTime);
	//����
	virtual void Draw(NiRendererPtr spRenderer);
	//����
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
	//��Ч���ڲ���
    float	m_fVelocityMinimum;	///����swooshЧ��������ٶȷ�ֵ
    float	m_fVelocityMaximum;	///����swooshЧ��������ٶȷ�ֵ
	float	m_fWidthRef;		///swoosh��ȵ���������
	UINT	m_nQuadCount;		///swooshЧ�����ľ�����ͼ��Ԫ����
    NiGeometryGroup* m_pkGeomGroup;
};

#endif