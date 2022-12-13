#pragma once

#include "typemanip.h"

namespace Tenio
{
	
	//////////////////////////////////////////////////////////////////////////
	
	class NullType {};
	
	//////////////////////////////////////////////////////////////////////////
	
	template <class T, class U>
		struct Typelist
    {
		typedef T Head;
		typedef U Tail;
    };
	
	
	//////////////////////////////////////////////////////////////////////////
	
	template
        <
		typename T1  = NullType, typename T2  = NullType, typename T3  = NullType,
		typename T4  = NullType, typename T5  = NullType, typename T6  = NullType,
		typename T7  = NullType, typename T8  = NullType, typename T9  = NullType,
		typename T10 = NullType, typename T11 = NullType, typename T12 = NullType,
		typename T13 = NullType, typename T14 = NullType, typename T15 = NullType,
		typename T16 = NullType, typename T17 = NullType, typename T18 = NullType
        > 
        struct MakeTypelist
	{
	private:
		typedef typename MakeTypelist
            <
			T2 , T3 , T4 , 
			T5 , T6 , T7 , 
			T8 , T9 , T10, 
			T11, T12, T13,
			T14, T15, T16, 
			T17, T18
            >
            ::Result TailResult;
		
	public:
		typedef Typelist<T1, TailResult> Result;
	};
	
	template<>
        struct MakeTypelist<>
	{
		typedef NullType Result;
	};
	
	
	
	////////////////////////////////////////////////////////////////////////////////
	// class template is_Typelist
	// detects if type is Typelist (including Nulltype)
	// Invocation :
	// is_Typelist<T>::value
	// returns a compile-time boolean constant containing true iff T is some Typelist<T1,T2>
	// is_Typelist<T>::type_id
	// returns a compile-time unsigned constant containing 
	// 1 iff T == Typelist<T1,T2>, 2 iff T == NullType and 3 otherwise
	////////////////////////////////////////////////////////////////////////////////
	struct Typelist_tag {};
	struct NullType_tag {};
	struct NoneList_tag {};
	
	enum 
	{
		Typelist_ID = 1,
            NullType_ID = 2,
            NoneList_ID = 3
	};
	
	template<typename T>
        struct is_Typelist
	{
	private:
		typedef char (&ye1)[Typelist_ID];
		typedef char (&ye2)[NullType_ID];
		typedef char (&no) [NoneList_ID];
		
		template<typename Head, typename Tail>
            static ye1 check(Type2Type< Typelist<Head, Tail> >);
		static ye2 check(Type2Type<NullType>);
		
		static no  check(...);
		
	public:
		#if (_MSC_VER >= 1300)
		// VC7 fail NPS_HierarchyGenerators.h if this one is enum
			static const unsigned int type_id = sizeof(check( Type2Type<T>() ));
		#else
			enum { type_id = sizeof(check( Type2Type<T>() )) };
		#endif
			  
			enum { value = type_id != sizeof(no) };
			
			typedef typename Select
			  <
				  type_id == Typelist_ID, 
				  Typelist_tag,
				  typename Select<type_id == NullType_ID, NullType_tag, NoneList_tag>::Result
			  > 
			  ::Result type_tag;
	};
	
	
	
#ifndef TL_FAST_COMPILATION
	
	// this macro will cause compile time error if _type_ is not a Typelist or NullType
#define ASSERT_TYPELIST(_type_) \
	
#else
	
	// might improve the compilation time
#define ASSERT_TYPELIST(_type_) \
	
#endif // ndef TL_FAST_COMPILATION	
	////////////////////////////////////////////////////////////////////////////////
	// class template Length
	// Computes the length of a typelist
	// Invocation (TList is a typelist):
	// Length<TList>::value
	// returns a compile-time constant containing the length of TList, not counting
	//     the end terminator (which by convention is NullType)
	////////////////////////////////////////////////////////////////////////////////
	
	template <class TList> struct Length;
	
	template <> 
        struct Length<NullType>
	{
		enum { value = 0 };
	};
	
	template <class TList>
        struct Length
	{
	private:
		ASSERT_TYPELIST(TList);
		
		typedef typename TList::Head Head;
		typedef typename TList::Tail Tail;
		
	public:
		enum { value = 1 + Length<Tail>::value };
	};
	
	////////////////////////////////////////////////////////////////////////////////
	// class template TypeAt
	// Finds the type at a given index in a typelist
	// Invocation (TList is a typelist and index is a compile-time integral 
	//     constant):
	// TypeAt<TList, index>::Result
	// returns the type in position 'index' in TList
	// If you pass an out-of-bounds index, the result is a compile-time error
	////////////////////////////////////////////////////////////////////////////////
	
	template <class TList, unsigned int index> 
        struct TypeAt
	{
		typedef typename TList::Head Head;
		typedef typename TList::Tail Tail;
        
	private:
		ASSERT_TYPELIST(TList);
		
		template<unsigned int i>
            struct In
		{
			typedef typename TypeAt<Tail, i - 1>::Result Result;
		};
		
		template<>
            struct In<0>
		{
			typedef Head Result;
		};
		
	public:
		typedef typename In<index>::Result Result;
	};
	
	////////////////////////////////////////////////////////////////////////////////
	// class template TypeAtNonStrict
	// Finds the type at a given index in a typelist
	// Invocations (TList is a typelist and index is a compile-time integral 
	//     constant):
	// a) TypeAt<TList, index>::Result
	// returns the type in position 'index' in TList, or NullType if index is 
	//     out-of-bounds
	// b) TypeAt<TList, index, D>::Result
	// returns the type in position 'index' in TList, or D if index is out-of-bounds
	////////////////////////////////////////////////////////////////////////////////
	
	
	template <class TList, unsigned int index, typename DefaultType = NullType> 
        struct TypeAtNonStrict
	{
		typedef typename TList::Head Head;
		typedef typename TList::Tail Tail;
        
        private:
            ASSERT_TYPELIST(TList);
			
            template<class TList1,unsigned int i>
				struct In
            {
                typedef typename TypeAtNonStrict
					<
                    typename TList1::Tail, 
                    i - 1,
                    DefaultType
					>
					::Result Result;
            };
			
            template<>
				struct In<Typelist<Head, Tail>, 0>
            {
                typedef Head Result;
            };
			
            template<>
				struct In<NullType, index>
            {
                typedef DefaultType Result;
            };
        public:
            typedef typename In<TList, index>::Result Result;
	};
	
	////////////////////////////////////////////////////////////////////////////////
	// class template IndexOf
	// Finds the index of a type in a typelist
	// Invocation (TList is a typelist and T is a type):
	// IndexOf<TList, T>::value
	// returns the position of T in TList, or NullType if T is not found in TList
	////////////////////////////////////////////////////////////////////////////////
	
	template <class TList, class T> 
        struct IndexOf
	{
		typedef typename TList::Head Head;
		typedef typename TList::Tail Tail;
        
        private:
            ASSERT_TYPELIST(TList);
			
            template<class TList1>
				struct In
            {
		private:
			typedef typename TList1::Tail Tail;
			
			enum { temp = (IndexOf<Tail, T>::value) };
			
		public:
			enum { value = temp == -1 ? -1 : 1 + temp  };
            };
			
            template<>
				struct In< Typelist<T, Tail> >
            {
                enum { value = 0 };
            };
			
            template<>
				struct In<NullType>
            {
                enum { value = -1 };
            };
			
        public:
            enum { value = In<TList>::value };
	};
	
};