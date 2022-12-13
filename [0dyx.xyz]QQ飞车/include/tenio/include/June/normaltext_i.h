#ifndef __NORMALTEXT_I_H__
#define __NORMALTEXT_I_H__

#include "../teniointerface.h"
#include "text_i.h"

namespace Tenio
{
	class INormalText;

	TENIO_MAKE_TYPEID_MAP(INormalText, COMPONENTID_NORMALTEXT);

	/**
	 * @ingroup tenuilib
	 * @brief ��ͨ�ı�Ԫ��
	 *
	 * <p>
	 *���� ������Ϊ EIF_NORMALTEXT �Ļ���Ԫ��
	 * </p>
	 *
	 * @sa enumIMAGEFORMATID
	 */	
	class INormalText : public IText, public IComponent
	{
	};
};

#endif // __NORMALTEXT_I_H__