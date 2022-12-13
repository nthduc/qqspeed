#ifndef __JUNEFONT_I_H__
#define __JUNEFONT_I_H__

#include <windows.h>

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief �������������
	 *
	 * <p>
	 *���� �����������Ļ��࣬���������е�����������̳�����ʵ�����ķ���
	 * </p>
	 *
	 * @sa 
	 */	
	class IFont
	{
	public:
		/**
		 *  @brief ��������ʽID
		 *  @return ���������ʽ ֵΪ enumFONTFORMATID ����
		 *  @sa enumFONTFORMATID
		 */	
		virtual int GetFormatId() = 0;

		/**
		 *  @brief ����ַ��߶�
		 *  @return �����ַ��߶�
		 *  @sa 
		 */	
		virtual int GetCharHeight() = 0;

		/**
		 *  @brief ���ĳ���ַ����
		 *  @param uChar �ַ�
		 *  @return �ַ�uChar�Ŀ��
		 *  @sa 
		 */	
		virtual int GetCharWidth(UINT uChar) = 0;

		/**
		 *  @brief ��õ����ַ����Ŀ��
		 *  @param lpszText �ַ���ָ��
		 *  @param nLen���ַ����ַ�����
		 *  @return �����ַ������
		 *  @sa 
		 */
		virtual int GetTextWidth(LPCTSTR lpszText, int nLen) = 0;
		
		/**
		 *  @brief ���������Ϣ�ṹ��ָ��
		 *  @return ����������Ϣ�ṹ
		 *  @sa 
		 */		
		virtual const LOGFONT& GetLogFont() = 0;
	};
};

#endif // __JUNEFONT_I_H__