#ifndef __AUTOCS_H__
#define __AUTOCS_H__

class CAutoCS
{
public:
	CAutoCS()
	{
		InitializeCriticalSection(&m_cs);
	}

	void Enter()
	{
		EnterCriticalSection(&m_cs);
	}

	void Leave()
	{
		LeaveCriticalSection(&m_cs);
	}

	~CAutoCS()
	{
		DeleteCriticalSection(&m_cs);
	}

private:
	CRITICAL_SECTION	m_cs;
};

class CAutoCSGuard
{
public:
	CAutoCSGuard(CAutoCS* pAutoCS):	m_pCS(pAutoCS)
	{
		m_pCS->Enter();
	}

	~CAutoCSGuard()
	{
		m_pCS->Leave();
	}

private:
	CAutoCS*	m_pCS;
};

#define DECL_AUTOCS			CAutoCS	m_xAutoCS;
#define USE_AUTOCS			CAutoCSGuard xAutoCS(&m_xAutoCS);
#define USE_AUTOCSEXT(x)	CAutoCSGuard xAutoCS(&x);

#endif