#include "Top-KartPCH.h"
#include "profiling.h"
#include <windows.h>

unsigned __int64 GetCycleCount()
{
	__asm _emit 0FH
		__asm _emit 31H
}

inline void Profile_Get_Ticks(_int64 * ticks)
{
	__asm
	{
		push edx;
		push ecx;
		mov ecx,ticks;
		_emit 0Fh
		_emit 31h
		mov [ecx],eax;
		mov [ecx+4],edx;
		pop ecx;
		pop edx;
	}
}


inline float Profile_Get_Tick_Rate(void)
{
	static float _CPUFrequency = -1.0f;

	if (_CPUFrequency == -1.0f) {
		unsigned __int64 curr_rate = 0;
		
		//::QueryPerformanceFrequency ((LARGE_INTEGER*)&curr_rate);
		//curr_rate=1.0f; 
		curr_rate = GetCycleCount();
		::Sleep(1000);
		curr_rate = GetCycleCount() - curr_rate;
		_CPUFrequency = (float)curr_rate;
		
	} 

	return _CPUFrequency;
}

CProfileNode::CProfileNode( const char * name, CProfileNode * parent ) :
Name( name ),
TotalCalls( 0 ),
TotalTime( 0 ),
StartTime( 0 ),
RecursionCounter( 0 ),
Parent( parent ),
Child( NULL ),
Sibling( NULL )
{
	Reset();
}


CProfileNode::~CProfileNode( void )
{	
	Free(Sibling, false);
	Free(Child, true);
	//delete Child;
	//delete Sibling;
}

void CProfileNode::Free(CProfileNode *& pNode, bool bChild)
{		
	if (pNode) 
	{
		CProfileNode *& pChild = pNode->Child; 
		CProfileNode *& pSibling = pNode->Sibling;
		
		// Free Sibling		
		if (pSibling) 
		{
			Free(pSibling, false);			
		}

		// Free Child;		
		if (pChild)
		{
			Free(pChild, true);			
		}	

		NiDelete pNode;
		pNode = NULL;
	}
}

CProfileNode * CProfileNode::Get_Sub_Node( const char * name )
{
	// Try to find this sub node
	CProfileNode * child = Child;
	while ( child ) {
		if ( 0 == strcmp(child->Name, name) ) {
			return child;
		}
		child = child->Sibling;		
	}

	// We didn't find it, so add it
	CProfileNode * node = NiNew CProfileNode( name, this );
	node->Sibling = Child;
	Child = node;
	return node;
}

void CProfileNode::Reset( void )
{
	TotalCalls = 0;

	TotalTime = 0.0f;
	OneTime=0.0f;
	MaxTime=0.0f;
	MinTime=0.0f;
	AveTime=0.0f;
	ControlMaxMin=true;

	if ( Child ) {
		Child->Reset();
	}
	if ( Sibling ) {
		Sibling->Reset();
	}
}


void CProfileNode::Call( void )
{
	TotalCalls++;
	if (RecursionCounter++ == 0) {
		StartTime=GetCycleCount();
		//Profile_Get_Ticks(&StartTime);
	}
}


bool CProfileNode::Return( void )
{
	if ( --RecursionCounter == 0 && TotalCalls != 0 ) { 
		__int64 time;
		time=GetCycleCount();
		//Profile_Get_Ticks(&time);
		time-=StartTime;

		TotalTime +=(float)time / Profile_Get_Tick_Rate();
		OneTime=(float)time / Profile_Get_Tick_Rate();

		if  (ControlMaxMin==true)
		{
			MinTime=OneTime;
			MaxTime=OneTime;
			ControlMaxMin=false;
		}

		if  (MinTime>OneTime)
			MinTime=OneTime;

		if  (MaxTime<OneTime)
			MaxTime=OneTime;

		AveTime=TotalTime/TotalCalls;
		//TotalTime = (float)time / Profile_Get_Tick_Rate();
	}
	return ( RecursionCounter == 0 );
}

bool CProfileIterator::bProfileIterator=false;

CProfileIterator::CProfileIterator( CProfileNode * start )
{
	CurrentParent = start;
	CurrentChild = CurrentParent->Get_Child();
}

CProfileIterator::~CProfileIterator()
{
}

void	CProfileIterator::First(void)
{
	CurrentChild = CurrentParent->Get_Child();
	//CurrentChild = CurrentParent;
}

void	CProfileIterator::Next(void)
{
	CurrentChild = CurrentChild->Get_Sibling();
}

bool	CProfileIterator::Is_Done(void)
{
	if (CurrentChild == NULL)
		return false;
	else
		return true;
	//return CurrentChild == NULL;
}

void	CProfileIterator::Enter_Child( int index )
{
	CurrentChild = CurrentParent->Get_Child();
	//while ( (CurrentChild != NULL) && (index != 0) ) {
	while ( (CurrentChild != NULL) && (index != 0) ) {	
		index--;
		CurrentChild = CurrentChild->Get_Sibling();
	}

	if ( CurrentChild != NULL ) {
		CurrentParent = CurrentChild;
		CurrentChild = CurrentParent->Get_Child();
	}
}


void	CProfileIterator::Enter_Parent( void )
{
	if ( CurrentParent->Get_Parent() != NULL ) {
		CurrentParent = CurrentParent->Get_Parent();
	}
	//CurrentChild = CurrentParent->Get_Child();
	CurrentChild = CurrentParent->Get_Child();
}



CProfileNode *	CProfileManager::Root = NULL;
CProfileNode *	CProfileManager::CurrentNode = NULL;
int				CProfileManager::FrameCounter = 0;
__int64			CProfileManager::ResetTime = 0;

CProfileManager::CProfileManager()
{
	Root = NiNew CProfileNode( "Root", NULL );
	CurrentNode = Root;
}

CProfileManager::~CProfileManager( )
{
	NiDelete Root;
}

void	CProfileManager::Start_Profile( const char * name )
{
	if (name != CurrentNode->Get_Name()) {
		CurrentNode = CurrentNode->Get_Sub_Node( name );
	} 

	CurrentNode->Call();
}

void	CProfileManager::Stop_Profile( void )
{
	// Return will indicate whether we should back up to our parent (we may
	// be profiling a recursive function)
	if (CurrentNode->Return()) {
		CurrentNode = CurrentNode->Get_Parent();
	}
}

void	CProfileManager::Stop_ProfileWithWatchDog(float& fOneTime)
{
	// Return will indicate whether we should back up to our parent (we may
	// be profiling a recursive function)
	if (CurrentNode->Return()) {
		fOneTime = CurrentNode->Get_Max_Time();
		CurrentNode = CurrentNode->Get_Parent();
	}
}

void	CProfileManager::Reset( void )
{ 
	Root->Reset(); 
	FrameCounter = 0;
	ResetTime=GetCycleCount();
	//Profile_Get_Ticks(&ResetTime);
}

void CProfileManager::Increment_Frame_Counter( void )
{
	FrameCounter++;
}

float CProfileManager::Get_Time_Since_Reset( void )
{
	__int64 time;
	time= GetCycleCount();
	//Profile_Get_Ticks(&time);
	time -= ResetTime;
	return (float)time / Profile_Get_Tick_Rate();
}
