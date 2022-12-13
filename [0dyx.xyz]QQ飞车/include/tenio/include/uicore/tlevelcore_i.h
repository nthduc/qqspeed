#ifndef __TLEVELCORE_I_H__
#define __TLEVELCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/tlevel_i.h"

namespace Tenio
{
	class ITLevelCore;

	TENIO_MAKE_TYPEID_MAP(ITLevelCore, COMPONENTID_TLEVELCORE);

	/**
	 * @ingroup tenuilib
	 * @brief uilib level¿Ø¼þºËÐÄ
	 * @sa IControlCore , ITLevel
	 */
	class ITLevelCore : public IControlCore, public ITLevel
	{
	public:
	};
};

#endif

// InterfaceName:TLevelCore
// SubFolderName:uicore
