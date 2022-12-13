#include "Top-KartPCH.h"
#include "GPColumnMgr.h"
#include "variant.h"
#include "stringex.h"
#include "common.h"
#include "NiSourceTexture.h"

#include "./Platform/GlobalInterface.h"
#include "gpdefine.h"
#include "gpcontrol.h"
#include "gptarget.h"

#include "GameMain.h"

CGPColumnMgr::CGPColumnMgr()
{
	//载入道具栏图像配置
	CVarTable vtImageCfg("GPImageConfig");
	CVar::LoadVarsFromFile( &vtImageCfg, "config/gp_image_cfg.lua");
	
	NiFixedString strImagePath			= vtImageCfg.Element("ImagePath")->String();
	NiFixedString strColumnImageName	= vtImageCfg.Element("columnPicName")->String();
	NiFixedString strSwapperImageName	= vtImageCfg.Element("swapperPicName")->String();
	NiFixedString strFontName			= vtImageCfg.Element("FontName")->String();


	CVarArray* arrImage	= (CVarArray*) vtImageCfg.Element("gpImageMap");
	
	NiSourceTexturePtr spTex;
	
	CStringEx strTmp;

	int nNumOfGPImage = arrImage->GetNum();
	for(int i = 0; i < nNumOfGPImage; ++i)		//载入各道具图像
	{
		spTex = NiSourceTexture::Create( NiApplication::ConvertMediaFilename( strTmp.FormatPt("%s/%s",
							strImagePath, arrImage->Element(i)->Element("name")->String() ) ) );
		if(spTex)
		{
			m_mapAllGPTex.insert( make_pair( arrImage->Element(i)->Element("prop_id")->Integer(), spTex ) );			
		}
	}


	//从服务器得到交换器的个数
	m_unCurExchangerNum = 15;

	m_unCurOwnPropsNum	= 0;

//	m_bIsColumnActive = true;
	m_bIsSwapperActive = true;

	//增加道具栏界面元素
	spTex = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(
					strTmp.FormatPt("%s/%s", strImagePath, strColumnImageName ) ) );
	AddScreenRect(m_spGPColumnBase,	spTex, 0.02, 0.03,
				(float)spTex->GetWidth()/GameMain::GetScreenWidth(),
				(float)spTex->GetHeight()/GameMain::GetScreenHeight());

	//增加交换器界面元素
	spTex = NiSourceTexture::Create(NiApplication::ConvertMediaFilename( 
					strTmp.FormatPt("%s/%s",strImagePath, strSwapperImageName ) ) );
	AddScreenRect(m_spSwaperElt, spTex, 0.22, 0.063,
				(float)spTex->GetWidth()/GameMain::GetScreenWidth(),
				(float)spTex->GetHeight()/GameMain::GetScreenHeight());
	
	//增加道具显示点1
	AddScreenRect(m_aspGPScnElt[0],	NULL,  0.035, 0.062, 
				54.0/GameMain::GetScreenWidth(),
				40.0/GameMain::GetScreenHeight());

	//增加道具显示点2
	AddScreenRect(m_aspGPScnElt[1],	NULL, 0.116, 0.057, 
				69.0/GameMain::GetScreenWidth(), 
				51.0/GameMain::GetScreenHeight() );

	NiFontPtr spFont = NiFont::Create( NiRenderer::GetRenderer(),NiApplication::
				ConvertMediaFilename(strTmp.FormatPt("%s/%s", strImagePath, strFontName)));

	m_spNumOfSwapper = NiNew NiString2D(spFont, NiFontString::COLORED, 
						32, "", NiColorA(1.0f, 1.0f, 1.0f, 0.6f), 315, 70);
	
	SetSwapperNum(m_unCurExchangerNum);	//初始化交换器个数
	
}

CGPColumnMgr::~CGPColumnMgr()
{
	
	m_spGPColumnBase = 0;
	m_spNumOfSwapper = 0;
	m_spSwaperElt    = 0;
	m_aspGPScnElt[0] = 0;
	m_aspGPScnElt[1] = 0;

	UIntKeyToTextureMap::iterator map_it;	//依次施放tex
	for( map_it = m_mapAllGPTex.begin(); map_it != m_mapAllGPTex.end(); map_it++)
	{
		map_it->second = 0;
	}
	
}


//加入一矩形元素
void CGPColumnMgr::AddScreenRect( NiScreenElementsPtr &spElt, NiSourceTexturePtr spTex, 
											float fLeft, float fTop, float fWidth, float fHeight)
{
	NiTexturingPropertyPtr  pkTexPro	= NiNew NiTexturingProperty();
	NiAlphaPropertyPtr		pkAlphaPro	= NiNew NiAlphaProperty();

	pkTexPro->SetBaseTexture(spTex);
	pkTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	pkTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

	pkAlphaPro->SetAlphaBlending(true);
	pkAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	pkAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

	spElt = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
	spElt->Insert(4);


	spElt->SetRectangle(0, fLeft, fTop, fWidth, fHeight);

	spElt->SetTextures(0, 0, 0, 0, 1, 1);

	spElt->AttachProperty(pkTexPro);
	spElt->AttachProperty(pkAlphaPro);

	spElt->UpdateBound();
	spElt->UpdateProperties();
	spElt->UpdateEffects();
	spElt->Update(0.0f);
}


