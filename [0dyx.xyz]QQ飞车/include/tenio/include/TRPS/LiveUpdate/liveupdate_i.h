#pragma once

#include "../../teniointerface.h"
#include "../../tstring.h"
using namespace std;

namespace Tenio
{
	
	class ILiveUpdate : public IComponent
	{
	public:
		virtual BOOL Initialize(tstring& strCommandLine) = 0;
		virtual int Run() = 0;
		virtual void Finalize() = 0;
	};

	TENIO_MAKE_TYPEID_MAP(ILiveUpdate, COMPONENTID_LIVEUPDATE);

}