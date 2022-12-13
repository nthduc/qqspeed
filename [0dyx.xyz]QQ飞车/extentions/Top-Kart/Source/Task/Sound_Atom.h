//////////////////////////////////////////////////////////////
//     FileName:    Sound_Atom.h
//     Author:      Rayhuang
//     Date:        2007-6-18 18:34:42
//     Description: [ÉùÒô²¥·ÅÔ­×Ó]
/////////////////////////////////////////////////////////////

#pragma once 
#include "AtomBase.h"

class CSound_Atom : public CAtomBase
{
public:
	CSound_Atom();
	~CSound_Atom();
	void Start();
protected:
	void _Stop();
private:
	DWORD m_dwSound;
};

