#ifndef __TMIFIMAGEBOX_I_H__
#define __TMIFIMAGEBOX_I_H__

#include "uilib/control_i.h"

namespace Tenio
{
	class ITMifImageBox;

	TENIO_MAKE_TYPEID_MAP(ITMifImageBox, COMPONENTID_TMIFIMAGEBOX);

	/**
	 * @ingroup tenuilib
	 * @brief mif �������ſؼ�
	 *
	 * <p>
	 *     ITMifImageBox �ǲ��� mif �����Ŀؼ��� �ڲ�ʹ�� IMifImageLayer ʵ����Ҫ���ܡ�
	 *	   ͬʱ������Ӧ�ļ����¼���֧��mif�����ļ��⣬Ҳ֧����ʾ IImageBox ֧�ֵ����е���������ͼ��
	 * </p>
	 *
	 * @see IControl
	 */
	class ITMifImageBox : public IControl
	{
	public:
		/**
		 * @brief װ��ͼ���ļ�
		 * @param lpszFileName [in] �ļ���
		 * @return װ�سɹ�������TRUE�����򷵻�FALSE
		 */
		virtual BOOL LoadImage(LPCTSTR lpszFileName) = 0;

		/**
		 * @brief ��ʼ����
		 * @param bReply [in] ѭ������
		 */
		virtual void StartPlay(BOOL bReplay = FALSE) = 0;
	};
};

#endif

// InterfaceName:TMifImageBox
// SubFolderName:UILib
