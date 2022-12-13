#ifndef __EVENT_IMPL_T_H__
#define __EVENT_IMPL_T_H__

namespace PartialTemplateSpecializationForEvent
{
	template<class IActualEventInterface>
		class IDefaultEventImpl : public IActualEventInterface
	{
	};
};


template<class IActualEventInterface, int nEventVersion>
class IEventImpl
	: public PartialTemplateSpecializationForEvent::IDefaultEventImpl
		<IActualEventInterface>
{
public:
	int GetEventVersion()
	{
		return nEventVersion;
	}
};

#endif