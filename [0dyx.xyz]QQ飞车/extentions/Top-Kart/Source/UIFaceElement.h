
// ===============================================================================
// UIFaceElement.H 
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
// Copyright Tencent 1.2007
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
#include <string>
#include <NiSystem.h>
// Project headers: 
#include "IUIRenderer.h"

// ============================================================================== 
// Consts & Macros 
// ============================================================================== 

// ============================================================================== 
// Types & Structures 
// ============================================================================== 


// ============================================================================== 
// Classes 
// ============================================================================== 

namespace GFX
{

	enum ENUM_FACE_ELEMENT_CLASSID
	{
		CLASSID_UNKNOWN = 0 ,
		CLASSID_TEXT    = 1 ,
		CLASSID_IMAGE       
	};

	//
	// Description : face上Element的基类，可派生出Text 和Image的Element
	//
	class IUIFaceElement : public NiMemObject
	{
        friend class CUIFace ;
		// ================================
		//   local types
		// ================================
	protected :

	public :

		// ================================
		//   data members
		// ================================
	protected :
		unsigned int m_nElementID ;             //该Element的ID，由加入UIFace的时候得到

	public :
		unsigned int m_nLayer     ;             //这个Element所在layer，Layer表示绘制的顺序

		// ================================
		//   static or const data members
		// ================================
	protected :
			static unsigned int s_ClassID ;    //这个是Element的ClassID，用来标识派生出来的类是什么类，
			                                   //具体可看ENUM_FACE_ELEMENT_CLASSID
	public :

		// ================================
		//   constructor / destructor
		// ================================
	public :
		IUIFaceElement(unsigned int nLayer = 1) 
			:  m_nLayer(nLayer), m_nElementID(0xffffffff)
		{
		}
		virtual ~IUIFaceElement()
		{
		}

		// ================================
		//   functions
		// ================================
	public :
		// Inherited Functions / Procedures
		virtual void DrawElement() = 0;           //绘制Element      
		virtual unsigned int GetClassID() = 0 ;   //得到ClassID
		// Properties Get() / Set(...)

		// Core Functions / Procedures
		virtual unsigned int GetElementID() const
		{
			return m_nElementID ;
		}

		// ================================
		//   static functions
		// ================================
	protected :

	public :

	};



	//
	// Description : Text的Element类
	//
	class CUIFaceTextElement : public IUIFaceElement
	{
        friend class CUIFace ;

		// ================================
		//   local types
		// ================================
	protected :

	public :


		// ================================
		//   data members
		// ================================
	protected :

	public :
		std::string  m_strContent  ;               // 这个text所要绘制的字串    
		std::string  m_strFontName ;               // 这个text所用到的字体的名字
		DWORD        m_dwTextColor ;               // 文本的颜色
        RECT         m_rcDestRect  ;               // 绘制目标的rc
		RECT         *m_rcClip      ;              // 裁剪框，如果不需裁减，置NULL
		unsigned int m_Format      ;               // 文本的对齐方式 具体请看FBGUIDef.h文件


		// ================================
		//   static or const data members
		// ================================
	protected :
		static unsigned int s_ClassID ;
	public :


		// ================================
		//   constructor / destructor
		// ================================
	public :

		CUIFaceTextElement( const char* szText, const char* szFontName, DWORD dwTextColor , RECT &rcDestRect,
			                RECT *rcClip, unsigned int format, unsigned int nLayer = 1 );
		~CUIFaceTextElement()
		{
		}

		// ================================
		//   functions
		// ================================
	public :
		// Inherited Functions / Procedures
		
		virtual void DrawElement() ;
        virtual unsigned int GetClassID() ;

		// Properties Get() / Set(...)
		
		//字体的高度，以象素为单位
		unsigned int GetTextHeight();

        //整个字串在屏幕上的长度，以象素为单位
		unsigned int GetTextWidth();               

		// Core Functions / Procedures

		// ================================
		//   static functions
		// ================================
	protected :

	public :

	};



	//
	// Description : Image Element类
	//
	class CUIFaceImageElement : public IUIFaceElement
	{
		friend class CUIFace ;
		// ================================
		//   local types
		// ================================
	protected:
	public:

		// ================================
		//   data members
		// ================================
	protected :
		unsigned int  m_ImageID ;               // 图片在UIRenderer缓冲区里的ID

	public :
		RECT          m_rcScr ;                 // 原图片要绘制的rc，也就是说要把原图片绘到屏幕上的区域
		RECT          m_rcDest ;                // 目标的rc，也就是所要绘到屏幕上的那块地方
		unsigned char m_byImageAlpha ;          // 绘图片时，图片的alpha值，这个只会影响显示，不影响图片本身的内容


		// ================================
		//   static or const data members
		// ================================
	protected :
		static unsigned int s_ClassID ;
	public :


		// ================================
		//   constructor / destructor
		// ================================
	public :

		//构造函数，参数依次为原图片要绘制的rc，所要绘到屏幕上的rc， 绘制时图片的alpha值，图片文件的路径，绘制的layer
		CUIFaceImageElement( RECT& rcSrc, RECT& rcDest, 
			                 unsigned char byImageAlpha, const char* lpszImageFileName, unsigned int nLayer = 1);
		~CUIFaceImageElement();


		// ================================
		//   functions
		// ================================
	public :
		// Inherited Functions / Procedures
		virtual void DrawElement() ;
		virtual unsigned int GetClassID();

		// Properties Get() / Set(...)

		// 得到图片的宽度
		unsigned int GetImageWidth()  const  ;

		// 得到图片的高度
		unsigned int GetImageHeight() const ;
		// Core Functions / Procedures



		// ================================
		//   static functions
		// ================================
	protected :

	public :

	};



}