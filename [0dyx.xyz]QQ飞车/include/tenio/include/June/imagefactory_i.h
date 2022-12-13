#ifndef __IMAGEFACTORY_I_H__
#define __IMAGEFACTORY_I_H__

#include "../teniointerface.h"
#include "image_i.h"

namespace Tenio
{
	class IImageFactory;

	TENIO_MAKE_TYPEID_MAP(IImageFactory, COMPONENTID_IMAGEFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief ����ͼ�����Ĺ���
	 *
	 * <p>
	 *���� ͼ����󴴽�������ͬ���ļ�����ͬһ������ָ�롣���������Ķ����������Factory�ͷ�
	 * </p>
	 *
	 * @sa 
	 */
	class IImageFactory : public IComponent
	{
	public:
		/**
		 *  @brief �����ļ�������ͼ�����
		 *  @param lpszFileName �ļ���������ȫ·���������·������\n
		 *						��������·����Factory����ָ�������·����Ѱ��
		 *  @return ���ش������ͼ�����ָ�룬�������NULL�����������ʧ��
		 *  @sa 
		 */	
		virtual IImage* LoadImageFile(LPCTSTR lpszFileName) = 0;

		/**
		 *  @brief �����������д���ͼ�����
		 *  @param pArray ��������ָ��
		 *  @param dwWidth	ͼ����
		 *	@param dwHeight	ͼ��߶�
		 *	@param dwBitsPerPixer ÿ��������ռλ��,������ 1,4,8,24,32
		 *	@param dwBytesPerLine �ж��룬ÿ���ֽ�����
		 *	@param bFlipImage �Ƿ���Ҫ������������ͼ���ǵ���ʱ��Ҫ��
		 *  @return �ɹ��򷵻ض���ָ�룬ʧ���򷵻�NULL
		 *  @sa 
		 */
		virtual IImage* LoadImageFromArray(
			BYTE* pArray,
			DWORD dwWidth,
			DWORD dwHeight,
			DWORD dwBitsPerPixel, 
			DWORD dwBytesPerLine, 
			BOOL bFlipImage) = 0;

		/**
		 *  @brief �ͷŶ���
		 *  @param pImage ���ͷŶ���ָ��
		 *  @sa 
		 */	
		virtual void ReleaseImage(IImage* pImage) = 0;

		/**
		 *  @brief ������Դ�ĸ�·����ȱʡ���ڿ�ִ���ļ���Ŀ¼data\ui\��
		 *  @param lpszResRootPath ��Դ��·��ȫ·����
		 */
		virtual void SetResRootPath(LPCTSTR lpszResRootPath) = 0;		

		/**
		 *  @brief �����Դ�ĸ�·��
		 *  @param lpszResRootPath ���·���Ļ�����
		 *  @param nMaxLen lpszResRootPath����󳤶�
		 */		
		virtual void GetResRootPath(LPTSTR lpszResRootPath, UINT nMaxLen) = 0;
	};

};

#endif

