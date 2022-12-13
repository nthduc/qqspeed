#ifndef __MULTISTATE_I_H__
#define __MULTISTATE_I_H__

namespace Tenio
{
#define STATEID_NORMAL	0x00000000
#define STATEID_HOVER	0x00000001
#define STATEID_PRESSED	0x00000002
#define STATEID_DISABLE	0x00000003

	class IMultiState
	{
	public:
		virtual void SwitchToState(DWORD dwState) = 0;
	};
}
#endif