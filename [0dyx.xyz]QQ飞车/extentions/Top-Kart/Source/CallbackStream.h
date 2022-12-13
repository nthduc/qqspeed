// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2006 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef CALLBACKSTREAM_H
#define CALLBACKSTREAM_H

#include <NiStream.h>

class NiAVObject;
interface INifLoaderEvent;

class CallbackStream : public NiStream
{
public:
    virtual void BackgroundLoadOnExit();

	CallbackStream(INifLoaderEvent* pEvent, DWORD dwCookie);

	friend class CNifLoader;
protected:
    void Precache(NiAVObject* pkObject);

private:
	INifLoaderEvent*	m_pEvent;
	DWORD				m_dwCookie;
};

#endif // CALLBACKSTREAM_H
