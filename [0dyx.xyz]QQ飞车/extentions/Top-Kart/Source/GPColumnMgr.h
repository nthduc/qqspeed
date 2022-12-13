////////////////////////////////////////////////
//FileName:    GPColumnMgr.h
//Author:      RayHuang
//Date:        2007-01-25
//Description: 渲染道具栏等元素
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


	//使用交换器，个数减一
	void UseExchanger();
	
	//得到道具，压入道具图片
//	void PushGPImage(unsigned int unPropID);
	
	//使用道具，弹出道具图片
//	void PopGPImage();

	//设置交换器的个数
	void SetSwapperNum(int nNumOfswapper);

	//绘制道具栏和交换器
	virtual void Draw(NiRendererPtr spRenderer);

	//更新
	virtual void Update(float fTime);
/*
	//关闭道具栏
	void ToggleColumn(bool bState);
*/

	//开关交换器
	void ToggleSwappper(bool bState);

protected:
	//加入一矩形元素
	void AddScreenRect( NiScreenElementsPtr &spElt, NiSourceTexturePtr spTex, 
								float fLeft, float fTop, float fWidth, float fHeight);

	//更换界面元素的纹理
	void ChangeElementTex(NiScreenElementsPtr spElt, NiSourceTexturePtr spTex);


	//交换道具图片
//	void SwapGPImage();
	
private:
	NiScreenElementsPtr m_spGPColumnBase;					//道具栏的界面元素
	NiScreenElementsPtr m_aspGPScnElt[MAX_OWNGP_NUM];		//道具的界面元素
	NiScreenElementsPtr m_spSwaperElt;						//交换器的界面元素
	NiString2DPtr		m_spNumOfSwapper;					//交换器的个数显示

	UIntKeyToTextureMap	m_mapAllGPTex;						//道具的图片

//	unsigned int		m_aunPropID[MAX_OWNGP_NUM];

	unsigned int		m_unCurOwnPropsNum;					//当前拥有道具数

	unsigned int		m_unCurExchangerNum;				//当前拥有交换器数目

//	bool				m_bIsColumnActive;					//道具栏是否显示

	bool				m_bIsSwapperActive;					//交换器是否显示
};