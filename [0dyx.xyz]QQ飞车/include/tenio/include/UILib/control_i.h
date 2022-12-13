#ifndef __CONTROL_I_H__
#define __CONTROL_I_H__

#include "../teniointerface.h"
#include "../june/canvas_i.h"
#include "variableparam_i.h"

namespace Tenio
{
	class IControl;

	/**
	 * @ingroup tenuilib
	 * @brief �ؼ�����
	 *
	 * <p>
	 *���� ���пؼ��Ļ��࣬�����ؼ��Ļ�����Ա������
	 * </p>
	 *
	 * @sa 
	 */	
	class IControl : public IComponent
	{
	public:
		/**
		 *  @brief �����ؼ����˺�����uilib���ã����������û�ֱ�ӵ��á�
		 *  @param lpszControlName���û�Ϊ�ؼ�ȡ�����ƣ�������һ�� IUILib ��������ڲ���Ψһ��
		 *  @param pParent ָ�����ؼ�
		 *  @return �����ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa IUILib::CreateControl()
		 */	
		virtual BOOL Create(LPCTSTR lpszControlName, IControl* pParent) = 0;

		/**
		 *  @brief ���ÿؼ��ĸ��ؼ�
		 *  @param pControl ָ�����ؼ�
		 *  @sa 
		 */
		virtual void SetParent(IControl* pControl) = 0;

		/**
		 *  @brief ���ÿؼ�����
		 *  @param pParam ���Զ��󣬱���ؼ���һ����������ֵ
		 *  @sa 
		 */	
		virtual void SetProperty(IVariableParam* pParam) = 0;

		/**
		 *  @brief ��õø��ؼ�ָ��
		 *  @return ���ؼ�ָ�룬���û�и��ؼ����򷵻�NULL
		 *  @sa 
		 */	
		virtual IControl* GetParent() = 0;

		/**
		 *  @brief ��ÿؼ�ID
		 *  @return ���ؿؼ�ID����ЧIDֵΪ INVALID_CONTROLID
		 *  @sa 
		 */
		virtual DWORD GetControlID() = 0;

		/**
		 *  @brief ��ÿؼ��ľ��δ�С
		 *  @param bScreen �Ƿ񷵻���Ļ����ľ���
		 *  @return ���bScreenΪTRUE���򷵻���Ļ������Σ����bScreenΪFALSE���򷵻�����ڸ��ؼ�����ľ���
		 *  @sa 
		 */
		virtual RECT GetRect(BOOL bScreen) = 0;

		/**
		 *  @brief �ؼ��Ƿ�ɼ�
		 *  @return �ؼ��ɼ��򷵻�TRUE�����ɼ��򷵻�FALSE
		 *  @sa 
		 */	
		virtual BOOL IsVisible() = 0;

		/**
		 *  @brief �ؼ��Ƿ����
		 *  @return �ؼ������򷵻�TRUE�����ɼ��򷵻�TRUE
		 *  @sa 
		 */	
		virtual BOOL IsDisable() = 0;

		/**
		 *  @brief ���ƿؼ���ָ����Canvas��
		 *  @param pCanvas ָ�����������ָ��
		 *  @param x�������ϻ��Ƶ���ʼx������
		 *  @param y  �����ϻ��Ƶ���ʼy������
		 *  @return 
		 *  @sa 
		 */	
		virtual void Draw(IJuneCanvas* pCanvas, int x, int y) = 0;

		/**
		 *  @brief �ı�ؼ����δ�С
		 *  @param rc���ı�������ڸ��ؼ�����Ŀؼ�����
		 *  @sa 
		 */	
		virtual void Move(RECT& rc) = 0;

		/**
		 *  @brief ��ʾ�����ؿؼ�
		 *  @param bShow TRUEΪ��ʾ��FALSEΪ����
		 *  @sa 
		 */	
		virtual void Show(BOOL bShow) = 0;

		/**
		 *  @brief ʹ�ؼ����û򲻿���
		 *  @param bEnable �Ƿ����
		 *  @sa 
		 */	
		virtual void Enable(BOOL bEnable) = 0;
		
		/**
		 *  @brief ˢ�¿ؼ�
		 *  @param rc �ؼ���ˢ������ΪNULLʱΪȫ��ˢ��
		 *  @sa 
		 */
		virtual void ReDraw(RECT* rc) = 0;

		/**
		 *  @brief ����tip�ַ���
		 *  @param lpszText ��ʾ����
		 *  @sa ITToolTip 
		 */	
		virtual void SetToolTipText(LPCTSTR lpszText) = 0;

		/**
		 *  @brief ��ý���
		 *  @sa 
		 */	
		virtual void SetFocus() = 0;

		/**
		 *  @brief ���������
		 *  @sa 
		 */
		virtual void SetCapture() = 0;

		/**
		 *  @brief �ͷ�������
		 *  @sa 
		 */	
		virtual void ReleaseCapture() = 0;

		/**
		 *  @brief ��������
		 *  @sa 
		 */	
		virtual void LockUpdate() = 0;

		/**
		 *  @brief �������£�����ʱ��ˢ��������
		 *  @sa 
		 */		
		virtual void UnLockUpdate() = 0;

		/**
		 *  @brief ʹ�ؼ���ʾ����ǰ��
		 *  @sa 
		 */	
		virtual void BringToTop() = 0;

		/**
		 *  @brief �ж��Ƿ�Ϊ�Լ�������ؼ�
		 *  @param dwControlID ���жϵĿؼ�ID
		 *  @return ����Ƿ���TRUE�������򷵻�FALSE
		 */		
		virtual BOOL IsMyOffspring(DWORD dwControlID) = 0;

		/**
		 *  @brief ��ʾtip(�ؼ�����������tip)
		 */	
		virtual void ShowToolTip() = 0;
	};
}
#endif