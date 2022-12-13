#include "iobject.h"


IMPLEMENT_RTTI_BASE(IObject);

IObject::IObject(void)
{

}

IObject::~IObject(void)
{

}
char IMemMgr::m_stObj[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
IMemMgr::IMemMgr()
{
	ReadCFG();
#ifdef SUPPORT_MULTI_THREAD
	InitializeCriticalSection(&m_protect);
#endif	
}

IMemMgr::~IMemMgr()
{
#ifdef SUPPORT_MULTI_THREAD
	EnterCriticalSection(&m_protect);
#endif	
	m_memObj8.ReleaseAllMemory();
	m_memObj16.ReleaseAllMemory();
	m_memObj24.ReleaseAllMemory();
	m_memObj32.ReleaseAllMemory();
	m_memObj48.ReleaseAllMemory();
	m_memObj64.ReleaseAllMemory();
	m_memObj80.ReleaseAllMemory();
	m_memObj96.ReleaseAllMemory();
	m_memObj128.ReleaseAllMemory();
	m_memObj160.ReleaseAllMemory();
	m_memObj192.ReleaseAllMemory();
	m_memObj256.ReleaseAllMemory();
	m_memObj320.ReleaseAllMemory();
	m_memObj512.ReleaseAllMemory();

#ifdef SUPPORT_MULTI_THREAD
	LeaveCriticalSection(&m_protect);
	DeleteCriticalSection(&m_protect);
#endif
}

void IMemMgr::LogOutMemoryAllocateStatus()
{
	if (m_stObj[0]) 
	{
		char A[256];
		if (m_stObj[1])
		{
			m_memObj8.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[2]) 
		{
			m_memObj16.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[3]) 
		{
			m_memObj24.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[4]) 
		{
			m_memObj32.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[5]) 
		{
			m_memObj48.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[6]) 
		{	
			m_memObj64.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[7]) 
		{	
			m_memObj80.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[8]) 
		{
			m_memObj96.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[9]) 
		{
			m_memObj128.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[10]) 
		{
			m_memObj160.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[11]) 
		{
			m_memObj192.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[12]) 
		{
			m_memObj256.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}
		if (m_stObj[13]) 
		{
			m_memObj320.ReportMemoryStatus(A);
			//	LogToScreen("%s", A);
		}	
		if (m_stObj[14]) 
		{
			m_memObj512.ReportMemoryStatus(A);
		}

	}

}

void IMemMgr::GetMemoryUsingStatus()
{
	if (m_stObj[0]) 
	{
		char A[256] = "";
		if (m_stObj[1])
		{
			m_memObj8.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[2]) 
		{
			m_memObj16.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[3]) 
		{
			m_memObj24.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[4]) 
		{
			m_memObj32.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[5]) 
		{
			m_memObj48.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[6]) 
		{	
			m_memObj64.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[7]) 
		{	
			m_memObj80.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[8]) 
		{
			m_memObj96.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[9]) 
		{
			m_memObj128.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[10]) 
		{
			m_memObj160.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[11]) 
		{
			m_memObj192.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[12]) 
		{
			m_memObj256.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}
		if (m_stObj[13]) 
		{
			m_memObj320.ReportMemoryStatus(A);
			//	CPython::runFmtStr("print '%s'", A);
		}	
		if (m_stObj[14]) 
		{
			m_memObj512.ReportMemoryStatus(A);
		}
	}	
}

void IMemMgr::ReadCFG()
{
	char path[128] = "./config/GameCFG.ini";
	m_stObj[0] = ::GetPrivateProfileInt("memory", "Obj_on", 1, path);
	m_stObj[1] = ::GetPrivateProfileInt("memory", "Obj_8", 1, path);
	m_stObj[2] = ::GetPrivateProfileInt("memory", "Obj_16", 1, path);
	m_stObj[3] = ::GetPrivateProfileInt("memory", "Obj_24", 1, path);
	m_stObj[4] = ::GetPrivateProfileInt("memory", "Obj_32", 1, path);
	m_stObj[5] = ::GetPrivateProfileInt("memory", "Obj_48", 1, path);
	m_stObj[6] = ::GetPrivateProfileInt("memory", "Obj_64", 1, path);
	m_stObj[7] = ::GetPrivateProfileInt("memory", "Obj_80", 1, path);
	m_stObj[8] = ::GetPrivateProfileInt("memory", "Obj_96", 1, path);
	m_stObj[9] = ::GetPrivateProfileInt("memory", "Obj_128", 1, path);
	m_stObj[10] = ::GetPrivateProfileInt("memory", "Obj_160", 1, path);
	m_stObj[11] = ::GetPrivateProfileInt("memory", "Obj_192", 1, path);
	m_stObj[12] = ::GetPrivateProfileInt("memory", "Obj_256", 1, path);
	m_stObj[13] = ::GetPrivateProfileInt("memory", "Obj_320", 1, path);
	m_stObj[14] = ::GetPrivateProfileInt("memory", "Obj_512", 1, path);
}

void IMemMgr::ReleaseFreeMemory()
{
	MEM_LOCK;
	m_memObj8.ReleaseFreeMemory();
	m_memObj16.ReleaseFreeMemory();
	m_memObj24.ReleaseFreeMemory();
	m_memObj32.ReleaseFreeMemory();
	m_memObj48.ReleaseFreeMemory();
	m_memObj64.ReleaseFreeMemory();
	m_memObj80.ReleaseFreeMemory();
	m_memObj96.ReleaseFreeMemory();
	m_memObj128.ReleaseFreeMemory();
	m_memObj160.ReleaseFreeMemory();
	m_memObj192.ReleaseFreeMemory();
	m_memObj256.ReleaseFreeMemory();
	m_memObj320.ReleaseFreeMemory();
	m_memObj512.ReleaseFreeMemory();
}

void * IMemMgr::New(size_t size, int num /* = 0 */)
{

	MEM_LOCK;

	void * pRet = NULL;
	size_t _size = size * num;
	if (m_stObj[0] > 0) 
	{
		if (m_stObj[1] && _size <= 8) 
		{
			pRet = m_memObj8.New(_size);
		}
		else if (m_stObj[2] && _size <= 16) 
		{
			pRet = m_memObj16.New(_size);
		}
		else if (m_stObj[3] && _size <= 24) 
		{
			pRet = m_memObj24.New(_size);
		}
		else if (m_stObj[4] && _size <= 32) 
		{
			pRet = m_memObj32.New(_size);
		}
		else if (m_stObj[5] && _size <= 48) 
		{
			pRet = m_memObj48.New(_size);
		}
		else if (m_stObj[6] && _size <= 64) 
		{
			pRet = m_memObj64.New(_size);
		}
		else if (m_stObj[7] && _size <= 80) 
		{
			pRet = m_memObj80.New(_size);
		}
		else if (m_stObj[8] && _size <= 96) 
		{
			pRet = m_memObj96.New(_size);
		}
		else if (m_stObj[9] && _size <= 128) 
		{
			pRet = m_memObj128.New(_size);
		}
		else if (m_stObj[10] && _size <= 160) 
		{
			pRet = m_memObj160.New(_size);
		}
		else if (m_stObj[11] && _size <= 192) 
		{
			pRet = m_memObj192.New(_size);
		}
		else if (m_stObj[12] && _size <= 256) 
		{
			pRet = m_memObj256.New(_size);
		}
		else if (m_stObj[13] && _size <= 320) 
		{
			pRet = m_memObj320.New(_size);
		}
		else if (m_stObj[14] && _size <= 512)
		{
			pRet = m_memObj512.New(_size);
		}
		else
		{
			unsigned char * pTemp = ::new unsigned char[_size+4];
			*(size_t*)pTemp = _size;
			pRet = &pTemp[4];
		}
	}
	else
	{
		unsigned char * pTemp = ::new unsigned char[_size];
		pRet = pTemp;
	}

	return pRet;
}



void IMemMgr::Delete(void * pMem)
{
	MEM_LOCK;
	if (m_stObj[0] > 0) 
	{
		size_t _size = GetSize(pMem);

#ifdef MEMORY_GUARD
		if (_size <= 64 && (*(size_t*)((size_t)pMem - (size_t)8) != 0xBEBEBEBE))
		{
			assert0("BAD MEMORY");
		}
#endif

		if (m_stObj[1] && _size <= 8) 
		{
			m_memObj8.Delete(pMem);
		}
		else if (m_stObj[2] && _size <= 16) 
		{
			m_memObj16.Delete(pMem);
		}
		else if (m_stObj[3] && _size <= 24)
		{
			m_memObj24.Delete(pMem);
		}
		else if (m_stObj[4] && _size <= 32) 
		{
			m_memObj32.Delete(pMem);
		}
		else if (m_stObj[5] && _size <= 48) 
		{
			m_memObj48.Delete(pMem);
		}
		else if (m_stObj[6] && _size <= 64) 
		{
			m_memObj64.Delete(pMem);
		}
		else if (m_stObj[7] && _size <= 80) 
		{
			m_memObj80.Delete(pMem);
		}
		else if (m_stObj[8] && _size <= 96) 
		{
			m_memObj96.Delete(pMem);
		}
		else if (m_stObj[9] && _size <= 128) 
		{
			m_memObj128.Delete(pMem);
		}
		else if (m_stObj[10] && _size <= 160) 
		{
			m_memObj160.Delete(pMem);
		}
		else if (m_stObj[11] && _size <= 192) 
		{
			m_memObj192.Delete(pMem);
		}
		else if (m_stObj[12] && _size <= 256) 
		{
			m_memObj256.Delete(pMem);
		}
		else if (m_stObj[13] && _size <= 320) 
		{
			m_memObj320.Delete(pMem);
		}
		else if (m_stObj[14] && _size <= 512)
		{
			m_memObj512.Delete(pMem);
		}
		else
			delete[] ((char*)pMem-4);
	}
	else
		delete[] pMem;

}