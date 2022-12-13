////////////////////////////////////////////////
//     FileName:    LockKey_Atom.h
//     Author:      rayhuang
//     Date:        2007-5-28
//     Description: À¯º¸≈Ã ‰»Î‘≠◊”
////////////////////////////////////////////////

#pragma once 
#include "AtomBase.h"

class CLockKey_Atom: public CAtomBase
{
public:
	CLockKey_Atom();
	~CLockKey_Atom();
	void Start();
protected:
	void _Stop();
};