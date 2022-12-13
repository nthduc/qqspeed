////////////////////////////////////////////////
//FileName:    GPColumnMgr.h
//Author:      RayHuang
//Date:        2007-01-25
//Description: ��Ⱦ��������Ԫ��
////////////////////////////////////////////////

#pragma once

#include <map>
#include <NiFont.h>
#include <NiString2D.h>
#include "RenderBase.h"

using namespace std;

const unsigned int MAX_OWNGP_NUM = 2;	

typedef map<unsigned int, NiSourceTexturePtr> UIntKeyToTextureMap;

class CGPColumnMgr : public NiMemObject, public CRenderBase2D
{
public:

	CGPColumnMgr();
	~CGPColumnMgr();


	//ʹ�ý�������������һ
	void UseExchanger();
	
	//�õ����ߣ�ѹ�����ͼƬ
//	void PushGPImage(unsigned int unPropID);
	
	//ʹ�õ��ߣ���������ͼƬ
//	void PopGPImage();

	//���ý������ĸ���
	void SetSwapperNum(int nNumOfswapper);

	//���Ƶ������ͽ�����
	virtual void Draw(NiRendererPtr spRenderer);

	//����
	virtual void Update(float fTime);
/*
	//�رյ�����
	void ToggleColumn(bool bState);
*/

	//���ؽ�����
	void ToggleSwappper(bool bState);

protected:
	//����һ����Ԫ��
	void AddScreenRect( NiScreenElementsPtr &spElt, NiSourceTexturePtr spTex, 
								float fLeft, float fTop, float fWidth, float fHeight);

	//��������Ԫ�ص�����
	void ChangeElementTex(NiScreenElementsPtr spElt, NiSourceTexturePtr spTex);


	//��������ͼƬ
//	void SwapGPImage();
	
private:
	NiScreenElementsPtr m_spGPColumnBase;					//�������Ľ���Ԫ��
	NiScreenElementsPtr m_aspGPScnElt[MAX_OWNGP_NUM];		//���ߵĽ���Ԫ��
	NiScreenElementsPtr m_spSwaperElt;						//�������Ľ���Ԫ��
	NiString2DPtr		m_spNumOfSwapper;					//�������ĸ�����ʾ

	UIntKeyToTextureMap	m_mapAllGPTex;						//���ߵ�ͼƬ

//	unsigned int		m_aunPropID[MAX_OWNGP_NUM];

	unsigned int		m_unCurOwnPropsNum;					//��ǰӵ�е�����

	unsigned int		m_unCurExchangerNum;				//��ǰӵ�н�������Ŀ

//	bool				m_bIsColumnActive;					//�������Ƿ���ʾ

	bool				m_bIsSwapperActive;					//�������Ƿ���ʾ
};