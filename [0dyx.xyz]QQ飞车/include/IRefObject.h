#pragma once


class IRefObject
{
public:
	IRefObject() : m_nRefCount(1){}
	virtual ~IRefObject(){}
	void AddRef()
	{
		++m_nRefCount;
	}
    void Release()
	{
		--m_nRefCount;
		assert(m_nRefCount >= 0);
		if ( m_nRefCount <= 0)
		{
			delete this;
		}
	}
    unsigned int GetRefCount() const
	{
		return m_nRefCount;
	}    

private:
    int m_nRefCount;    
};

