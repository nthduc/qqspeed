#ifndef __ICONFACTORY_I_H__
#define __ICONFACTORY_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IIconFactory;

	TENIO_MAKE_TYPEID_MAP(IIconFactory, COMPONENTID_ICONFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief windowsͼ��������
	 *
	 * <p>
	 *���� ͬ���ļ�����ͬһ��ͼ����
	 * </p>
	 *
	 * @sa 
	 */
	class IIconFactory : public IComponent
	{
	public:
		/**
		 *  @brief ����ͼ����
		 *  @param lpszIconFile ͼ���ļ��䣬������ȫ·�������·����\n
		 *						��������·���������趨����Դ��·����Ѱ��
		 *  @return ����ͼ������ʧ���򷵻�NULL
		 *  @sa 
		 */
		virtual HICON LoadIcon(LPCTSTR lpszIconFile) = 0;

		/**
		 *  @brief ����ͼ����
		 *  @param hIcon �����ٵ�ͼ����
		 *  @sa 
		 */
		virtual void ReleaseIcon(HICON hIcon) = 0;

		/**
		 *  @brief ����ͼ����Դ�ļ���Ŀ¼��ȱʡ���ڿ�ִ���ļ���Ŀ¼��data/ui/
		 *  @param lpszResRootPath ��Դ��Ŀ¼
		 *  @sa 
		 */		
		virtual void SetResRootPath(LPCTSTR lpszResRootPath) = 0;
	};
};


#endif

// InterfaceName:IconFactory
// SubFolderName:uicore
