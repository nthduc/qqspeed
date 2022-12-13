////////////////////////////////////////////////
//     FileName:    singlet.h
//     Author:      Fish
//     Date:        2007-2-9 11:02:41
//     Description: 简单易用的单件实现基类
////////////////////////////////////////////////

#ifndef _SINGLET_H_
#define _SINGLET_H_

template<class T>
class CSingleT
{
public:
	//取得单件实例
	static T* Get()
	{
		return ms_pObject;
	}

	//创建单件
	static void Create()
	{
		if ( !ms_pObject )
		{
			ms_pObject = new T;
		}
		assert(ms_pObject);
	}

	//销毁单件
	static void Destroy()
	{
		if ( ms_pObject )
		{
			delete ms_pObject;
			ms_pObject = NULL;
		}
	}

protected:
	static T*	ms_pObject;
};

template<class T> 
T* CSingleT<T>::ms_pObject = NULL;

#endif