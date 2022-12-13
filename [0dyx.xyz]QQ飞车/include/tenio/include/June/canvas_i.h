#ifndef __JUNECANVAS_I_H__
#define __JUNECANVAS_I_H__

#include "region_i.h"
#include "canvasdefine.h"

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief ����
	 *
	 * <p>
	 *���� ICanvas�ӿ�����������Canvasģ��Ļ��ࡣ\n
	 *	Canvasģ���ṩͼ������ֻ��Ƶ�Ŀ�ĵأ�ͬʱҲʵ�ּ��ֳ��õ�ͼ�λ��Ʒ�����
	 * </p>
	 *
	 * @sa 
	 */	
	class IJuneCanvas
	{
	public:
		/**
		 *  @brief ����Canvas�ĸ�ʽID
		 *  @return ����ֵ�μ� enumCANVASFORMATID 
		 *  @sa 
		 */		
		virtual int GetCanvasFormatId() = 0;

		/**
		 *  @brief �ô�ɫ�������
		 *  @param rc ����������
		 *  @param cr ������ɫֵ
		 *  @sa 
		 */	
		virtual void FillRect(const RECT& rc, COLORREF cr) = 0;

		/**
		 *  @brief ʹ�ô�͸��ֵ�Ĵ�ɫ�������
		 *  @param rc ����������
		 *  @param cr ������ɫֵ
		 *  @param byAlpha alphaֵ��ȡֵ��Χ��0~255
		 *  @return 
		 *  @sa 
		 */	
		virtual void FillRect(const RECT& rc, COLORREF cr, BYTE byAlpha) = 0;

		/**
		 *  @brief ���ƾ�������߿�
		 *  @param rc �������
		 *  @param cr �߿���ɫֵ
		 *  @return 
		 *  @sa 
		 */	
		virtual void FrameRect(const RECT& rc, COLORREF cr) = 0;

		/**
		 *  @brief ���ƾ�������߿�
		 *  @param rc �������
		 *  @param pi ������Ϣ���� tagPenInfo 
		 *  @return 
		 *  @sa 
		 */	
		virtual void FrameRectIndirect(const RECT& rc, PENINFO& pi) = 0;

		/**
		 *  @brief �������ˣ��ð�͸���������
		 *  @param rc �ƶ�ǰ��������
		 *  @param size �߿���
		 *  @param rc �ƶ����������
		 *  @param size �ƶ���߿���
		 *  @return 
		 *  @sa 
		 */	
		virtual void DragHalftoneRect(
			const RECT& rc, 
			SIZE size, 
			const RECT& rcLast, 
			SIZE sizeLast) = 0;

		/**
		 *  @brief ��ֱ��
		 *  @param ptBegin ��ʼλ��
		 *  @param ptEnd ����λ��
		 *  @param cr ��ɫֵ
		 *  @return 
		 *  @sa 
		 */	
		virtual void Line(const POINT& ptBegin, const POINT& ptEnd, COLORREF cr) = 0;	

		/**
		 *  @brief ��ֱ��
		 *  @param ptBegin ��ʼλ��
		 *  @param ptEnd ����λ��
		 *  @param pi ������Ϣ���� tagPenInfo 
		 *  @return 
		 *  @sa 
		 */	
		virtual void LineIndirect(const POINT& ptBegin, const POINT& ptEnd, PENINFO& pi) = 0;
		
		/**
		 *  @brief ���Canvas�ĸ�������
		 *  @return Canvas�ĸ����������ָ��
		 *  @sa 
		 */		
		virtual IJuneRegion* GetUpdateRegion() = 0;
	};
};

#endif