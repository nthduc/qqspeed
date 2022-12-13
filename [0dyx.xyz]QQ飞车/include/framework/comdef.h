////////////////////////////////////////////////
//     FileName:    comdef.h
//     Author:      Fish
//     Date:        2006-11-20 15:46:01
//     Description: []
////////////////////////////////////////////////

#pragma once

#ifdef DECL_IID
#undef DECL_IID
#endif

#ifdef IMPL_GUID
#undef IMPL_GUID
#endif

#ifdef DECL_CLSID
#undef DECL_CLSID
#endif

#define DECL_IID(x, s)	EXTERN_C const IID IID_##x;	struct __declspec(uuid(s)) x;
#define DECL_CLSID(x)	EXTERN_C const IID CLSID_##x;
#define IMPL_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) const GUID name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}
