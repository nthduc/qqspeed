/********************************************************************
	作者:	　wincoqin
	时间:	  2007/04/13
    文件名:   ITerrainEffect.h
	说明:     []
*********************************************************************/
#pragma once

#include "TerrainEffectDef.h"


class ITerrainPropEffect
{
public:
	virtual bool	    InitPropEffect(TerrainPropEffect * pTerrainPropEffect) = 0;
	virtual void		Update(float fDeltaTime)    = 0;
	virtual void		StartTerrainEffect()		= 0;
	virtual void		StopTerrainEffect()			= 0;
	virtual bool		IsPropEffectFinished()		= 0;
	virtual const int	GetTerrainPropEffectType()	= 0;
};