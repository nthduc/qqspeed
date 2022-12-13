#ifndef __NORMALREGION_I_H__
#define __NORMALREGION_I_H__

#include "region_i.h"

namespace Tenio
{
	class INormalRegion;
	TENIO_MAKE_TYPEID_MAP(INormalRegion, COMPONENTID_NORMALREGION);

	/**
	 * @ingroup tenuilib
	 * @brief ��ͨ����������ʵ��
	 *
	 * <p>
	 *���� �ڲ��ɾ��ο��ʾ����������
	 * </p>
	 *
	 * @sa 
	 */
	class INormalRegion : public IJuneRegion, public IComponent
	{
	public:
	};
};

#endif