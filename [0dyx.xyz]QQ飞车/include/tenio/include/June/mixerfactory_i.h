#ifndef __MIXFACTORY_I_H__
#define __MIXFACTORY_I_H__

#include "../teniointerface.h"
#include "junestruct.h"
#include "mixer_i.h"

namespace Tenio
{
	class IMixerFactory;
	
	TENIO_MAKE_TYPEID_MAP(IMixerFactory, COMPONENTID_MIXERFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief 混和器创建工厂
	 *
	 * <p>
	 *　　 enumCANVASFORMATID 
	 * </p>
	 *
	 * @sa enumCANVASFORMATID 
	 */	
	class IMixerFactory : public IComponent
	{
	public:
		/**
		 *  @brief 创建混合器
		 *  @param CanvasFormatId : 取值为 enumCANVASFORMATID 
		 *  @param ImageFormatId  : 取值为 enumIMAGEFORMATID 
		 *	@param MixModeId      : 取值为 enumMIXMODE 
		 *  @return 如果有合适的 Mixer, 则返回一个 Mixer. 否则返回 NULL.\n
		 *			返回的 Mixer 由 MixerFactory 管理生命期, 删除 Factory时删除该 Mixer
		 *  @sa enumCANVASFORMATID , enumIMAGEFORMATID , enumMIXMODE 
		 */		
		virtual IJuneMixer* Create(int nCanvasFormatId, int nImageFormatId, 
			int nMixModeId) = 0;
	};
};

#endif