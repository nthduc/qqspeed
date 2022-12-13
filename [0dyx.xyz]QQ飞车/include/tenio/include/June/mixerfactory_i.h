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
	 * @brief �������������
	 *
	 * <p>
	 *���� enumCANVASFORMATID 
	 * </p>
	 *
	 * @sa enumCANVASFORMATID 
	 */	
	class IMixerFactory : public IComponent
	{
	public:
		/**
		 *  @brief ���������
		 *  @param CanvasFormatId : ȡֵΪ enumCANVASFORMATID 
		 *  @param ImageFormatId  : ȡֵΪ enumIMAGEFORMATID 
		 *	@param MixModeId      : ȡֵΪ enumMIXMODE 
		 *  @return ����к��ʵ� Mixer, �򷵻�һ�� Mixer. ���򷵻� NULL.\n
		 *			���ص� Mixer �� MixerFactory ����������, ɾ�� Factoryʱɾ���� Mixer
		 *  @sa enumCANVASFORMATID , enumIMAGEFORMATID , enumMIXMODE 
		 */		
		virtual IJuneMixer* Create(int nCanvasFormatId, int nImageFormatId, 
			int nMixModeId) = 0;
	};
};

#endif