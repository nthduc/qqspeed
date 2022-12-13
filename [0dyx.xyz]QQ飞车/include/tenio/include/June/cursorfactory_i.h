#ifndef __CURSORFACTORY_I_H__
#define __CURSORFACTORY_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class ICursorFactory;

	TENIO_MAKE_TYPEID_MAP(ICursorFactory, COMPONENTID_CURSORFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief �����Դ����
	 *
	 * <p>
	 *���� 	 �����ṩͨ�õĹ������Factory����ʱ�����������й�������ⲿ����Ҫ����
	 * </p>
	 *
	 * @sa 
	 */
	
	class ICursorFactory : public IComponent
	{
	public:
		/**
		 *  @brief ���ͨ�ù����
		 *  @param lpszCursorName����Դ���ƣ���the Microsoft Win32? APIԤ����ġ�MSDN LoadCursor
		 *  @return ���ع����
		 *  @sa 
		 */	
		virtual HCURSOR GetCommonCursor(LPCTSTR lpszCursorName) = 0;

		/**
		 *  @brief ͨ��ID��ù����
		 *  @param dwCursorID ���ID
		 *  @return ���ع����
		 */	
		virtual HCURSOR GetCursor(DWORD dwCursorID) = 0;		
	};
};

#endif

// InterfaceName:CursorFactory
// SubFolderName:uicore
