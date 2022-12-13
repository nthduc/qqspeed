#ifndef __JUNEREGION_I_H__
#define __JUNEREGION_I_H__

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief �����������
	 *
	 * <p>
	 *���� �ö�������������ĺϲ������õȲ�������������������\n
	 *	�������˵����δ��л�õ�ǰ����µ����о���:\n
	 *	\n
	 *		int nRectCount = pRegion->EnumRects(NULL, 0);\n
	 *	\n
	 *		RECT* prcList = NULL;\n
	 *		TTenio_AllocMemory(&prcList, sizeof(RECT)*nRectCount, TRUE);\n
	 *		pRegion->EnumRects(prcList, nRectCount);\n
	 *	 \n
	 *		// do something using prcList ...\n
	 *	\n	   
	 *		TTenio_ReleaseMemory(prcList);\n
	 *	\n
	 * </p>
	 *
	 * @sa 
	 */	
	class IJuneRegion
	{
	public:
		/**
		 *  @brief �����Ƿ��Զ���������ϲ�
		 *  @param bAutoMixe ���� AutoMix ״̬��ʱ, �������״���Զ��޳��뱾��״�ظ��Ĳ���
		 *  @sa 
		 */
		virtual void SetAutoMix(BOOL bAutoMix) = 0;

		/**
		 *  @brief �ֶ����� Mix, �����ظ��Ĳ��ֶ�������
		 */
		virtual void MixChildren() = 0;

		/**
		 *  @brief ��һ�����μ��뵽��״��
		 *  @param prcNewRect ��������¾���ָ��
		 *  @return �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE(�������ָ��ΪNULL)
		 *  @sa 
		 */
		virtual BOOL InsertRect(const RECT* prcNewRect) = 0;

		/**
		 *  @brief ��ǰ��״�Ƴ���һ������
		 *  @param prcToRemoveRect ���Ƴ��ľ���ָ��
		 *  @return �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE(�������ָ��ΪNULL)
		 *  @sa 
		 */
		virtual BOOL RemoveRect(const RECT* prcToRemoveRect) = 0;

		/**
		 *  @brief ��� Region
		 *  @return �������ⷵ��TRUE
		 *  @sa 
		 */
		virtual BOOL ClearAll() = 0;

		/**
		 *  @brief ö�ٵ�ǰ���о���
		 *  @param pRectList ������ε��ڴ�ָ�룬���ⲿ����
		 *  @param nCount ���ڴ�ɴ�����εĸ���
		 *  @return pRectList  = NULL �� nCount <= 0 ʱ���� Rect �ĸ��� \n
					pRectList != NULL �� nCount > 0  ʱ������Щ Rect
		 *  @sa 
		 */
		virtual int EnumRects(RECT* pRectList, int nCount) = 0;

		/**
		 *  @brief ��ǰ����������Ƿ�Ϊ��
		 *  @return ���򷵻�TRUE�������򷵻�FALSE
		 *  @sa 
		 */
		virtual BOOL IsEmpty() = 0;

		/**
		 *  @brief ƽ�Ƶ�ǰ���������������
		 *  @param x ƽ�Ƶ�x�����
		 *  @param y ƽ�Ƶ�y�����
		 *  @sa 
		 */	
		virtual void Offset(int x, int y) = 0;

		/**
		 *  @brief �ж�һ���������Ƿ������region֮��
		 *  @param rc����������
		 *  @return �Ƿ���TRUE, �������ڻ��ص��򷵻�FALSE
		 */	
		virtual BOOL RectOutOfRegion(RECT& rc) = 0;
	};
};

#endif