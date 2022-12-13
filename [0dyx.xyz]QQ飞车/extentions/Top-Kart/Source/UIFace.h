
// ===============================================================================
// UIFace.H 
// 
// Purpose   : See Header
// 
// Latest Design doc link: http://
//
// 
// Author: Pucca Liang(mailto:puccaliang@tencent.com) 
// created:     1 2, 2007   10:32
// filename:    
// file path:   
// file base:   
// file ext:    .h
// 
// Copyright Tencent 2.2007
// ============================================================================== 
// <History> 
// 
// ==============================================================================
// <QA> 
//  TODO: 
// ============================================================================== 

# pragma once 

// ============================================================================== 
// includes 
// ============================================================================== 
// System headers: 
#include <vector>
#include <NiPoint3.h>
#include <NiTriShape.h>
#include <NiRenderedTexture.h>
#include <NiRenderTargetGroup.h>
#include <NiDX9Renderer.h>
#include <NiSystem.h>

// Project headers: 
#include "UIFaceElement.h"

// ============================================================================== 
// Consts & Macros 
// ============================================================================== 
/*  对齐方式
#define GUI_DT_LEFT					0x00000002			// 左对齐
#define GUI_DT_RIGHT				0x00000004			// 右对齐
#define GUI_DT_TOP					0x00000008			// 上对齐
#define	GUI_DT_BOTTOM				0x00000010			// 下对齐
#define GUI_DT_VCENTER				0x00000020			// 纵向居中
#define GUI_DT_HCENTER				0x00000040			// 横向居中
*/

// ============================================================================== 
// Types & Structures 
// ============================================================================== 


// ============================================================================== 
// Classes 
// ============================================================================== 

namespace GFX
{
//
// Description : UIface类，包括一个四顶点的几何体和一张Render Target，
//
	class CUIFace : public NiMemObject
	{
		// ================================
		//   local types
		// ================================
	protected :
        bool      m_bNeedUpdate ;                             // 更新标记，为true时UpdateFace生效，
		                                                      // 为false时，则调用Update时就不再Update了
	public :


		// ================================
		//   data members
		// ================================
	protected :
		NiTriShapePtr                m_spFaceGeometry ;        //片的几何体，是一个四顶点的片
		NiRenderedTexturePtr         m_spRenderTexture ;       //Render Target
		NiRenderTargetGroupPtr       m_spRenderTargetGroup ;   //用于Render Target渲染的Group
		NiPoint3                     m_FaceNormal   ;          //片的法线
		std::vector<IUIFaceElement*> m_vecElements  ;          //这个片上所要绘制的Element的集合

		unsigned int                 m_BackImageID  ;          //背景图在UIRenderer的ID，由load背景图片的时候生成
		unsigned int                 m_nCurrentElementID ;     //用来产生ElementID的变量
	public :



		// ================================
		//   static or const data members
		// ================================
	protected :

	public :



		// ================================
		//   constructor / destructor
		// ================================
	public :
		// CUIFace的构造函数，按序分别是面的宽度，高度，面的中心点，法线，背景图文件的路径。
		// 此时RenderTarget的大小就是背景图文件的大小，以象素为单位
		CUIFace( float fFaceWidth, float fFaceHeight, NiPoint3 &faceCenterPoint, NiPoint3 &faceNormal, 
			     const char* pBackTexFile  ) ;
        
		// CUIFace的构造函数，按序分别是面的宽度，高度，面的中心点，法线，RenderTarget的象素宽度，高度
		CUIFace( float fFaceWidth, float fFaceHeight, NiPoint3 &faceCenterPoint, NiPoint3 &faceNormal, 
			     int nTexWidth, int nTexHeight ) ;

		//!!!!!!下面两个构造函数是只会创建纹理，而不会建几何体
		
		// 通过图片文件来创建RenderTarget
		CUIFace(const char* pBackTexFile) ;
        //通过 宽 和 高创建 RenderTarget
	    CUIFace(int nTexWidth, int nTexHeight);

		~CUIFace() ;


		// ================================
		//   functions
		// ================================
	public :
		// Inherited Functions / Procedures


		/* Properties Get() / Set(...)*/

		// 按ElementID来获取该指定的Face Element的指针
		IUIFaceElement* GetFaceElement(unsigned int hElementID) ;
		
		// 得到面的几何体
		NiTriShapePtr GetGeometry() const;

		// 得到Render Target的纹理
        NiRenderedTexturePtr GetRenderTarget();
		
		// 设置面的中心点
		void SetCenterPoint( NiPoint3 &faceCenterPoint) ;


		// Core Functions / Procedures

		//更新这个Face，包括更新RenderTarget，调用的位置需注意，必须在gb的BeginFrame和EndFrame之间调用
		void UpdateFace(bool bForece = false);

		//渲染这个Face，
		void RenderFace(NiRenderer* pRender);

		//更换片的背景图，参数是图文件的路径
		void ChangeBackImage( const char* pBackTexFile) ;

       //清空Render Target
		void ClearTexture() ;

		// 创建一个图片的Element，并返回一个Element的ID。
		// 参数依次是 原图片要绘制的rc， 所要绘到屏幕上那块地方的rc，绘图片时图片的alpha值， 图片路径，Element所在的层
		unsigned int CreateImageElement( RECT& rcSrc, RECT& rcDest, unsigned char byImageAlpha, 
			                             const char* lpszImageFileName, unsigned int nLayer = 1 ) ;

		// 创建一个文本的Element, 并返回一个Element的ID
		// 参数依次是绘制的字串，所用到的字体的名字，文本的颜色，绘制目标的rc，裁剪框（如果不需裁减，置NULL），文本的对齐方式 具体请看FBGUIDef.h文件
		unsigned int CreateTextElement(  const char* szText, const char* szFontName, DWORD dwTextColor , 
			                             RECT &rcDestRect, RECT *rcClip, unsigned int format, unsigned int nLayer = 1 )  ;
        
		// 销毁一个Element，通过创建时的ID来用。
		void DestroyElement(unsigned int nElementID ) ;

		void MarkUpdate();


	protected:
		void _ConstructFace( float fFaceWidth, float fFaceHeight, NiPoint3 &faceCenterPoint, NiPoint3 &faceNormal);
		void _ConstructRenderTarget(unsigned int nTexWidth, unsigned int nTexHeight) ;

		unsigned int GenerateElementID();

		//添加Element
		int  AddFaceElement(IUIFaceElement *pElement) ;

		void _SortElementsByLayerID();

		// ================================
		//   static functions
		// ================================
	protected :

	public :

	};



// ============================================================================== 
// Functions 
// ============================================================================== 



}