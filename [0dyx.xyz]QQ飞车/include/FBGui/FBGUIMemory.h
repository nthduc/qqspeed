#pragma once

namespace FBGUI
{

	namespace Exception
	{

		// ********************************************************************** //
		// CException
		// ********************************************************************** //

		class CGUIException
		{
		public:
			char*	mpErrorDesc;
			char*	mpFileName;
			int		mLineNumber;

			CGUIException( char* pFileName, char* pErrorDesc, int vLineNumber )
				: mpFileName( pFileName ), mpErrorDesc( pErrorDesc ), mLineNumber( vLineNumber ) { }
		};

		#define GUI_EXCEPTION( DESC )				{ throw new CGUIException( __FILE__, DESC, __LINE__ ); }
		#define GUI_ASSERT( x, DESC )				{ if ( !( x ) ) throw new CGUIException( __FILE__, DESC, __LINE__ ); }

	};

	namespace Memory
	{

		// ********************************************************************** //
		// CGUIMemoryInfo
		// ********************************************************************** //

		class CGUIMemoryInfo
		{
		public:
			int				mAddress;
			int				mSize;
			char*			mpFileName;
			int				mLineNumber;
			CGUIMemoryInfo*	mpPrevInfo;
			CGUIMemoryInfo*	mpNextInfo;
		};

		// ********************************************************************** //
		// CMemoryManager
		// ********************************************************************** //

		class CGUIMemoryManager
		{
		protected:
			static char*			spOwnerFileName;
			static int				sOwnerLineNumber;
			static CGUIMemoryInfo*	spInfoHead;
			static CGUIMemoryInfo*	spInfoTail;

		public:
			static void  SetOwner( char* pFileName, int vLineNumber );
			static void* OperatorNew( int vSize );
			static void  OperatorDelete( void* pObject );
			static int GetTotalSize( );
			static int GetAllocCount( );
			static int GetAllocAddress( int vIndex );
			static int GetAllocSize( int vIndex );
			static std::string GetAllocFile( int vIndex );
			static int GetAllocLineNumber( int vIndex );
		};

	};

};

#ifdef FBGUI_MEM_DIAGNOSE

// ********************************************************************** //
// Operators
// ********************************************************************** //

inline void* __cdecl operator new ( size_t vSize )
	{ return FBGUI::Memory::CGUIMemoryManager::OperatorNew( (int) vSize ); }

inline void* __cdecl operator new[] ( size_t vSize )
	{ return FBGUI::Memory::CGUIMemoryManager::OperatorNew( (int) vSize ); }

inline void __cdecl operator delete ( void* pObject )
	{ FBGUI::Memory::CGUIMemoryManager::OperatorDelete( pObject ); }

inline void __cdecl operator delete[] ( void* pObject )
	{ FBGUI::Memory::CGUIMemoryManager::OperatorDelete( pObject ); }

#define new ( FBGUI::Memory::CGUIMemoryManager::SetOwner( __FILE__, __LINE__ ), false ) ? 0 : new

#endif