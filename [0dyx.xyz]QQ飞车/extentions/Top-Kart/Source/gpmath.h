////////////////////////////////////////////////
//     FileName:    gpmath.h
//     Author:      Fish
//     Date:        2007-1-24 9:13:51
//     Description: []
////////////////////////////////////////////////

#ifndef _GPMATH_H_
#define _GPMATH_H_

#include "math.h"
#include "NxVec3.h"

namespace GPMath
{
	double ComputeDistanceFromPointToLine(NiPoint3 p, NiPoint3 v, NiPoint3 q)
	{
		float c = (q - p) * (q - p);
		float a = pow((q - p) * v, 2) / (v * v);
		return sqrt(c - a);
	}

	NiPoint3 ComputerNearestPointFromPointToLine(NiPoint3 p, NiPoint3 v, NiPoint3 q)
	{
		NiPoint3 proj = v * (q - p).Dot(v);
		return p + proj;
	}

	double ComputeAngleBetweenLines(NiPoint3 v1, NiPoint3 v2)
	{
		v1.Unitize();
		v2.Unitize();
		double angel = acos(v1 * v2);
		return angel;
	}
};
#endif