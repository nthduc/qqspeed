#ifndef _UNKNOWN_HELPER_H_
#define _UNKNOWN_HELPER_H_


#ifndef IMPLEMENT_IUNKNOWN

#define IMPLEMENT_IUNKNOWN_ADDREF(ObjectClass, InterfaceClass) \
    STDMETHODIMP_(ULONG) ObjectClass::X##InterfaceClass::AddRef(void) \
    { \
        METHOD_PROLOGUE(ObjectClass, InterfaceClass); \
        return pThis->ExternalAddRef(); \
    }

#define IMPLEMENT_IUNKNOWN_RELEASE(ObjectClass, InterfaceClass) \
    STDMETHODIMP_(ULONG) ObjectClass::X##InterfaceClass::Release(void) \
    { \
        METHOD_PROLOGUE(ObjectClass, InterfaceClass); \
        return pThis->ExternalRelease(); \
    }

#define IMPLEMENT_IUNKNOWN_QUERYINTERFACE(ObjectClass, InterfaceClass) \
    STDMETHODIMP ObjectClass::X##InterfaceClass::QueryInterface(REFIID riid, LPVOID* ppVoid) \
    { \
        METHOD_PROLOGUE(ObjectClass, InterfaceClass); \
        return (HRESULT)pThis->ExternalQueryInterface(&riid, ppVoid); \
    }

#define IMPLEMENT_IUNKNOWN(ObjectClass, InterfaceClass) \
    IMPLEMENT_IUNKNOWN_ADDREF(ObjectClass, InterfaceClass) \
    IMPLEMENT_IUNKNOWN_RELEASE(ObjectClass, InterfaceClass) \
    IMPLEMENT_IUNKNOWN_QUERYINTERFACE(ObjectClass, InterfaceClass)

#endif // IMPLEMENT_IUNKNOWN


#endif