
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
/*  ���뷽ʽ
#define GUI_DT_LEFT					0x00000002			// �����
#define GUI_DT_RIGHT				0x00000004			// �Ҷ���
#define GUI_DT_TOP					0x00000008			// �϶���
#define	GUI_DT_BOTTOM				0x00000010			// �¶���
#define GUI_DT_VCENTER				0x00000020			// �������
#define GUI_DT_HCENTER				0x00000040			// �������
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
// Description : UIface�࣬����һ���Ķ���ļ������һ��Render Target��
//
	class CUIFace : public NiMemObject
	{
		// ================================
		//   local types
		// ================================
	protected :
        bool      m_bNeedUpdate ;                             // ���±�ǣ�ΪtrueʱUpdateFace��Ч��
		                                                      // Ϊfalseʱ�������Updateʱ�Ͳ���Update��
	public :


		// ================================
		//   data members
		// ================================
	protected :
		NiTriShapePtr                m_spFaceGeometry ;        //Ƭ�ļ����壬��һ���Ķ����Ƭ
		NiRenderedTexturePtr         m_spRenderTexture ;       //Render Target
		NiRenderTargetGroupPtr       m_spRenderTargetGroup ;   //����Render Target��Ⱦ��Group
		NiPoint3                     m_FaceNormal   ;          //Ƭ�ķ���
		std::vector<IUIFaceElement*> m_vecElements  ;          //���Ƭ����Ҫ���Ƶ�Element�ļ���

		unsigned int                 m_BackImageID  ;          //����ͼ��UIRenderer��ID����load����ͼƬ��ʱ������
		unsigned int                 m_nCurrentElementID ;     //��������ElementID�ı���
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
		// CUIFace�Ĺ��캯��������ֱ�����Ŀ�ȣ��߶ȣ�������ĵ㣬���ߣ�����ͼ�ļ���·����
		// ��ʱRenderTarget�Ĵ�С���Ǳ���ͼ�ļ��Ĵ�С��������Ϊ��λ
		CUIFace( float fFaceWidth, float fFaceHeight, NiPoint3 &faceCenterPoint, NiPoint3 &faceNormal, 
			     const char* pBackTexFile  ) ;
        
		// CUIFace�Ĺ��캯��������ֱ�����Ŀ�ȣ��߶ȣ�������ĵ㣬���ߣ�RenderTarget�����ؿ�ȣ��߶�
		CUIFace( float fFaceWidth, float fFaceHeight, NiPoint3 &faceCenterPoint, NiPoint3 &faceNormal, 
			     int nTexWidth, int nTexHeight ) ;

		//!!!!!!�����������캯����ֻ�ᴴ�����������Ὠ������
		
		// ͨ��ͼƬ�ļ�������RenderTarget
		CUIFace(const char* pBackTexFile) ;
        //ͨ�� �� �� �ߴ��� RenderTarget
	    CUIFace(int nTexWidth, int nTexHeight);

		~CUIFace() ;


		// ================================
		//   functions
		// ================================
	public :
		// Inherited Functions / Procedures


		/* Properties Get() / Set(...)*/

		// ��ElementID����ȡ��ָ����Face Element��ָ��
		IUIFaceElement* GetFaceElement(unsigned int hElementID) ;
		
		// �õ���ļ�����
		NiTriShapePtr GetGeometry() const;

		// �õ�Render Target������
        NiRenderedTexturePtr GetRenderTarget();
		
		// ����������ĵ�
		void SetCenterPoint( NiPoint3 &faceCenterPoint) ;


		// Core Functions / Procedures

		//�������Face����������RenderTarget�����õ�λ����ע�⣬������gb��BeginFrame��EndFrame֮�����
		void UpdateFace(bool bForece = false);

		//��Ⱦ���Face��
		void RenderFace(NiRenderer* pRender);

		//����Ƭ�ı���ͼ��������ͼ�ļ���·��
		void ChangeBackImage( const char* pBackTexFile) ;

       //���Render Target
		void ClearTexture() ;

		// ����һ��ͼƬ��Element��������һ��Element��ID��
		// ���������� ԭͼƬҪ���Ƶ�rc�� ��Ҫ�浽��Ļ���ǿ�ط���rc����ͼƬʱͼƬ��alphaֵ�� ͼƬ·����Element���ڵĲ�
		unsigned int CreateImageElement( RECT& rcSrc, RECT& rcDest, unsigned char byImageAlpha, 
			                             const char* lpszImageFileName, unsigned int nLayer = 1 ) ;

		// ����һ���ı���Element, ������һ��Element��ID
		// ���������ǻ��Ƶ��ִ������õ�����������֣��ı�����ɫ������Ŀ���rc���ü����������ü�����NULL�����ı��Ķ��뷽ʽ �����뿴FBGUIDef.h�ļ�
		unsigned int CreateTextElement(  const char* szText, const char* szFontName, DWORD dwTextColor , 
			                             RECT &rcDestRect, RECT *rcClip, unsigned int format, unsigned int nLayer = 1 )  ;
        
		// ����һ��Element��ͨ������ʱ��ID���á�
		void DestroyElement(unsigned int nElementID ) ;

		void MarkUpdate();


	protected:
		void _ConstructFace( float fFaceWidth, float fFaceHeight, NiPoint3 &faceCenterPoint, NiPoint3 &faceNormal);
		void _ConstructRenderTarget(unsigned int nTexWidth, unsigned int nTexHeight) ;

		unsigned int GenerateElementID();

		//���Element
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