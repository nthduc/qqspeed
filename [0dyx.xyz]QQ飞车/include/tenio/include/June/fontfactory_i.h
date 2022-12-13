#ifndef __FONTFACTORY_I_H__
#define __FONTFACTORY_I_H__

#include "../teniointerface.h"
#include "font_i.h"
#include "junestruct.h"

namespace Tenio
{
	class IFontFactory;

	TENIO_MAKE_TYPEID_MAP(IFontFactory, COMPONENTID_FONTFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief ���幤��
	 *
	 * <p>
	 *���� ���𴴽���������������Թ���ͳһ���٣��ⲿ����Ҫ����
	 * </p>
	 *
	 * @sa 
	 */	
	class IFontFactory : public IComponent
	{
	public:
		/**
		 *  @brief ������������ͬ������Ϣ����ͬһ���������ָ��
		 *  @param LogFont ������ϸ��Ϣ��LOGFONT��windows�ṹ
		 *  @return �����������ָ��
		 *  @sa 
		 */		
		virtual IFont* GetFont(const LOGFONT& LogFont) = 0;
	};
};

#endif // __FONTFACTORY_I_H__

// InterfaceName:FontFactory
// SubFolderName:June