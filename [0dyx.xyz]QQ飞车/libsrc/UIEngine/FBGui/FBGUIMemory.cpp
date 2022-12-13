#include "FBGUIBase.h"
#include "FBGUIMemory.h"

namespace FBGUI
{
	// ********************************************************************** //
	// CGUIMemoryManager
	// ********************************************************************** //

	char*			CGUIMemoryManager::spOwnerFileName		= NULL;
	int				CGUIMemoryManager::sOwnerLineNumber		= 0;
	CGUIMemoryInfo*	CGUIMemoryManager::spInfoHead			= NULL;
	CGUIMemoryInfo*	CGUIMemoryManager::spInfoTail			= NULL;

	void CGUIMemoryManager::SetOwner( char* pFileName, int vLineNumber )
	{
		spOwnerFileName	 = pFileName;
		sOwnerLineNumber = vLineNumber;
	}

	void* CGUIMemoryManager::OperatorNew( int vSize )
	{
		void* tpObject = ::malloc( vSize );

		CGUIMemoryInfo* tpMemInfo = (CGUIMemoryInfo*) ::malloc( sizeof( CGUIMemoryInfo ) );

		tpMemInfo->mAddress		= (int) tpObject;
		tpMemInfo->mSize		= vSize;
		tpMemInfo->mpFileName	= spOwnerFileName;
		tpMemInfo->mLineNumber	= sOwnerLineNumber;
		tpMemInfo->mpPrevInfo	= NULL;
		tpMemInfo->mpNextInfo	= NULL;

		if ( spInfoHead == NULL	)
		{
			spInfoHead = tpMemInfo;
			spInfoTail = tpMemInfo;
		}
		else
		{
			spInfoTail->mpNextInfo	= tpMemInfo;
			tpMemInfo->mpPrevInfo	= spInfoTail;
			spInfoTail				= tpMemInfo;
		}

		return tpObject;
	}

	void CGUIMemoryManager::OperatorDelete( void* pObject )
	{
		::free( pObject );

		for ( CGUIMemoryInfo* tpMemInfo = spInfoHead; tpMemInfo != NULL; tpMemInfo = tpMemInfo->mpNextInfo )
		{
			if ( tpMemInfo->mAddress == (unsigned int) pObject )
			{
				CGUIMemoryInfo* tpPrevInfo = tpMemInfo->mpPrevInfo;
				CGUIMemoryInfo* tpNextInfo = tpMemInfo->mpNextInfo;

				if ( tpPrevInfo != NULL )
					tpPrevInfo->mpNextInfo = tpNextInfo;

				if ( tpNextInfo != NULL )
					tpNextInfo->mpPrevInfo = tpPrevInfo;

				if ( tpMemInfo == spInfoHead )
					spInfoHead = tpNextInfo;

				if ( tpMemInfo == spInfoTail )
					spInfoTail = tpPrevInfo;

				::free( tpMemInfo );

				break;
			}
		}
	}

	int CGUIMemoryManager::GetAllocCount( )
	{
		int tCount = 0;
		for ( CGUIMemoryInfo* tpMemInfo = spInfoHead; tpMemInfo != NULL; )
			{ tCount ++; tpMemInfo = tpMemInfo->mpNextInfo; }

		return tCount;
	}

	int CGUIMemoryManager::GetTotalSize( )
	{
		int tTotalSize = 0;
		for ( CGUIMemoryInfo* tpMemInfo = spInfoHead; tpMemInfo != NULL; )
			{ tTotalSize += tpMemInfo->mSize; tpMemInfo = tpMemInfo->mpNextInfo; }

		return tTotalSize;
	}

	int CGUIMemoryManager::GetAllocAddress( int vIndex )
	{
		int tCount = 0;
		for ( CGUIMemoryInfo* tpMemInfo = spInfoHead; tpMemInfo != NULL; )
		{ 
			if ( tCount == vIndex )
				return tpMemInfo->mAddress;

			tCount ++; 
			tpMemInfo = tpMemInfo->mpNextInfo; 
		}
		return -1;
	}

	int CGUIMemoryManager::GetAllocSize( int vIndex )
	{
		int tCount = 0;
		for ( CGUIMemoryInfo* tpMemInfo = spInfoHead; tpMemInfo != NULL; )
		{ 
			if ( tCount == vIndex )
				return tpMemInfo->mSize;

			tCount ++; 
			tpMemInfo = tpMemInfo->mpNextInfo; 
		}
		return -1;
	}

	std::string CGUIMemoryManager::GetAllocFile( int vIndex )
	{
		int tCount = 0;
		for ( CGUIMemoryInfo* tpMemInfo = spInfoHead; tpMemInfo != NULL; )
		{ 
			if ( tCount == vIndex )
				return tpMemInfo->mpFileName == NULL ? "未知文件" : tpMemInfo->mpFileName;

			tCount ++; 
			tpMemInfo = tpMemInfo->mpNextInfo; 
		}
		return "未知文件";
	}

	int CGUIMemoryManager::GetAllocLineNumber( int vIndex )
	{
		int tCount = 0;
		for ( CGUIMemoryInfo* tpMemInfo = spInfoHead; tpMemInfo != NULL; )
		{ 
			if ( tCount == vIndex )
				return tpMemInfo->mLineNumber;

			tCount ++; 
			tpMemInfo = tpMemInfo->mpNextInfo; 
		}
		return -1;
	}
}