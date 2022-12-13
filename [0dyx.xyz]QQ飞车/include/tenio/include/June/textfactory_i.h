#ifndef __TEXTFACTORY_I_H__
#define __TEXTFACTORY_I_H__

#include "../teniointerface.h"
#include "text_i.h"
#include "junestruct.h"

namespace Tenio
{
	class ITextFactory;

	TENIO_MAKE_TYPEID_MAP(ITextFactory, COMPONENTID_TEXTFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief ����Ԫ�ع���
	 * <p>
	 *���� ��������Ԫ�صĹ���	 
	 * </p>
	 *
	 * @sa 
	 */
	class ITextFactory : public IComponent
	{
	public:
		/**
		 *  @brief �����������ʹ�������Ԫ��
		 *  @param eId �������ͣ��� enumIMAGEFORMATID 
		 *  @return �������ֶ���ָ��
		 *  @sa 
		 */	
		virtual IText* CreateInstance(ENUMIMAGEFORMATID eId) = 0;

		/**
		 *  @brief �ͷ����ֶ���
		 *  @param ���ͷŵ����ֶ���ָ��
		 *  @sa 
		 */
		virtual void Release(IText* pText) = 0;
	};
};

#endif // __TEXTFACTORY_H__

// InterfaceName:NormalTextImageFactory
// SubFolderName:June