//更换界面元素的纹理
void CGPColumnMgr::ChangeElementTex(NiScreenElementsPtr spElt, NiSourceTexturePtr spTex)
{
	NiTexturingPropertyPtr spTexPro = NiDynamicCast(NiTexturingProperty, spElt->GetProperty(NiProperty::TEXTURING));

	spTexPro->SetBaseTexture(spTex);

	spElt->UpdateProperties();
	spElt->UpdateEffects();
	spElt->Update(0);	
}

/*
//得到道具，压入道具图片，传入道具ID
void CGPColumnMgr::PushGPImage(unsigned int unPropID)
{
	if( m_unCurOwnPropsNum == 1 )	//判断tex应放入的矩形元素
	{
		ChangeElementTex(m_aspGPScnElt[1], m_mapAllGPTex[unPropID]);
	}
	else if( m_unCurOwnPropsNum == 2)
	{
		ChangeElementTex(m_aspGPScnElt[0], m_mapAllGPTex[unPropID]);
	}
}


//使用道具，弹出道具图片
void CGPColumnMgr::PopGPImage()
{
	if(m_unCurOwnPropsNum == 2)
	{
		ChangeElementTex(m_aspGPScnElt[1], m_mapAllGPTex[ m_aunPropID[0] ]);
		ChangeElementTex(m_aspGPScnElt[0], NULL);
		m_aunPropID[0] = 0;
		m_unCurOwnPropsNum--;
	}
	else if(m_unCurOwnPropsNum == 1)
	{
		ChangeElementTex(m_aspGPScnElt[1], NULL);
		m_aunPropID[1] = 0;
		m_unCurOwnPropsNum--;
	}
	
}
*/

//交换道具图片
/*void CGPColumnMgr::SwapGPImage()
{
	if(m_unCurOwnPropsNum == 2)
	{
		ChangeElementTex( m_aspGPScnElt[0], m_mapAllGPTex[ m_aunPropID[1] ] );
		ChangeElementTex( m_aspGPScnElt[1], m_mapAllGPTex[ m_aunPropID[0] ] );
		Swap( m_aunPropID[0], m_aunPropID[1] );
	}
}
*/

//设置交换器的个数
void CGPColumnMgr::SetSwapperNum(int nNumOfswapper)
{
	m_spNumOfSwapper->sprintf("%d", nNumOfswapper);
}


//使用交换器，个数减一
void CGPColumnMgr::UseExchanger()
{
	if(!m_bIsSwapperActive)
	{
		return;
	}
	IGPTarget* pHost = (IGPTarget*) NxPlayMgr::GetHostPlay();
	if ( !pHost )
	{
		return;
	}
	GP_ID_LIST& listGPID = pHost->GetGPs();  
	if(m_unCurExchangerNum > 0 && listGPID.size() == 2)//当前交换器个数是否为零，并且道具是否有两个
	{
		m_unCurExchangerNum--;	//交换器个数减一

		//交换道具位置
		GP_ID_LIST_Iterator id_it = listGPID.begin();
		GP_ID kGPID1 = *id_it;
		GP_ID kGPID2 = *(++id_it);
		listGPID.clear();
		listGPID.push_back(kGPID2);
		listGPID.push_back(kGPID1);

		SetSwapperNum(m_unCurExchangerNum);
		
	}
}


//绘制道具栏和交换器
void CGPColumnMgr::Draw(NiRendererPtr spRenderer)
{
	m_spGPColumnBase->Draw(spRenderer);	
/*	if(m_bIsSwapperActive)
	{
		m_spSwaperElt->Draw(spRenderer);
		m_spNumOfSwapper->Draw(spRenderer);
	}	
*/	if(m_unCurOwnPropsNum == 1)		//判断需要渲染的矩形
	{
		m_aspGPScnElt[1]->Draw(spRenderer);
	}
	else if(m_unCurOwnPropsNum >= 2)
	{
		m_aspGPScnElt[0]->Draw(spRenderer);
		m_aspGPScnElt[1]->Draw(spRenderer);
	}	
}
/*
void CGPColumnMgr::ToggleColumn(bool bState)
{
	m_bIsColumnActive = bState;
}
*/
void CGPColumnMgr::ToggleSwappper(bool bState)
{
	m_bIsSwapperActive = bState;
}

//更新
void CGPColumnMgr::Update(float fTime)
{
	if( NxPlayMgr::GetHostPlay() )
	{
		GP_ID_LIST& listGPID = NxPlayMgr::GetHostPlay()->GetGPs();
		GP_ID_LIST_Iterator ID_It = listGPID.begin();

		m_unCurOwnPropsNum = listGPID.size();

		int i = 1;
		for(; ID_It != listGPID.end(); ID_It++)
		{			
			ChangeElementTex(m_aspGPScnElt[i], m_mapAllGPTex[(*ID_It).dwID]);
			--i;
			if(i < 0)
			{
				break;
			}
		}	
	}
}