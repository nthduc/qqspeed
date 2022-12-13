
#define _WIN32_WINNT 0x0400
#include "profile.h"
#include "kernel.h"
#include <stack>
#include <windows.h>

typedef signed __int64			INT64;

namespace PreciseTimer
{
#ifdef _MSC_VER
	#pragma warning( disable : 4035 )
	__forceinline INT64 GetAbsoluteCycles()
	{
		__asm rdtsc;
	}
	#pragma warning( default : 4035 )
#else
	INT64 GetAbsoluteCycles();
#endif

static INT64 gs_CyclePerSec;
static double gs_SecPerCycle;
static bool   gs_bInit = false;

#define RDTSC __asm _emit 0x0F __asm _emit 0x31
#pragma warning(disable: 4035)
__inline __int64 GetCycleNumber()
{	
    __asm
    {
        RDTSC	
    }
}

INT64  GetCPU_HZ()
{
    LARGE_INTEGER t1,t2,tf;
    __int64 c1,c2;
    QueryPerformanceFrequency(&tf);
    QueryPerformanceCounter(&t1);
    c1 = GetCycleNumber();
    _asm 
    {		
        MOV  EBX, 2500000		
        WaitAlittle:			
                DEC		EBX		
        JNZ	WaitAlittle	
    }
    QueryPerformanceCounter(&t2);
    c2 = GetCycleNumber();
    return ((c2 - c1) * tf.QuadPart / (t2.QuadPart - t1.QuadPart));
}

void   Init()
{
	if( !gs_bInit )
	{
		gs_CyclePerSec = GetCPU_HZ();
		gs_SecPerCycle = 1.0 / double(gs_CyclePerSec);
		gs_bInit = true;
	}
}

double CycleToSecond( INT64 cycle )
{
	return cycle*gs_SecPerCycle;
}

double GetAbsoluteSeconds()
{
	return CycleToSecond( GetAbsoluteCycles() );
}

INT64 GetCyclesPerSecond()
{
	return gs_CyclePerSec;
}

double GetSecondsPerCycle()
{
	return gs_SecPerCycle;
}

INT64 SecondToCycle( double sec )
{
	return (INT64)( sec*double(gs_CyclePerSec) );
}

};

namespace Profile
{

const int MAX_PROFILENAME = 64;
const int MAX_RECORDBUF = 64;
struct stProfileRecord
{
	char name[MAX_PROFILENAME];
	int   calltimes;
	stProfileRecord *parent;
	INT64 starttime;
	INT64 accumtime;
};

stProfileRecord s_RecordBuf[MAX_RECORDBUF];
int  NumRecord = -1;
std::stack<stProfileRecord *> s_IDStack;
bool  bThisFrameValid = false;

void BeginFrame()
{
	PreciseTimer::Init();
	if( bThisFrameValid ) End();

	if( NumRecord == -1 )
	{
		NumRecord = 0;
		PreciseTimer::Init();
		assert( s_IDStack.empty() );
	}
	bThisFrameValid = true;
	Begin( "root" );
}

stProfileRecord *GetRecordPtr( const char *name )
{
	for( int i=0; i<NumRecord; i++ )
	{
		if( strcmp(s_RecordBuf[i].name, name) == 0 ) return &s_RecordBuf[i];
	}
	return NULL;
}

void Begin( const char *name )
{
	if( !bThisFrameValid ) return;

	stProfileRecord *pobj = GetRecordPtr(name);
	if( !pobj )
	{
		pobj = &s_RecordBuf[NumRecord++];
		assert( NumRecord < MAX_RECORDBUF );
		strcpy( pobj->name, name );
		pobj->accumtime = 0;

		if( s_IDStack.empty() ) pobj->parent = 0;
		else pobj->parent = s_IDStack.top();
	}

	pobj->starttime = PreciseTimer::GetAbsoluteCycles();
	pobj->calltimes++;
	s_IDStack.push( pobj );
}

void End()
{
	if( !bThisFrameValid ) return;

	stProfileRecord *pobj = s_IDStack.top();
	pobj->accumtime += PreciseTimer::GetAbsoluteCycles() - pobj->starttime;
	s_IDStack.pop();
}

int GetIndentNum( stProfileRecord *p )
{
	int count = 0;
	while( p->parent )
	{
		count++;
		p = p->parent;
		assert( count < 1000 );
	}

	return count;
}

static INT64 GetChildAccumTime( stProfileRecord *precord )
{
	INT64 count = 0;
	for( int i=0; i<NumRecord; i++ )
	{
		if( s_RecordBuf[i].parent == precord ) count += s_RecordBuf[i].accumtime;
	}
	return count;
}

void DumpRecordToLine( char line[256], stProfileRecord *precord )
{
	//在上一层时间中百分比, 平均每次时间，调用次数，名字
	char indent[256] = "";
	int numindent = GetIndentNum( precord );
	for( int i=0; i<numindent; i++ ) strcat( indent, "   " );
	strcat( indent, precord->name );

	INT64 usedtime = precord->accumtime - GetChildAccumTime(precord);
	double timeinsec = PreciseTimer::CycleToSecond(usedtime);

	float avetime = 0;
	if( precord->calltimes ) avetime = float( timeinsec*1000 / precord->calltimes );

	float percent = float( double(usedtime) / s_RecordBuf[0].accumtime );
	percent *= 100;
	
	char mystr[6][64];
	sprintf( mystr[0], "%3.1f", percent );
	sprintf( mystr[1], "%3.2f", avetime );
	sprintf( mystr[2], "%d",    precord->calltimes );
	sprintf( line, "%5s :%5s :%5s :%s\n", mystr[0], mystr[1], mystr[2], indent );
}

void DumpRecordToBuffer( char *buffer, int maxbuf, stProfileRecord *precord )
{
	char line[256];
	DumpRecordToLine( line, precord );
	assert( int(strlen(buffer) + strlen(line)) < maxbuf );
	strcat( buffer, line );

	for( int i=0; i<NumRecord; i++ )
	{
		if( s_RecordBuf[i].parent == precord ) DumpRecordToBuffer( buffer, maxbuf, &s_RecordBuf[i] );
	}
}

void DumpToBuffer( char *buffer, int maxbuf )
{
	End();

	int i;
	assert( NumRecord>0 && s_RecordBuf[0].parent==0 );

	float fps = float( s_RecordBuf[0].calltimes / PreciseTimer::CycleToSecond(s_RecordBuf[0].accumtime) );
	sprintf( buffer, "FPS=%.1f\n", fps );

	strcat( buffer, "Percent : Aver : Times : Name\n" );
	int len = (int)strlen(buffer);
	DumpRecordToBuffer( &buffer[len], maxbuf-len, &s_RecordBuf[0] );

	for( i=0; i<NumRecord; i++ )
	{
		s_RecordBuf[i].accumtime = 0;
		s_RecordBuf[i].calltimes = 0;
	}
	while( !s_IDStack.empty() ) s_IDStack.pop();
	bThisFrameValid = false;
}

}//namespace Profile

