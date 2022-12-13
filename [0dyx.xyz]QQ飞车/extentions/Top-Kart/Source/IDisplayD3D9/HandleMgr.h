/*
 *	HandleMgr.h
 *  HandleMgr functions header.	handle管理器
 *
 *  Mike, 2003.12.31.
 */
#ifndef _HANDLE_MGR_H_
#define _HANDLE_MGR_H_

#include	<vector>
#include    <assert.h>

//o---------------------------------------------o
//|		HandleMgr handle管理器					|
//o---------------------------------------------o

typedef unsigned int        UINT;

template<class T>
void ReleaseRaw( T*);

template<class T>
class CHandleMgr{

	class CItem{
		friend CHandleMgr;
		T*		pT_;
		bool	isUsed_;		//本句柄空闲与否

		CItem()					:isUsed_(false), pT_(0){}
		void	Release(){
			if ( ! isUsed_)
			{
				return;
			}

			ReleaseRaw( pT_);	//define other place
			pT_ = 0;
			isUsed_ = false;
		}
	};
	std::vector<CItem> items_;

public:
	
	CHandleMgr();		
	virtual	~CHandleMgr();
	void	Release();

	int		CreateHandle( T* music);
	void	ReleaseHandle( UINT hImg);
	bool	isValidHandle( UINT h)			
	{ 
		return ( h > 0 ) && ( h < items_.size() ) && items_[h].isUsed_;
	}

	T*		GetItem( int hImg);
private:
	int		GetFreeHandle();
};

//o---------------------------------------------o
//|		CHandleMgr 句柄管理器					|
//o---------------------------------------------o
template<class T>
CHandleMgr<T>::CHandleMgr(){ 
	//保留 0 号句柄不用
	items_.push_back( CItem()); 
	items_[0].isUsed_ = true;
}

template<class T>
CHandleMgr<T>::~CHandleMgr(){
}

template<class T>
void CHandleMgr<T>::Release(){ 
	for (int i=1; i<items_.size(); i++)
	{
		items_[i].Release();
	}
}

template<class T>
int CHandleMgr<T>::GetFreeHandle(){
	for( int i=1; i<items_.size(); i++){
		if ( !items_[i].isUsed_)
		{
			return i;
		}
	}
	
	items_.push_back( CItem());

	return items_.size() - 1;
}

//	UINT h = musicMgr_.GetFreeHandle();
template<class T>
int CHandleMgr<T>::CreateHandle( T* pT){
	UINT h = GetFreeHandle();
	CItem* item = &items_[h];
	item->pT_ = pT;
	item->isUsed_ = true;
	return h;
}

template<class T>
void CHandleMgr<T>::ReleaseHandle( UINT hImg){
	if ( hImg <= 0 || hImg >= items_.size() )
	{
		return;
	}
	
	items_[hImg].Release();
};

template<class T>
T* CHandleMgr<T>::GetItem( int hImg){
	
	if ( hImg <= 0 || hImg >= items_.size() )
	{
		assert(false);		//	overflow, an invalid handle of image
		hImg = 0;
	}
	
	return items_[hImg].pT_;
}

#endif //_TEX_MGR_H_

