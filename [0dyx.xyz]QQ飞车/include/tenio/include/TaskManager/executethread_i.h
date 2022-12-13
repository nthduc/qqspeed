#ifndef __EXECUTETHREAD_I_H__
#define __EXECUTETHREAD_I_H__

#include "../teniointerface.h"
#include "../teniointerface.h"

namespace Tenio
{
	class IExecuteThread;

	TENIO_MAKE_TYPEID_MAP(IExecuteThread, COMPONENTID_EXECUTETHREAD);

	class IExecuteThread : public IComponent
	{
	public:
		virtual BOOL AddTask(ITaskExecutor* pTaskExecutor) = 0;
		virtual BOOL BeginThread(HWND hWnd) = 0;
		//virtual BOOL Cancel() = 0;
		virtual BOOL Finalize() = 0;
	};
};

#endif

// InterfaceName:ExecuteThread
// SubFolderName:TaskManager
