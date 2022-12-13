template <class T>
class ResRichContainer
{
private :
	typedef T*  elem_type;
	typedef int size_type;

	elem_type   _date;
	size_type   _size;

public :
	elem_type Ptr(){return _date;};
	size_type Size(){return _size;};
	elem_type* PtrPtr(){return &_date;};
	void      SetSize(int n){_size = n;};

	~ResRichContainer(){delete _date; _date = NULL; _size = 0;};
	ResRichContainer(){_date = NULL; _size = 0;};
};

extern IGameSystem * g_pGameSystem;

template <class T>
bool LoadBin(const char* bin,ResRichContainer<T>& data)
{
	int pInBuff,pInUit;
	int re = g_pGameSystem->LoadBin((void**)(data.PtrPtr()),bin,&pInBuff,&pInUit);
	data.SetSize(pInBuff / pInUit);

	return re ? false : true;
}
