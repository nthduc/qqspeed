
#ifndef VEHICLE_H
#define VEHICLE_H

#include "NiObject.h"
#include "NiNode.h"

class CVehicle: public NiObject
{
public:
	//CVehicle();
	//virtual ~CVehicle();


public:
	virtual NiNodePtr GetNiRoot() = 0;
	virtual void Update(float fDeltaTime) = 0;

private:

};

#endif