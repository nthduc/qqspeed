
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
	// Description : face��Element�Ļ��࣬��������Text ��Image��Element
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
		unsigned int m_nElementID ;             //��Element��ID���ɼ���UIFace��ʱ��õ�

	public :
		unsigned int m_nLayer     ;             //���Element����layer��Layer��ʾ���Ƶ�˳��

		// ================================
		//   static or const data members
		// ================================
	protected :
			static unsigned int s_ClassID ;    //�����Element��ClassID��������ʶ��������������ʲô�࣬
			                                   //����ɿ�ENUM_FACE_ELEMENT_CLASSID
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
		virtual void DrawElement() = 0;           //����Element      
		virtual unsigned int GetClassID() = 0 ;   //�õ�ClassID
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
	// Description : Text��Element��
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
		std::string  m_strContent  ;               // ���text��Ҫ���Ƶ��ִ�    
		std::string  m_strFontName ;               // ���text���õ������������
		DWORD        m_dwTextColor ;               // �ı�����ɫ
        RECT         m_rcDestRect  ;               // ����Ŀ���rc
		RECT         *m_rcClip      ;              // �ü����������ü�����NULL
		unsigned int m_Format      ;               // �ı��Ķ��뷽ʽ �����뿴FBGUIDef.h�ļ�


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
		
		//����ĸ߶ȣ�������Ϊ��λ
		unsigned int GetTextHeight();

        //�����ִ�����Ļ�ϵĳ��ȣ�������Ϊ��λ
		unsigned int GetTextWidth();               

		// Core Functions / Procedures

		// ================================
		//   static functions
		// ================================
	protected :

	public :

	};



	//
	// Description : Image Element��
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
		unsigned int  m_ImageID ;               // ͼƬ��UIRenderer���������ID

	public :
		RECT          m_rcScr ;                 // ԭͼƬҪ���Ƶ�rc��Ҳ����˵Ҫ��ԭͼƬ�浽��Ļ�ϵ�����
		RECT          m_rcDest ;                // Ŀ���rc��Ҳ������Ҫ�浽��Ļ�ϵ��ǿ�ط�
		unsigned char m_byImageAlpha ;          // ��ͼƬʱ��ͼƬ��alphaֵ�����ֻ��Ӱ����ʾ����Ӱ��ͼƬ���������


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

		//���캯������������ΪԭͼƬҪ���Ƶ�rc����Ҫ�浽��Ļ�ϵ�rc�� ����ʱͼƬ��alphaֵ��ͼƬ�ļ���·�������Ƶ�layer
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

		// �õ�ͼƬ�Ŀ��
		unsigned int GetImageWidth()  const  ;

		// �õ�ͼƬ�ĸ߶�
		unsigned int GetImageHeight() const ;
		// Core Functions / Procedures



		// ================================
		//   static functions
		// ================================
	protected :

	public :

	};



}