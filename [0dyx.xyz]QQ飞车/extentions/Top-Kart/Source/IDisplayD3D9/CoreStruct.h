#ifndef CORE_STRUCT_H
#define CORE_STRUCT_H

template <class ptr>
class AptComPtr{
	ptr m_ptr;
public :
	AptPtr(){ m_ptr = NULL };
	~AptPtr(){ Release(); };
	Release(){ if(m_ptr){ m_ptr->Relase(); m_ptr = NULL; } };
	void operator = (ptr pt){ m_ptr = pt};
};

#endif