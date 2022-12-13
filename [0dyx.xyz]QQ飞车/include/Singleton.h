/*!	
	\file
			This file define the Singleton baseclass  
	\author	
			floppy
	\version
			0.1
	\date	
			2005.09.14
*/
#ifndef _SINGLETON_H
#define _SINGLETON_H

namespace QGEG
{


/*! 
	\brief
 			Who need unique inhert this class.
	\author 
			floppy input "Portions Copyright (C) Scott Bilas, 2000"
	\par	Example:
			Here is a example to use the class
	\code
			class CRenderMgr : public Singleton<CRenderMgr>
			{
				public:
					CRenderMgr(){}
					~CRenderMgr(){}
				private:
					int	m_nNumber;
					....
			}
	\endcode
*/
template <typename T> class Singleton
{
protected:
	/*! \brief The One */
	static T* ms_Singleton;

public:
	Singleton()
	{
		ms_Singleton = static_cast<T*>(this);
	}

	~Singleton()
	{
		ms_Singleton = 0;
	}

	/*! \brief Get the reference of object */
	static T& GetSingleton()
	{
		return *ms_Singleton;
	}

	/*! \brief Get the pointer to object */
	static T* GetSingletonPtr()
	{
		return ms_Singleton;
	}

};

} // End of QGEG namespace

#endif // _SINGLETON_H
