
#pragma once

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
	// class template Length
	// Computes the length of a typelist
	// Invocation (TList is a typelist):
	// Length<TList>::value
	// returns a compile-time constant containing the length of TList, not counting
	//     the end terminator (which by convention is NullType)
	////////////////////////////////////////////////////////////////////////////////
	
	template <class TList> struct Length;
	template <> struct Length<NullType>
	{
		enum { value = 0 };
	};
	
	template <class T, class U>
        struct Length< Typelist<T, U> >
	{
		enum { value = 1 + Length<U>::value };
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
	
	template <class TList, unsigned int index> struct TypeAt;
	
	template <class Head, class Tail>
        struct TypeAt<Typelist<Head, Tail>, 0>
	{
		typedef Head Result;
	};
	
	template <class Head, class Tail, unsigned int i>
        struct TypeAt<Typelist<Head, Tail>, i>
	{
		typedef typename TypeAt<Tail, i - 1>::Result Result;
	};
	
	////////////////////////////////////////////////////////////////////////////////
	// class template IndexOf
	// Finds the index of a type in a typelist
	// Invocation (TList is a typelist and T is a type):
	// IndexOf<TList, T>::value
	// returns the position of T in TList, or NullType if T is not found in TList
	////////////////////////////////////////////////////////////////////////////////
	
	template <class TList, class T> struct IndexOf;
	
	template <class T>
		struct IndexOf<NullType, T>
	{
		enum { value = -1 };
	};
	
	template <class T, class Tail>
		struct IndexOf<Typelist<T, Tail>, T>
	{
		enum { value = 0 };
	};
	
	template <class Head, class Tail, class T>
		struct IndexOf<Typelist<Head, Tail>, T>
	{
		private:
			enum { temp = IndexOf<Tail, T>::value };
		public:
			enum { value = (temp == -1 ? -1 : 1 + temp) };
	};
	
	
};