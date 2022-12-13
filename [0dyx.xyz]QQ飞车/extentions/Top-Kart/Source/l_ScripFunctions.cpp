/*
** Lua binding: ScripFunctions
** Generated automatically by tolua++-1.0.4 on 04/05/07 19:30:51.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int tolua_ScripFunctions_open (lua_State* tolua_S);

#pragma warning(disable : 4800) //forcing value to bool warning
#include "ScriptVm.h"
#include "Setting.h"
#include "GPSettings.h"
#include "DescConstant.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_FLOAT2 (lua_State* tolua_S)
{
 FLOAT2* self = (FLOAT2*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_stSendPkg (lua_State* tolua_S)
{
 stSendPkg* self = (stSendPkg*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}

static int tolua_collect_stKarBaseParam (lua_State* tolua_S)
{
 stKarBaseParam* self = (stKarBaseParam*) tolua_tousertype(tolua_S,1,0);
 delete self;
 return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"FLOAT2");
 tolua_usertype(tolua_S,"stKartCollisionParam");
 tolua_usertype(tolua_S,"stKarBaseParam");
 tolua_usertype(tolua_S,"FLOAT3");
 tolua_usertype(tolua_S,"stSendPkg");
 tolua_usertype(tolua_S,"stFriction");
 tolua_usertype(tolua_S,"stSuspension");
}

/* function: lua_break */
static int tolua_ScripFunctions_lua_break00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
 !tolua_isstring(tolua_S,1,0,&tolua_err) ||
 !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
 goto tolua_lerror;
 else
#endif
 {
  const char* sDebugInfo = ((const char*)  tolua_tostring(tolua_S,1,0));
 {
  lua_break(sDebugInfo);
 }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_break'.",&tolua_err);
 return 0;
#endif
}

/* get function: ClConfig::window_width */
static int tolua_get_ClConfig_window_width(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)ClConfig::window_width);
 return 1;
}

/* set function: ClConfig::window_width */
static int tolua_set_ClConfig_window_width(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  ClConfig::window_width = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: ClConfig::window_height */
static int tolua_get_ClConfig_window_height(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)ClConfig::window_height);
 return 1;
}

/* set function: ClConfig::window_height */
static int tolua_set_ClConfig_window_height(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  ClConfig::window_height = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::nearDistance */
static int tolua_get_GPSettings_nearDistance(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::nearDistance);
 return 1;
}

/* set function: GPSettings::nearDistance */
static int tolua_set_GPSettings_nearDistance(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::nearDistance = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::nearAngel */
static int tolua_get_GPSettings_nearAngel(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::nearAngel);
 return 1;
}

/* set function: GPSettings::nearAngel */
static int tolua_set_GPSettings_nearAngel(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::nearAngel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::farDistance */
static int tolua_get_GPSettings_farDistance(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::farDistance);
 return 1;
}

/* set function: GPSettings::farDistance */
static int tolua_set_GPSettings_farDistance(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::farDistance = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::farAngel */
static int tolua_get_GPSettings_farAngel(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::farAngel);
 return 1;
}

/* set function: GPSettings::farAngel */
static int tolua_set_GPSettings_farAngel(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::farAngel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::exchangetime */
static int tolua_get_GPSettings_exchangetime(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::exchangetime);
 return 1;
}

/* set function: GPSettings::exchangetime */
static int tolua_set_GPSettings_exchangetime(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::exchangetime = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimernode */
static int tolua_get_GPSettings_aimernode(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimernode);
 return 1;
}

/* set function: GPSettings::aimernode */
static int tolua_set_GPSettings_aimernode(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimernode = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimednode */
static int tolua_get_GPSettings_aimednode(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimednode);
 return 1;
}

/* set function: GPSettings::aimednode */
static int tolua_set_GPSettings_aimednode(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimednode = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimboardx */
static int tolua_get_GPSettings_aimboardx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimboardx);
 return 1;
}

/* set function: GPSettings::aimboardx */
static int tolua_set_GPSettings_aimboardx(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimboardx = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimedboardx */
static int tolua_get_GPSettings_aimedboardx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimedboardx);
 return 1;
}

/* set function: GPSettings::aimedboardx */
static int tolua_set_GPSettings_aimedboardx(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimedboardx = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimoffsetx */
static int tolua_get_GPSettings_aimoffsetx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimoffsetx);
 return 1;
}

/* set function: GPSettings::aimoffsetx */
static int tolua_set_GPSettings_aimoffsetx(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimoffsetx = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimoffsety */
static int tolua_get_GPSettings_aimoffsety(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimoffsety);
 return 1;
}

/* set function: GPSettings::aimoffsety */
static int tolua_set_GPSettings_aimoffsety(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimoffsety = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimoffsetz */
static int tolua_get_GPSettings_aimoffsetz(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimoffsetz);
 return 1;
}

/* set function: GPSettings::aimoffsetz */
static int tolua_set_GPSettings_aimoffsetz(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimoffsetz = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimedmex */
static int tolua_get_GPSettings_aimedmex(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimedmex);
 return 1;
}

/* set function: GPSettings::aimedmex */
static int tolua_set_GPSettings_aimedmex(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimedmex = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimedmey */
static int tolua_get_GPSettings_aimedmey(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimedmey);
 return 1;
}

/* set function: GPSettings::aimedmey */
static int tolua_set_GPSettings_aimedmey(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimedmey = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimedmez */
static int tolua_get_GPSettings_aimedmez(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimedmez);
 return 1;
}

/* set function: GPSettings::aimedmez */
static int tolua_set_GPSettings_aimedmez(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimedmez = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimedoffsetx */
static int tolua_get_GPSettings_aimedoffsetx(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimedoffsetx);
 return 1;
}

/* set function: GPSettings::aimedoffsetx */
static int tolua_set_GPSettings_aimedoffsetx(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimedoffsetx = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimedoffsety */
static int tolua_get_GPSettings_aimedoffsety(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimedoffsety);
 return 1;
}

/* set function: GPSettings::aimedoffsety */
static int tolua_set_GPSettings_aimedoffsety(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimedoffsety = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimedoffsetz */
static int tolua_get_GPSettings_aimedoffsetz(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimedoffsetz);
 return 1;
}

/* set function: GPSettings::aimedoffsetz */
static int tolua_set_GPSettings_aimedoffsetz(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimedoffsetz = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimeddistance */
static int tolua_get_GPSettings_aimeddistance(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimeddistance);
 return 1;
}

/* set function: GPSettings::aimeddistance */
static int tolua_set_GPSettings_aimeddistance(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimeddistance = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimrotspeed */
static int tolua_get_GPSettings_aimrotspeed(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimrotspeed);
 return 1;
}

/* set function: GPSettings::aimrotspeed */
static int tolua_set_GPSettings_aimrotspeed(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimrotspeed = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::aimscalespeed */
static int tolua_get_GPSettings_aimscalespeed(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::aimscalespeed);
 return 1;
}

/* set function: GPSettings::aimscalespeed */
static int tolua_set_GPSettings_aimscalespeed(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::aimscalespeed = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gottenshownode */
static int tolua_get_GPSettings_gottenshownode(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gottenshownode);
 return 1;
}

/* set function: GPSettings::gottenshownode */
static int tolua_set_GPSettings_gottenshownode(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gottenshownode = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::throwanimation */
static int tolua_get_GPSettings_throwanimation(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::throwanimation);
 return 1;
}

/* set function: GPSettings::throwanimation */
static int tolua_set_GPSettings_throwanimation(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::throwanimation = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::watchcameraspeed */
static int tolua_get_GPSettings_watchcameraspeed(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::watchcameraspeed);
 return 1;
}

/* set function: GPSettings::watchcameraspeed */
static int tolua_set_GPSettings_watchcameraspeed(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::watchcameraspeed = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::eatsound */
static int tolua_get_GPSettings_eatsound(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::eatsound);
 return 1;
}

/* set function: GPSettings::eatsound */
static int tolua_set_GPSettings_eatsound(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::eatsound = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::eatensound */
static int tolua_get_GPSettings_eatensound(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::eatensound);
 return 1;
}

/* set function: GPSettings::eatensound */
static int tolua_set_GPSettings_eatensound(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::eatensound = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gottendelay */
static int tolua_get_GPSettings_gottendelay(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gottendelay);
 return 1;
}

/* set function: GPSettings::gottendelay */
static int tolua_set_GPSettings_gottendelay(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gottendelay = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpPropPointRadius */
static int tolua_get_GPSettings_gpPropPointRadius(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpPropPointRadius);
 return 1;
}

/* set function: GPSettings::gpPropPointRadius */
static int tolua_set_GPSettings_gpPropPointRadius(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpPropPointRadius = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpStaticBoxFloatToGroud */
static int tolua_get_GPSettings_gpStaticBoxFloatToGroud(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpStaticBoxFloatToGroud);
 return 1;
}

/* set function: GPSettings::gpStaticBoxFloatToGroud */
static int tolua_set_GPSettings_gpStaticBoxFloatToGroud(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpStaticBoxFloatToGroud = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpMagnetAcceForce */
static int tolua_get_GPSettings_gpMagnetAcceForce(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpMagnetAcceForce);
 return 1;
}

/* set function: GPSettings::gpMagnetAcceForce */
static int tolua_set_GPSettings_gpMagnetAcceForce(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpMagnetAcceForce = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpMagnetTargetModifyParam */
static int tolua_get_GPSettings_gpMagnetTargetModifyParam(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpMagnetTargetModifyParam);
 return 1;
}

/* set function: GPSettings::gpMagnetTargetModifyParam */
static int tolua_set_GPSettings_gpMagnetTargetModifyParam(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpMagnetTargetModifyParam = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpMagnetSlackSlipFriction */
static int tolua_get_GPSettings_gpMagnetSlackSlipFriction(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpMagnetSlackSlipFriction);
 return 1;
}

/* set function: GPSettings::gpMagnetSlackSlipFriction */
static int tolua_set_GPSettings_gpMagnetSlackSlipFriction(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpMagnetSlackSlipFriction = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpMissileFloatToGround */
static int tolua_get_GPSettings_gpMissileFloatToGround(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpMissileFloatToGround);
 return 1;
}

/* set function: GPSettings::gpMissileFloatToGround */
static int tolua_set_GPSettings_gpMissileFloatToGround(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpMissileFloatToGround = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpMissileHelixRadius */
static int tolua_get_GPSettings_gpMissileHelixRadius(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpMissileHelixRadius);
 return 1;
}

/* set function: GPSettings::gpMissileHelixRadius */
static int tolua_set_GPSettings_gpMissileHelixRadius(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpMissileHelixRadius = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpMissileHelixCircleCount */
static int tolua_get_GPSettings_gpMissileHelixCircleCount(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpMissileHelixCircleCount);
 return 1;
}

/* set function: GPSettings::gpMissileHelixCircleCount */
static int tolua_set_GPSettings_gpMissileHelixCircleCount(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpMissileHelixCircleCount = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpFlyingFloatToGround */
static int tolua_get_GPSettings_gpFlyingFloatToGround(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpFlyingFloatToGround);
 return 1;
}

/* set function: GPSettings::gpFlyingFloatToGround */
static int tolua_set_GPSettings_gpFlyingFloatToGround(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpFlyingFloatToGround = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpFlyingHelixMaxRadius */
static int tolua_get_GPSettings_gpFlyingHelixMaxRadius(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpFlyingHelixMaxRadius);
 return 1;
}

/* set function: GPSettings::gpFlyingHelixMaxRadius */
static int tolua_set_GPSettings_gpFlyingHelixMaxRadius(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpFlyingHelixMaxRadius = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpFlyingHelixMinRadius */
static int tolua_get_GPSettings_gpFlyingHelixMinRadius(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpFlyingHelixMinRadius);
 return 1;
}

/* set function: GPSettings::gpFlyingHelixMinRadius */
static int tolua_set_GPSettings_gpFlyingHelixMinRadius(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpFlyingHelixMinRadius = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpFlyingHelixSideMaxAngle */
static int tolua_get_GPSettings_gpFlyingHelixSideMaxAngle(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpFlyingHelixSideMaxAngle);
 return 1;
}

/* set function: GPSettings::gpFlyingHelixSideMaxAngle */
static int tolua_set_GPSettings_gpFlyingHelixSideMaxAngle(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpFlyingHelixSideMaxAngle = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpFlyingHelixSideMinAngle */
static int tolua_get_GPSettings_gpFlyingHelixSideMinAngle(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpFlyingHelixSideMinAngle);
 return 1;
}

/* set function: GPSettings::gpFlyingHelixSideMinAngle */
static int tolua_set_GPSettings_gpFlyingHelixSideMinAngle(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpFlyingHelixSideMinAngle = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpFlyingTurnMaxCount */
static int tolua_get_GPSettings_gpFlyingTurnMaxCount(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpFlyingTurnMaxCount);
 return 1;
}

/* set function: GPSettings::gpFlyingTurnMaxCount */
static int tolua_set_GPSettings_gpFlyingTurnMaxCount(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpFlyingTurnMaxCount = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GPSettings::gpFlyingTurnMinCount */
static int tolua_get_GPSettings_gpFlyingTurnMinCount(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GPSettings::gpFlyingTurnMinCount);
 return 1;
}

/* set function: GPSettings::gpFlyingTurnMinCount */
static int tolua_set_GPSettings_gpFlyingTurnMinCount(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GPSettings::gpFlyingTurnMinCount = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: x of class  FLOAT2 */
static int tolua_get_FLOAT2_x(lua_State* tolua_S)
{
  FLOAT2* self = (FLOAT2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}

/* set function: x of class  FLOAT2 */
static int tolua_set_FLOAT2_x(lua_State* tolua_S)
{
  FLOAT2* self = (FLOAT2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: y of class  FLOAT2 */
static int tolua_get_FLOAT2_y(lua_State* tolua_S)
{
  FLOAT2* self = (FLOAT2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}

/* set function: y of class  FLOAT2 */
static int tolua_set_FLOAT2_y(lua_State* tolua_S)
{
  FLOAT2* self = (FLOAT2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: x of class  FLOAT3 */
static int tolua_get_FLOAT3_x(lua_State* tolua_S)
{
  FLOAT3* self = (FLOAT3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}

/* set function: x of class  FLOAT3 */
static int tolua_set_FLOAT3_x(lua_State* tolua_S)
{
  FLOAT3* self = (FLOAT3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: y of class  FLOAT3 */
static int tolua_get_FLOAT3_y(lua_State* tolua_S)
{
  FLOAT3* self = (FLOAT3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}

/* set function: y of class  FLOAT3 */
static int tolua_set_FLOAT3_y(lua_State* tolua_S)
{
  FLOAT3* self = (FLOAT3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: z of class  FLOAT3 */
static int tolua_get_FLOAT3_z(lua_State* tolua_S)
{
  FLOAT3* self = (FLOAT3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}

/* set function: z of class  FLOAT3 */
static int tolua_set_FLOAT3_z(lua_State* tolua_S)
{
  FLOAT3* self = (FLOAT3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: vMass of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_vMass(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vMass'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->vMass);
 return 1;
}

/* set function: vMass of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_vMass(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vMass'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vMass = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: vDigitalSteeringDelta of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_vDigitalSteeringDelta(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vDigitalSteeringDelta'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->vDigitalSteeringDelta);
 return 1;
}

/* set function: vDigitalSteeringDelta of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_vDigitalSteeringDelta(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vDigitalSteeringDelta'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vDigitalSteeringDelta = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: vSteeringMaxAngle of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_vSteeringMaxAngle(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vSteeringMaxAngle'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->vSteeringMaxAngle);
 return 1;
}

/* set function: vSteeringMaxAngle of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_vSteeringMaxAngle(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vSteeringMaxAngle'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vSteeringMaxAngle = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: vMoterForce of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_vMoterForce(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vMoterForce'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->vMoterForce);
 return 1;
}

/* set function: vMoterForce of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_vMoterForce(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vMoterForce'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vMoterForce = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: vMaxVelocity of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_vMaxVelocity(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vMaxVelocity'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->vMaxVelocity);
 return 1;
}

/* set function: vMaxVelocity of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_vMaxVelocity(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vMaxVelocity'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vMaxVelocity = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: vCameraDistance of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_vCameraDistance(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vCameraDistance'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->vCameraDistance);
 return 1;
}

/* set function: vCameraDistance of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_vCameraDistance(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vCameraDistance'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vCameraDistance = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: vTransmissionEfficiency of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_vTransmissionEfficiency(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vTransmissionEfficiency'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->vTransmissionEfficiency);
 return 1;
}

/* set function: vTransmissionEfficiency of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_vTransmissionEfficiency(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vTransmissionEfficiency'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vTransmissionEfficiency = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: vDifferentialRatio of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_vDifferentialRatio(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vDifferentialRatio'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->vDifferentialRatio);
 return 1;
}

/* set function: vDifferentialRatio of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_vDifferentialRatio(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'vDifferentialRatio'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->vDifferentialRatio = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: motorMinRpmToGearDown of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_motorMinRpmToGearDown(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'motorMinRpmToGearDown'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->motorMinRpmToGearDown);
 return 1;
}

/* set function: motorMinRpmToGearDown of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_motorMinRpmToGearDown(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'motorMinRpmToGearDown'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->motorMinRpmToGearDown = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: motorMaxRpmToGearUp of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_motorMaxRpmToGearUp(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'motorMaxRpmToGearUp'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->motorMaxRpmToGearUp);
 return 1;
}

/* set function: motorMaxRpmToGearUp of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_motorMaxRpmToGearUp(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'motorMaxRpmToGearUp'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->motorMaxRpmToGearUp = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: motorMinRpm of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_motorMinRpm(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'motorMinRpm'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->motorMinRpm);
 return 1;
}

/* set function: motorMinRpm of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_motorMinRpm(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'motorMinRpm'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->motorMinRpm = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: motorMaxRpm of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_motorMaxRpm(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'motorMaxRpm'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->motorMaxRpm);
 return 1;
}

/* set function: motorMaxRpm of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_motorMaxRpm(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'motorMaxRpm'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->motorMaxRpm = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: gearNbForwardGears of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_unsigned_gearNbForwardGears(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gearNbForwardGears'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->gearNbForwardGears);
 return 1;
}

/* set function: gearNbForwardGears of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_unsigned_gearNbForwardGears(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gearNbForwardGears'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->gearNbForwardGears = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: gearBackwardGearRatio of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_gearBackwardGearRatio(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gearBackwardGearRatio'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->gearBackwardGearRatio);
 return 1;
}

/* set function: gearBackwardGearRatio of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_gearBackwardGearRatio(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gearBackwardGearRatio'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->gearBackwardGearRatio = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: gearForwardGearRatios of class  stKarBaseParam */
static int tolua_get_ScripFunctions_stKarBaseParam_gearForwardGearRatios(lua_State* tolua_S)
{
 int tolua_index;
  stKarBaseParam* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (stKarBaseParam*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=32)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->gearForwardGearRatios[tolua_index]);
 return 1;
}

/* set function: gearForwardGearRatios of class  stKarBaseParam */
static int tolua_set_ScripFunctions_stKarBaseParam_gearForwardGearRatios(lua_State* tolua_S)
{
 int tolua_index;
  stKarBaseParam* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (stKarBaseParam*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=32)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->gearForwardGearRatios[tolua_index] = ((float)  tolua_tonumber(tolua_S,3,0));
 return 0;
}

/* get function: wheelApproximation of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_unsigned_wheelApproximation(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelApproximation'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelApproximation);
 return 1;
}

/* set function: wheelApproximation of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_unsigned_wheelApproximation(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelApproximation'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelApproximation = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: wheelWidth of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_wheelWidth(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelWidth'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelWidth);
 return 1;
}

/* set function: wheelWidth of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_wheelWidth(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelWidth'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelWidth = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: wheelSuspension of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_wheelSuspension(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelSuspension'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelSuspension);
 return 1;
}

/* set function: wheelSuspension of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_wheelSuspension(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelSuspension'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelSuspension = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: wheelSpringRestitution of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_wheelSpringRestitution(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelSpringRestitution'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelSpringRestitution);
 return 1;
}

/* set function: wheelSpringRestitution of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_wheelSpringRestitution(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelSpringRestitution'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelSpringRestitution = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: wheelSpringDamping of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_wheelSpringDamping(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelSpringDamping'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelSpringDamping);
 return 1;
}

/* set function: wheelSpringDamping of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_wheelSpringDamping(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelSpringDamping'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelSpringDamping = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: wheelSpringBias of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_wheelSpringBias(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelSpringBias'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelSpringBias);
 return 1;
}

/* set function: wheelSpringBias of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_wheelSpringBias(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelSpringBias'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelSpringBias = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: wheelmaxBrakeForce of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_wheelmaxBrakeForce(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelmaxBrakeForce'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelmaxBrakeForce);
 return 1;
}

/* set function: wheelmaxBrakeForce of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_wheelmaxBrakeForce(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelmaxBrakeForce'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelmaxBrakeForce = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: wheelFrictionToFront of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_wheelFrictionToFront(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelFrictionToFront'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelFrictionToFront);
 return 1;
}

/* set function: wheelFrictionToFront of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_wheelFrictionToFront(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelFrictionToFront'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelFrictionToFront = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: wheelFrictionToSide1 of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_wheelFrictionToSide1(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelFrictionToSide1'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelFrictionToSide1);
 return 1;
}

/* set function: wheelFrictionToSide1 of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_wheelFrictionToSide1(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelFrictionToSide1'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelFrictionToSide1 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: wheelFrictionToSide2 of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_wheelFrictionToSide2(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelFrictionToSide2'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->wheelFrictionToSide2);
 return 1;
}

/* set function: wheelFrictionToSide2 of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_wheelFrictionToSide2(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'wheelFrictionToSide2'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->wheelFrictionToSide2 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: radiusCoe1 of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_radiusCoe1(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'radiusCoe1'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->radiusCoe1);
 return 1;
}

/* set function: radiusCoe1 of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_radiusCoe1(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'radiusCoe1'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->radiusCoe1 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: radiusCoe2 of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_radiusCoe2(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'radiusCoe2'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->radiusCoe2);
 return 1;
}

/* set function: radiusCoe2 of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_radiusCoe2(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'radiusCoe2'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->radiusCoe2 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: radiusCoe3 of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_radiusCoe3(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'radiusCoe3'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->radiusCoe3);
 return 1;
}

/* set function: radiusCoe3 of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_radiusCoe3(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'radiusCoe3'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->radiusCoe3 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: radiusCoe4 of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_radiusCoe4(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'radiusCoe4'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->radiusCoe4);
 return 1;
}

/* set function: radiusCoe4 of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_radiusCoe4(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'radiusCoe4'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->radiusCoe4 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: radiusCoe5 of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_radiusCoe5(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'radiusCoe5'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->radiusCoe5);
 return 1;
}

/* set function: radiusCoe5 of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_radiusCoe5(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'radiusCoe5'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->radiusCoe5 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: torqueCurCoe of class  stKarBaseParam */
static int tolua_get_ScripFunctions_stKarBaseParam_torqueCurCoe(lua_State* tolua_S)
{
 int tolua_index;
  stKarBaseParam* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (stKarBaseParam*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=6)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->torqueCurCoe[tolua_index],"FLOAT2");
 return 1;
}

/* set function: torqueCurCoe of class  stKarBaseParam */
static int tolua_set_ScripFunctions_stKarBaseParam_torqueCurCoe(lua_State* tolua_S)
{
 int tolua_index;
  stKarBaseParam* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (stKarBaseParam*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=6)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->torqueCurCoe[tolua_index] = *((FLOAT2*)  tolua_tousertype(tolua_S,3,0));
 return 0;
}

/* get function: m_fVMax_NormalAccel of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fVMax_NormalAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fVMax_NormalAccel'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fVMax_NormalAccel);
 return 1;
}

/* set function: m_fVMax_NormalAccel of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fVMax_NormalAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fVMax_NormalAccel'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fVMax_NormalAccel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fVMax_UseItemAccel of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fVMax_UseItemAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fVMax_UseItemAccel'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fVMax_UseItemAccel);
 return 1;
}

/* set function: m_fVMax_UseItemAccel of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fVMax_UseItemAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fVMax_UseItemAccel'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fVMax_UseItemAccel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fVTopAccel_NormalAccel of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fVTopAccel_NormalAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fVTopAccel_NormalAccel'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fVTopAccel_NormalAccel);
 return 1;
}

/* set function: m_fVTopAccel_NormalAccel of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fVTopAccel_NormalAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fVTopAccel_NormalAccel'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fVTopAccel_NormalAccel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fVTopAccel_UseItemAccel of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fVTopAccel_UseItemAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fVTopAccel_UseItemAccel'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fVTopAccel_UseItemAccel);
 return 1;
}

/* set function: m_fVTopAccel_UseItemAccel of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fVTopAccel_UseItemAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fVTopAccel_UseItemAccel'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fVTopAccel_UseItemAccel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAcel_OriForward of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fAcel_OriForward(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_OriForward'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAcel_OriForward);
 return 1;
}

/* set function: m_fAcel_OriForward of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fAcel_OriForward(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_OriForward'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAcel_OriForward = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAcel_OriBack of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fAcel_OriBack(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_OriBack'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAcel_OriBack);
 return 1;
}

/* set function: m_fAcel_OriBack of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fAcel_OriBack(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_OriBack'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAcel_OriBack = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAcelMax_NormalAccel of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fAcelMax_NormalAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcelMax_NormalAccel'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAcelMax_NormalAccel);
 return 1;
}

/* set function: m_fAcelMax_NormalAccel of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fAcelMax_NormalAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcelMax_NormalAccel'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAcelMax_NormalAccel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAcelMax_UseItemAccel of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fAcelMax_UseItemAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcelMax_UseItemAccel'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAcelMax_UseItemAccel);
 return 1;
}

/* set function: m_fAcelMax_UseItemAccel of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fAcelMax_UseItemAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcelMax_UseItemAccel'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAcelMax_UseItemAccel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fDeAcelMax of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fDeAcelMax(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fDeAcelMax'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fDeAcelMax);
 return 1;
}

/* set function: m_fDeAcelMax of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fDeAcelMax(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fDeAcelMax'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fDeAcelMax = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAcel_Vfu_NormalAccel of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fAcel_Vfu_NormalAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_Vfu_NormalAccel'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAcel_Vfu_NormalAccel);
 return 1;
}

/* set function: m_fAcel_Vfu_NormalAccel of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fAcel_Vfu_NormalAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_Vfu_NormalAccel'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAcel_Vfu_NormalAccel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAcel_Vfu_UseItemAccel of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fAcel_Vfu_UseItemAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_Vfu_UseItemAccel'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAcel_Vfu_UseItemAccel);
 return 1;
}

/* set function: m_fAcel_Vfu_UseItemAccel of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fAcel_Vfu_UseItemAccel(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_Vfu_UseItemAccel'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAcel_Vfu_UseItemAccel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fVMin of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fVMin(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fVMin'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fVMin);
 return 1;
}

/* set function: m_fVMin of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fVMin(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fVMin'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fVMin = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAcel_NonePress of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fAcel_NonePress(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_NonePress'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAcel_NonePress);
 return 1;
}

/* set function: m_fAcel_NonePress of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fAcel_NonePress(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_NonePress'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAcel_NonePress = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAcel_Vfu_NonePress of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fAcel_Vfu_NonePress(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_Vfu_NonePress'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAcel_Vfu_NonePress);
 return 1;
}

/* set function: m_fAcel_Vfu_NonePress of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fAcel_Vfu_NonePress(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_Vfu_NonePress'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAcel_Vfu_NonePress = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAcel_AcelToNormal of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_m_fAcel_AcelToNormal(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_AcelToNormal'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAcel_AcelToNormal);
 return 1;
}

/* set function: m_fAcel_AcelToNormal of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_m_fAcel_AcelToNormal(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAcel_AcelToNormal'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAcel_AcelToNormal = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: bDreamDrifting of class  stKarBaseParam */
static int tolua_get_stKarBaseParam_unsigned_bDreamDrifting(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bDreamDrifting'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->bDreamDrifting);
 return 1;
}

/* set function: bDreamDrifting of class  stKarBaseParam */
static int tolua_set_stKarBaseParam_unsigned_bDreamDrifting(lua_State* tolua_S)
{
  stKarBaseParam* self = (stKarBaseParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'bDreamDrifting'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->bDreamDrifting = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fFricFactor of class  stFriction */
static int tolua_get_stFriction_m_fFricFactor(lua_State* tolua_S)
{
  stFriction* self = (stFriction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fFricFactor'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fFricFactor);
 return 1;
}

/* set function: m_fFricFactor of class  stFriction */
static int tolua_set_stFriction_m_fFricFactor(lua_State* tolua_S)
{
  stFriction* self = (stFriction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fFricFactor'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fFricFactor = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fRollFriction of class  stFriction */
static int tolua_get_stFriction_m_fRollFriction(lua_State* tolua_S)
{
  stFriction* self = (stFriction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fRollFriction'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fRollFriction);
 return 1;
}

/* set function: m_fRollFriction of class  stFriction */
static int tolua_set_stFriction_m_fRollFriction(lua_State* tolua_S)
{
  stFriction* self = (stFriction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fRollFriction'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fRollFriction = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fSlideFriction of class  stFriction */
static int tolua_get_stFriction_m_fSlideFriction(lua_State* tolua_S)
{
  stFriction* self = (stFriction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fSlideFriction'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fSlideFriction);
 return 1;
}

/* set function: m_fSlideFriction of class  stFriction */
static int tolua_set_stFriction_m_fSlideFriction(lua_State* tolua_S)
{
  stFriction* self = (stFriction*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fSlideFriction'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fSlideFriction = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fBackSpeedCoef of class  stKartCollisionParam */
static int tolua_get_stKartCollisionParam_m_fBackSpeedCoef(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fBackSpeedCoef'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fBackSpeedCoef);
 return 1;
}

/* set function: m_fBackSpeedCoef of class  stKartCollisionParam */
static int tolua_set_stKartCollisionParam_m_fBackSpeedCoef(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fBackSpeedCoef'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fBackSpeedCoef = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAlpha of class  stKartCollisionParam */
static int tolua_get_stKartCollisionParam_m_fAlpha(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAlpha'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAlpha);
 return 1;
}

/* set function: m_fAlpha of class  stKartCollisionParam */
static int tolua_set_stKartCollisionParam_m_fAlpha(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAlpha'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAlpha = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAlpha1 of class  stKartCollisionParam */
static int tolua_get_stKartCollisionParam_m_fAlpha1(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAlpha1'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAlpha1);
 return 1;
}

/* set function: m_fAlpha1 of class  stKartCollisionParam */
static int tolua_set_stKartCollisionParam_m_fAlpha1(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAlpha1'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAlpha1 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fTurnSpeed of class  stKartCollisionParam */
static int tolua_get_stKartCollisionParam_m_fTurnSpeed(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fTurnSpeed'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fTurnSpeed);
 return 1;
}

/* set function: m_fTurnSpeed of class  stKartCollisionParam */
static int tolua_set_stKartCollisionParam_m_fTurnSpeed(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fTurnSpeed'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fTurnSpeed = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fLinearSpeedLostRate of class  stKartCollisionParam */
static int tolua_get_stKartCollisionParam_m_fLinearSpeedLostRate(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fLinearSpeedLostRate'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fLinearSpeedLostRate);
 return 1;
}

/* set function: m_fLinearSpeedLostRate of class  stKartCollisionParam */
static int tolua_set_stKartCollisionParam_m_fLinearSpeedLostRate(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fLinearSpeedLostRate'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fLinearSpeedLostRate = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAlphaDrift1 of class  stKartCollisionParam */
static int tolua_get_stKartCollisionParam_m_fAlphaDrift1(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAlphaDrift1'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAlphaDrift1);
 return 1;
}

/* set function: m_fAlphaDrift1 of class  stKartCollisionParam */
static int tolua_set_stKartCollisionParam_m_fAlphaDrift1(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAlphaDrift1'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAlphaDrift1 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fAlpha2 of class  stKartCollisionParam */
static int tolua_get_stKartCollisionParam_m_fAlpha2(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAlpha2'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fAlpha2);
 return 1;
}

/* set function: m_fAlpha2 of class  stKartCollisionParam */
static int tolua_set_stKartCollisionParam_m_fAlpha2(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fAlpha2'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fAlpha2 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fTurnSpeed1 of class  stKartCollisionParam */
static int tolua_get_stKartCollisionParam_m_fTurnSpeed1(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fTurnSpeed1'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fTurnSpeed1);
 return 1;
}

/* set function: m_fTurnSpeed1 of class  stKartCollisionParam */
static int tolua_set_stKartCollisionParam_m_fTurnSpeed1(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fTurnSpeed1'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fTurnSpeed1 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fLinearSpeedLostRate1 of class  stKartCollisionParam */
static int tolua_get_stKartCollisionParam_m_fLinearSpeedLostRate1(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fLinearSpeedLostRate1'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fLinearSpeedLostRate1);
 return 1;
}

/* set function: m_fLinearSpeedLostRate1 of class  stKartCollisionParam */
static int tolua_set_stKartCollisionParam_m_fLinearSpeedLostRate1(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fLinearSpeedLostRate1'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fLinearSpeedLostRate1 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: m_fForwardSpeedCoef of class  stKartCollisionParam */
static int tolua_get_stKartCollisionParam_m_fForwardSpeedCoef(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fForwardSpeedCoef'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->m_fForwardSpeedCoef);
 return 1;
}

/* set function: m_fForwardSpeedCoef of class  stKartCollisionParam */
static int tolua_set_stKartCollisionParam_m_fForwardSpeedCoef(lua_State* tolua_S)
{
  stKartCollisionParam* self = (stKartCollisionParam*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'm_fForwardSpeedCoef'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->m_fForwardSpeedCoef = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: _begin_dis of class  stSendPkg */
static int tolua_get_stSendPkg_unsigned__begin_dis(lua_State* tolua_S)
{
  stSendPkg* self = (stSendPkg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '_begin_dis'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->_begin_dis);
 return 1;
}

/* set function: _begin_dis of class  stSendPkg */
static int tolua_set_stSendPkg_unsigned__begin_dis(lua_State* tolua_S)
{
  stSendPkg* self = (stSendPkg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '_begin_dis'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->_begin_dis = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: _end_dis of class  stSendPkg */
static int tolua_get_stSendPkg_unsigned__end_dis(lua_State* tolua_S)
{
  stSendPkg* self = (stSendPkg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '_end_dis'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->_end_dis);
 return 1;
}

/* set function: _end_dis of class  stSendPkg */
static int tolua_set_stSendPkg_unsigned__end_dis(lua_State* tolua_S)
{
  stSendPkg* self = (stSendPkg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '_end_dis'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->_end_dis = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: _frequence of class  stSendPkg */
static int tolua_get_stSendPkg_unsigned__frequence(lua_State* tolua_S)
{
  stSendPkg* self = (stSendPkg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '_frequence'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->_frequence);
 return 1;
}

/* set function: _frequence of class  stSendPkg */
static int tolua_set_stSendPkg_unsigned__frequence(lua_State* tolua_S)
{
  stSendPkg* self = (stSendPkg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable '_frequence'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->_frequence = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_vPosition of class  stSuspension */
static int tolua_get_stSuspension_sim_vPosition(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_vPosition'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->sim_vPosition,"FLOAT3");
 return 1;
}

/* set function: sim_vPosition of class  stSuspension */
static int tolua_set_stSuspension_sim_vPosition(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_vPosition'",NULL);
 if (!tolua_isusertype(tolua_S,2,"FLOAT3",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_vPosition = *((FLOAT3*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: sim_vSpecChassis of class  stSuspension */
static int tolua_get_stSuspension_sim_vSpecChassis(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_vSpecChassis'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->sim_vSpecChassis,"FLOAT3");
 return 1;
}

/* set function: sim_vSpecChassis of class  stSuspension */
static int tolua_set_stSuspension_sim_vSpecChassis(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_vSpecChassis'",NULL);
 if (!tolua_isusertype(tolua_S,2,"FLOAT3",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_vSpecChassis = *((FLOAT3*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: sim_vSpecBase of class  stSuspension */
static int tolua_get_stSuspension_sim_vSpecBase(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_vSpecBase'",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&self->sim_vSpecBase,"FLOAT3");
 return 1;
}

/* set function: sim_vSpecBase of class  stSuspension */
static int tolua_set_stSuspension_sim_vSpecBase(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_vSpecBase'",NULL);
 if (!tolua_isusertype(tolua_S,2,"FLOAT3",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_vSpecBase = *((FLOAT3*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: sim_fSpecWheelRadius of class  stSuspension */
static int tolua_get_stSuspension_sim_fSpecWheelRadius(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSpecWheelRadius'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fSpecWheelRadius);
 return 1;
}

/* set function: sim_fSpecWheelRadius of class  stSuspension */
static int tolua_set_stSuspension_sim_fSpecWheelRadius(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSpecWheelRadius'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fSpecWheelRadius = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fMassChassis of class  stSuspension */
static int tolua_get_stSuspension_sim_fMassChassis(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fMassChassis'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fMassChassis);
 return 1;
}

/* set function: sim_fMassChassis of class  stSuspension */
static int tolua_set_stSuspension_sim_fMassChassis(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fMassChassis'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fMassChassis = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fMassWheel of class  stSuspension */
static int tolua_get_stSuspension_sim_fMassWheel(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fMassWheel'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fMassWheel);
 return 1;
}

/* set function: sim_fMassWheel of class  stSuspension */
static int tolua_set_stSuspension_sim_fMassWheel(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fMassWheel'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fMassWheel = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fZBases of class  stSuspension */
static int tolua_get_stSuspension_sim_fZBases(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fZBases'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fZBases);
 return 1;
}

/* set function: sim_fZBases of class  stSuspension */
static int tolua_set_stSuspension_sim_fZBases(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fZBases'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fZBases = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fZChassis of class  stSuspension */
static int tolua_get_stSuspension_sim_fZChassis(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fZChassis'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fZChassis);
 return 1;
}

/* set function: sim_fZChassis of class  stSuspension */
static int tolua_set_stSuspension_sim_fZChassis(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fZChassis'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fZChassis = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fZPillar of class  stSuspension */
static int tolua_get_stSuspension_sim_fZPillar(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fZPillar'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fZPillar);
 return 1;
}

/* set function: sim_fZPillar of class  stSuspension */
static int tolua_set_stSuspension_sim_fZPillar(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fZPillar'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fZPillar = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fZWheels of class  stSuspension */
static int tolua_get_stSuspension_sim_fZWheels(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fZWheels'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fZWheels);
 return 1;
}

/* set function: sim_fZWheels of class  stSuspension */
static int tolua_set_stSuspension_sim_fZWheels(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fZWheels'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fZWheels = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fAnchorZSuspensions of class  stSuspension */
static int tolua_get_stSuspension_sim_fAnchorZSuspensions(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fAnchorZSuspensions'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fAnchorZSuspensions);
 return 1;
}

/* set function: sim_fAnchorZSuspensions of class  stSuspension */
static int tolua_set_stSuspension_sim_fAnchorZSuspensions(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fAnchorZSuspensions'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fAnchorZSuspensions = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fAnchorZPillar of class  stSuspension */
static int tolua_get_stSuspension_sim_fAnchorZPillar(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fAnchorZPillar'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fAnchorZPillar);
 return 1;
}

/* set function: sim_fAnchorZPillar of class  stSuspension */
static int tolua_set_stSuspension_sim_fAnchorZPillar(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fAnchorZPillar'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fAnchorZPillar = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fAnchorChassis of class  stSuspension */
static int tolua_get_stSuspension_sim_fAnchorChassis(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fAnchorChassis'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fAnchorChassis);
 return 1;
}

/* set function: sim_fAnchorChassis of class  stSuspension */
static int tolua_set_stSuspension_sim_fAnchorChassis(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fAnchorChassis'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fAnchorChassis = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fSusSpring of class  stSuspension */
static int tolua_get_stSuspension_sim_fSusSpring(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSusSpring'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fSusSpring);
 return 1;
}

/* set function: sim_fSusSpring of class  stSuspension */
static int tolua_set_stSuspension_sim_fSusSpring(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSusSpring'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fSusSpring = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fSusDamping of class  stSuspension */
static int tolua_get_stSuspension_sim_fSusDamping(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSusDamping'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fSusDamping);
 return 1;
}

/* set function: sim_fSusDamping of class  stSuspension */
static int tolua_set_stSuspension_sim_fSusDamping(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSusDamping'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fSusDamping = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fSusValue of class  stSuspension */
static int tolua_get_stSuspension_sim_fSusValue(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSusValue'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fSusValue);
 return 1;
}

/* set function: sim_fSusValue of class  stSuspension */
static int tolua_set_stSuspension_sim_fSusValue(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSusValue'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fSusValue = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fSusRestitution of class  stSuspension */
static int tolua_get_stSuspension_sim_fSusRestitution(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSusRestitution'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fSusRestitution);
 return 1;
}

/* set function: sim_fSusRestitution of class  stSuspension */
static int tolua_set_stSuspension_sim_fSusRestitution(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSusRestitution'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fSusRestitution = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: sim_fSusLimitPlane of class  stSuspension */
static int tolua_get_stSuspension_sim_fSusLimitPlane(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSusLimitPlane'",NULL);
#endif
 tolua_pushnumber(tolua_S,(lua_Number)self->sim_fSusLimitPlane);
 return 1;
}

/* set function: sim_fSusLimitPlane of class  stSuspension */
static int tolua_set_stSuspension_sim_fSusLimitPlane(lua_State* tolua_S)
{
  stSuspension* self = (stSuspension*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sim_fSusLimitPlane'",NULL);
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sim_fSusLimitPlane = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescSuspension::suspension */
static int tolua_get_DescSuspension_suspension(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&DescSuspension::suspension,"stSuspension");
 return 1;
}

/* set function: DescSuspension::suspension */
static int tolua_set_DescSuspension_suspension(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"stSuspension",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescSuspension::suspension = *((stSuspension*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: DescSendPkg::SendPkg */
static int tolua_get_ScripFunctions_DescSendPkg_SendPkg(lua_State* tolua_S)
{
 int tolua_index;
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=5)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&DescSendPkg::SendPkg[tolua_index],"stSendPkg");
 return 1;
}

/* set function: DescSendPkg::SendPkg */
static int tolua_set_ScripFunctions_DescSendPkg_SendPkg(lua_State* tolua_S)
{
 int tolua_index;
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=5)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  DescSendPkg::SendPkg[tolua_index] = *((stSendPkg*)  tolua_tousertype(tolua_S,3,0));
 return 0;
}

/* get function: DescConstant::KarType */
static int tolua_get_DescConstant_KarType(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescConstant::KarType);
 return 1;
}

/* set function: DescConstant::KarType */
static int tolua_set_DescConstant_KarType(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescConstant::KarType = ((int)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescConstant::KarBaseParam */
static int tolua_get_ScripFunctions_DescConstant_KarBaseParam(lua_State* tolua_S)
{
 int tolua_index;
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=64)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushusertype(tolua_S,(void*)&DescConstant::KarBaseParam[tolua_index],"stKarBaseParam");
 return 1;
}

/* set function: DescConstant::KarBaseParam */
static int tolua_set_ScripFunctions_DescConstant_KarBaseParam(lua_State* tolua_S)
{
 int tolua_index;
#ifndef TOLUA_RELEASE
 {
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=64)
 tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  DescConstant::KarBaseParam[tolua_index] = *((stKarBaseParam*)  tolua_tousertype(tolua_S,3,0));
 return 0;
}

/* get function: DescConstant::KartCollisionParam */
static int tolua_get_DescConstant_KartCollisionParam(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&DescConstant::KartCollisionParam,"stKartCollisionParam");
 return 1;
}

/* set function: DescConstant::KartCollisionParam */
static int tolua_set_DescConstant_KartCollisionParam(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"stKartCollisionParam",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescConstant::KartCollisionParam = *((stKartCollisionParam*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: DescConstant::KartFriction */
static int tolua_get_DescConstant_KartFriction(lua_State* tolua_S)
{
 tolua_pushusertype(tolua_S,(void*)&DescConstant::KartFriction,"stFriction");
 return 1;
}

/* set function: DescConstant::KartFriction */
static int tolua_set_DescConstant_KartFriction(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isusertype(tolua_S,2,"stFriction",0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescConstant::KartFriction = *((stFriction*)  tolua_tousertype(tolua_S,2,0));
 return 0;
}

/* get function: DescConstant::KartRotate */
static int tolua_get_DescConstant_KartRotate(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescConstant::KartRotate);
 return 1;
}

/* set function: DescConstant::KartRotate */
static int tolua_set_DescConstant_KartRotate(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescConstant::KartRotate = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescConstant::KartRotate1 */
static int tolua_get_DescConstant_KartRotate1(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescConstant::KartRotate1);
 return 1;
}

/* set function: DescConstant::KartRotate1 */
static int tolua_set_DescConstant_KartRotate1(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescConstant::KartRotate1 = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraNormalHeight */
static int tolua_get_DescCamera_cameraNormalHeight(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraNormalHeight);
 return 1;
}

/* set function: DescCamera::cameraNormalHeight */
static int tolua_set_DescCamera_cameraNormalHeight(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraNormalHeight = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraNormalMaxDis */
static int tolua_get_DescCamera_cameraNormalMaxDis(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraNormalMaxDis);
 return 1;
}

/* set function: DescCamera::cameraNormalMaxDis */
static int tolua_set_DescCamera_cameraNormalMaxDis(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraNormalMaxDis = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraNormalMinDis */
static int tolua_get_DescCamera_cameraNormalMinDis(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraNormalMinDis);
 return 1;
}

/* set function: DescCamera::cameraNormalMinDis */
static int tolua_set_DescCamera_cameraNormalMinDis(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraNormalMinDis = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraNormalFocusDis */
static int tolua_get_DescCamera_cameraNormalFocusDis(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraNormalFocusDis);
 return 1;
}

/* set function: DescCamera::cameraNormalFocusDis */
static int tolua_set_DescCamera_cameraNormalFocusDis(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraNormalFocusDis = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraNormalCoef */
static int tolua_get_DescCamera_cameraNormalCoef(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraNormalCoef);
 return 1;
}

/* set function: DescCamera::cameraNormalCoef */
static int tolua_set_DescCamera_cameraNormalCoef(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraNormalCoef = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraNormalInterp */
static int tolua_get_DescCamera_cameraNormalInterp(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraNormalInterp);
 return 1;
}

/* set function: DescCamera::cameraNormalInterp */
static int tolua_set_DescCamera_cameraNormalInterp(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraNormalInterp = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraSpeedUpHeight */
static int tolua_get_DescCamera_cameraSpeedUpHeight(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraSpeedUpHeight);
 return 1;
}

/* set function: DescCamera::cameraSpeedUpHeight */
static int tolua_set_DescCamera_cameraSpeedUpHeight(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraSpeedUpHeight = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraSpeedUpMaxDis */
static int tolua_get_DescCamera_cameraSpeedUpMaxDis(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraSpeedUpMaxDis);
 return 1;
}

/* set function: DescCamera::cameraSpeedUpMaxDis */
static int tolua_set_DescCamera_cameraSpeedUpMaxDis(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraSpeedUpMaxDis = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraSpeedUpMinDis */
static int tolua_get_DescCamera_cameraSpeedUpMinDis(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraSpeedUpMinDis);
 return 1;
}

/* set function: DescCamera::cameraSpeedUpMinDis */
static int tolua_set_DescCamera_cameraSpeedUpMinDis(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraSpeedUpMinDis = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraSpeedUpFocusDis */
static int tolua_get_DescCamera_cameraSpeedUpFocusDis(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraSpeedUpFocusDis);
 return 1;
}

/* set function: DescCamera::cameraSpeedUpFocusDis */
static int tolua_set_DescCamera_cameraSpeedUpFocusDis(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraSpeedUpFocusDis = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraSpeedUpCoef */
static int tolua_get_DescCamera_cameraSpeedUpCoef(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraSpeedUpCoef);
 return 1;
}

/* set function: DescCamera::cameraSpeedUpCoef */
static int tolua_set_DescCamera_cameraSpeedUpCoef(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraSpeedUpCoef = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraSpeedUpInterp */
static int tolua_get_DescCamera_cameraSpeedUpInterp(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraSpeedUpInterp);
 return 1;
}

/* set function: DescCamera::cameraSpeedUpInterp */
static int tolua_set_DescCamera_cameraSpeedUpInterp(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraSpeedUpInterp = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraHeightIterp */
static int tolua_get_DescCamera_cameraHeightIterp(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraHeightIterp);
 return 1;
}

/* set function: DescCamera::cameraHeightIterp */
static int tolua_set_DescCamera_cameraHeightIterp(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraHeightIterp = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraFocusIterp */
static int tolua_get_DescCamera_cameraFocusIterp(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraFocusIterp);
 return 1;
}

/* set function: DescCamera::cameraFocusIterp */
static int tolua_set_DescCamera_cameraFocusIterp(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraFocusIterp = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::cameraMaxSpeed */
static int tolua_get_DescCamera_cameraMaxSpeed(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::cameraMaxSpeed);
 return 1;
}

/* set function: DescCamera::cameraMaxSpeed */
static int tolua_set_DescCamera_cameraMaxSpeed(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::cameraMaxSpeed = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::CAMERA_FAR */
static int tolua_get_DescCamera_CAMERA_FAR(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::CAMERA_FAR);
 return 1;
}

/* set function: DescCamera::CAMERA_FAR */
static int tolua_set_DescCamera_CAMERA_FAR(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::CAMERA_FAR = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::CAMERA_NEAR */
static int tolua_get_DescCamera_CAMERA_NEAR(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::CAMERA_NEAR);
 return 1;
}

/* set function: DescCamera::CAMERA_NEAR */
static int tolua_set_DescCamera_CAMERA_NEAR(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::CAMERA_NEAR = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::CAMERA_AspectRatio */
static int tolua_get_DescCamera_CAMERA_AspectRatio(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::CAMERA_AspectRatio);
 return 1;
}

/* set function: DescCamera::CAMERA_AspectRatio */
static int tolua_set_DescCamera_CAMERA_AspectRatio(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::CAMERA_AspectRatio = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: DescCamera::CAMERA_FOV */
static int tolua_get_DescCamera_CAMERA_FOV(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)DescCamera::CAMERA_FOV);
 return 1;
}

/* set function: DescCamera::CAMERA_FOV */
static int tolua_set_DescCamera_CAMERA_FOV(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  DescCamera::CAMERA_FOV = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GameSetting::SpeedLineShowVelo */
static int tolua_get_GameSetting_SpeedLineShowVelo(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GameSetting::SpeedLineShowVelo);
 return 1;
}

/* set function: GameSetting::SpeedLineShowVelo */
static int tolua_set_GameSetting_SpeedLineShowVelo(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GameSetting::SpeedLineShowVelo = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GameSetting::SpeedDispCoef */
static int tolua_get_GameSetting_SpeedDispCoef(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)GameSetting::SpeedDispCoef);
 return 1;
}

/* set function: GameSetting::SpeedDispCoef */
static int tolua_set_GameSetting_SpeedDispCoef(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GameSetting::SpeedDispCoef = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: GameSetting::ShowShadow */
static int tolua_get_GameSetting_ShowShadow(lua_State* tolua_S)
{
 tolua_pushboolean(tolua_S,(bool)GameSetting::ShowShadow);
 return 1;
}

/* set function: GameSetting::ShowShadow */
static int tolua_set_GameSetting_ShowShadow(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isboolean(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  GameSetting::ShowShadow = ((bool)  tolua_toboolean(tolua_S,2,0));
 return 0;
}

/* get function: ContactConfig::smallangle */
static int tolua_get_ContactConfig_smallangle(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)ContactConfig::smallangle);
 return 1;
}

/* set function: ContactConfig::smallangle */
static int tolua_set_ContactConfig_smallangle(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  ContactConfig::smallangle = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: ContactConfig::midangle */
static int tolua_get_ContactConfig_midangle(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)ContactConfig::midangle);
 return 1;
}

/* set function: ContactConfig::midangle */
static int tolua_set_ContactConfig_midangle(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  ContactConfig::midangle = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: ContactConfig::bigangle */
static int tolua_get_ContactConfig_bigangle(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)ContactConfig::bigangle);
 return 1;
}

/* set function: ContactConfig::bigangle */
static int tolua_set_ContactConfig_bigangle(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  ContactConfig::bigangle = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: ContactConfig::inversevelocityrate */
static int tolua_get_ContactConfig_inversevelocityrate(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)ContactConfig::inversevelocityrate);
 return 1;
}

/* set function: ContactConfig::inversevelocityrate */
static int tolua_set_ContactConfig_inversevelocityrate(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  ContactConfig::inversevelocityrate = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* get function: PhysSceneConfig::gravity */
static int tolua_get_PhysSceneConfig_gravity(lua_State* tolua_S)
{
 tolua_pushnumber(tolua_S,(lua_Number)PhysSceneConfig::gravity);
 return 1;
}

/* set function: PhysSceneConfig::gravity */
static int tolua_set_PhysSceneConfig_gravity(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
 tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  PhysSceneConfig::gravity = ((float)  tolua_tonumber(tolua_S,2,0));
 return 0;
}

/* Open function */
TOLUA_API int tolua_ScripFunctions_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_function(tolua_S,"lua_break",tolua_ScripFunctions_lua_break00);
 tolua_module(tolua_S,"ClConfig",1);
 tolua_beginmodule(tolua_S,"ClConfig");
 tolua_variable(tolua_S,"window_width",tolua_get_ClConfig_window_width,tolua_set_ClConfig_window_width);
 tolua_variable(tolua_S,"window_height",tolua_get_ClConfig_window_height,tolua_set_ClConfig_window_height);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"GPSettings",1);
 tolua_beginmodule(tolua_S,"GPSettings");
 tolua_variable(tolua_S,"nearDistance",tolua_get_GPSettings_nearDistance,tolua_set_GPSettings_nearDistance);
 tolua_variable(tolua_S,"nearAngel",tolua_get_GPSettings_nearAngel,tolua_set_GPSettings_nearAngel);
 tolua_variable(tolua_S,"farDistance",tolua_get_GPSettings_farDistance,tolua_set_GPSettings_farDistance);
 tolua_variable(tolua_S,"farAngel",tolua_get_GPSettings_farAngel,tolua_set_GPSettings_farAngel);
 tolua_variable(tolua_S,"exchangetime",tolua_get_GPSettings_exchangetime,tolua_set_GPSettings_exchangetime);
 tolua_variable(tolua_S,"aimernode",tolua_get_GPSettings_aimernode,tolua_set_GPSettings_aimernode);
 tolua_variable(tolua_S,"aimednode",tolua_get_GPSettings_aimednode,tolua_set_GPSettings_aimednode);
 tolua_variable(tolua_S,"aimboardx",tolua_get_GPSettings_aimboardx,tolua_set_GPSettings_aimboardx);
 tolua_variable(tolua_S,"aimedboardx",tolua_get_GPSettings_aimedboardx,tolua_set_GPSettings_aimedboardx);
 tolua_variable(tolua_S,"aimoffsetx",tolua_get_GPSettings_aimoffsetx,tolua_set_GPSettings_aimoffsetx);
 tolua_variable(tolua_S,"aimoffsety",tolua_get_GPSettings_aimoffsety,tolua_set_GPSettings_aimoffsety);
 tolua_variable(tolua_S,"aimoffsetz",tolua_get_GPSettings_aimoffsetz,tolua_set_GPSettings_aimoffsetz);
 tolua_variable(tolua_S,"aimedmex",tolua_get_GPSettings_aimedmex,tolua_set_GPSettings_aimedmex);
 tolua_variable(tolua_S,"aimedmey",tolua_get_GPSettings_aimedmey,tolua_set_GPSettings_aimedmey);
 tolua_variable(tolua_S,"aimedmez",tolua_get_GPSettings_aimedmez,tolua_set_GPSettings_aimedmez);
 tolua_variable(tolua_S,"aimedoffsetx",tolua_get_GPSettings_aimedoffsetx,tolua_set_GPSettings_aimedoffsetx);
 tolua_variable(tolua_S,"aimedoffsety",tolua_get_GPSettings_aimedoffsety,tolua_set_GPSettings_aimedoffsety);
 tolua_variable(tolua_S,"aimedoffsetz",tolua_get_GPSettings_aimedoffsetz,tolua_set_GPSettings_aimedoffsetz);
 tolua_variable(tolua_S,"aimeddistance",tolua_get_GPSettings_aimeddistance,tolua_set_GPSettings_aimeddistance);
 tolua_variable(tolua_S,"aimrotspeed",tolua_get_GPSettings_aimrotspeed,tolua_set_GPSettings_aimrotspeed);
 tolua_variable(tolua_S,"aimscalespeed",tolua_get_GPSettings_aimscalespeed,tolua_set_GPSettings_aimscalespeed);
 tolua_variable(tolua_S,"gottenshownode",tolua_get_GPSettings_gottenshownode,tolua_set_GPSettings_gottenshownode);
 tolua_variable(tolua_S,"throwanimation",tolua_get_GPSettings_throwanimation,tolua_set_GPSettings_throwanimation);
 tolua_variable(tolua_S,"watchcameraspeed",tolua_get_GPSettings_watchcameraspeed,tolua_set_GPSettings_watchcameraspeed);
 tolua_variable(tolua_S,"eatsound",tolua_get_GPSettings_eatsound,tolua_set_GPSettings_eatsound);
 tolua_variable(tolua_S,"eatensound",tolua_get_GPSettings_eatensound,tolua_set_GPSettings_eatensound);
 tolua_variable(tolua_S,"gottendelay",tolua_get_GPSettings_gottendelay,tolua_set_GPSettings_gottendelay);
 tolua_variable(tolua_S,"gpPropPointRadius",tolua_get_GPSettings_gpPropPointRadius,tolua_set_GPSettings_gpPropPointRadius);
 tolua_variable(tolua_S,"gpStaticBoxFloatToGroud",tolua_get_GPSettings_gpStaticBoxFloatToGroud,tolua_set_GPSettings_gpStaticBoxFloatToGroud);
 tolua_variable(tolua_S,"gpMagnetAcceForce",tolua_get_GPSettings_gpMagnetAcceForce,tolua_set_GPSettings_gpMagnetAcceForce);
 tolua_variable(tolua_S,"gpMagnetTargetModifyParam",tolua_get_GPSettings_gpMagnetTargetModifyParam,tolua_set_GPSettings_gpMagnetTargetModifyParam);
 tolua_variable(tolua_S,"gpMagnetSlackSlipFriction",tolua_get_GPSettings_gpMagnetSlackSlipFriction,tolua_set_GPSettings_gpMagnetSlackSlipFriction);
 tolua_variable(tolua_S,"gpMissileFloatToGround",tolua_get_GPSettings_gpMissileFloatToGround,tolua_set_GPSettings_gpMissileFloatToGround);
 tolua_variable(tolua_S,"gpMissileHelixRadius",tolua_get_GPSettings_gpMissileHelixRadius,tolua_set_GPSettings_gpMissileHelixRadius);
 tolua_variable(tolua_S,"gpMissileHelixCircleCount",tolua_get_GPSettings_gpMissileHelixCircleCount,tolua_set_GPSettings_gpMissileHelixCircleCount);
 tolua_variable(tolua_S,"gpFlyingFloatToGround",tolua_get_GPSettings_gpFlyingFloatToGround,tolua_set_GPSettings_gpFlyingFloatToGround);
 tolua_variable(tolua_S,"gpFlyingHelixMaxRadius",tolua_get_GPSettings_gpFlyingHelixMaxRadius,tolua_set_GPSettings_gpFlyingHelixMaxRadius);
 tolua_variable(tolua_S,"gpFlyingHelixMinRadius",tolua_get_GPSettings_gpFlyingHelixMinRadius,tolua_set_GPSettings_gpFlyingHelixMinRadius);
 tolua_variable(tolua_S,"gpFlyingHelixSideMaxAngle",tolua_get_GPSettings_gpFlyingHelixSideMaxAngle,tolua_set_GPSettings_gpFlyingHelixSideMaxAngle);
 tolua_variable(tolua_S,"gpFlyingHelixSideMinAngle",tolua_get_GPSettings_gpFlyingHelixSideMinAngle,tolua_set_GPSettings_gpFlyingHelixSideMinAngle);
 tolua_variable(tolua_S,"gpFlyingTurnMaxCount",tolua_get_GPSettings_gpFlyingTurnMaxCount,tolua_set_GPSettings_gpFlyingTurnMaxCount);
 tolua_variable(tolua_S,"gpFlyingTurnMinCount",tolua_get_GPSettings_gpFlyingTurnMinCount,tolua_set_GPSettings_gpFlyingTurnMinCount);
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"FLOAT2","FLOAT2","",tolua_collect_FLOAT2);
#else
 tolua_cclass(tolua_S,"FLOAT2","FLOAT2","",NULL);
#endif
 tolua_beginmodule(tolua_S,"FLOAT2");
 tolua_variable(tolua_S,"x",tolua_get_FLOAT2_x,tolua_set_FLOAT2_x);
 tolua_variable(tolua_S,"y",tolua_get_FLOAT2_y,tolua_set_FLOAT2_y);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"FLOAT3","FLOAT3","",NULL);
 tolua_beginmodule(tolua_S,"FLOAT3");
 tolua_variable(tolua_S,"x",tolua_get_FLOAT3_x,tolua_set_FLOAT3_x);
 tolua_variable(tolua_S,"y",tolua_get_FLOAT3_y,tolua_set_FLOAT3_y);
 tolua_variable(tolua_S,"z",tolua_get_FLOAT3_z,tolua_set_FLOAT3_z);
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"stKarBaseParam","stKarBaseParam","",tolua_collect_stKarBaseParam);
#else
 tolua_cclass(tolua_S,"stKarBaseParam","stKarBaseParam","",NULL);
#endif
 tolua_beginmodule(tolua_S,"stKarBaseParam");
 tolua_variable(tolua_S,"vMass",tolua_get_stKarBaseParam_vMass,tolua_set_stKarBaseParam_vMass);
 tolua_variable(tolua_S,"vDigitalSteeringDelta",tolua_get_stKarBaseParam_vDigitalSteeringDelta,tolua_set_stKarBaseParam_vDigitalSteeringDelta);
 tolua_variable(tolua_S,"vSteeringMaxAngle",tolua_get_stKarBaseParam_vSteeringMaxAngle,tolua_set_stKarBaseParam_vSteeringMaxAngle);
 tolua_variable(tolua_S,"vMoterForce",tolua_get_stKarBaseParam_vMoterForce,tolua_set_stKarBaseParam_vMoterForce);
 tolua_variable(tolua_S,"vMaxVelocity",tolua_get_stKarBaseParam_vMaxVelocity,tolua_set_stKarBaseParam_vMaxVelocity);
 tolua_variable(tolua_S,"vCameraDistance",tolua_get_stKarBaseParam_vCameraDistance,tolua_set_stKarBaseParam_vCameraDistance);
 tolua_variable(tolua_S,"vTransmissionEfficiency",tolua_get_stKarBaseParam_vTransmissionEfficiency,tolua_set_stKarBaseParam_vTransmissionEfficiency);
 tolua_variable(tolua_S,"vDifferentialRatio",tolua_get_stKarBaseParam_vDifferentialRatio,tolua_set_stKarBaseParam_vDifferentialRatio);
 tolua_variable(tolua_S,"motorMinRpmToGearDown",tolua_get_stKarBaseParam_motorMinRpmToGearDown,tolua_set_stKarBaseParam_motorMinRpmToGearDown);
 tolua_variable(tolua_S,"motorMaxRpmToGearUp",tolua_get_stKarBaseParam_motorMaxRpmToGearUp,tolua_set_stKarBaseParam_motorMaxRpmToGearUp);
 tolua_variable(tolua_S,"motorMinRpm",tolua_get_stKarBaseParam_motorMinRpm,tolua_set_stKarBaseParam_motorMinRpm);
 tolua_variable(tolua_S,"motorMaxRpm",tolua_get_stKarBaseParam_motorMaxRpm,tolua_set_stKarBaseParam_motorMaxRpm);
 tolua_variable(tolua_S,"gearNbForwardGears",tolua_get_stKarBaseParam_unsigned_gearNbForwardGears,tolua_set_stKarBaseParam_unsigned_gearNbForwardGears);
 tolua_variable(tolua_S,"gearBackwardGearRatio",tolua_get_stKarBaseParam_gearBackwardGearRatio,tolua_set_stKarBaseParam_gearBackwardGearRatio);
 tolua_array(tolua_S,"gearForwardGearRatios",tolua_get_ScripFunctions_stKarBaseParam_gearForwardGearRatios,tolua_set_ScripFunctions_stKarBaseParam_gearForwardGearRatios);
 tolua_variable(tolua_S,"wheelApproximation",tolua_get_stKarBaseParam_unsigned_wheelApproximation,tolua_set_stKarBaseParam_unsigned_wheelApproximation);
 tolua_variable(tolua_S,"wheelWidth",tolua_get_stKarBaseParam_wheelWidth,tolua_set_stKarBaseParam_wheelWidth);
 tolua_variable(tolua_S,"wheelSuspension",tolua_get_stKarBaseParam_wheelSuspension,tolua_set_stKarBaseParam_wheelSuspension);
 tolua_variable(tolua_S,"wheelSpringRestitution",tolua_get_stKarBaseParam_wheelSpringRestitution,tolua_set_stKarBaseParam_wheelSpringRestitution);
 tolua_variable(tolua_S,"wheelSpringDamping",tolua_get_stKarBaseParam_wheelSpringDamping,tolua_set_stKarBaseParam_wheelSpringDamping);
 tolua_variable(tolua_S,"wheelSpringBias",tolua_get_stKarBaseParam_wheelSpringBias,tolua_set_stKarBaseParam_wheelSpringBias);
 tolua_variable(tolua_S,"wheelmaxBrakeForce",tolua_get_stKarBaseParam_wheelmaxBrakeForce,tolua_set_stKarBaseParam_wheelmaxBrakeForce);
 tolua_variable(tolua_S,"wheelFrictionToFront",tolua_get_stKarBaseParam_wheelFrictionToFront,tolua_set_stKarBaseParam_wheelFrictionToFront);
 tolua_variable(tolua_S,"wheelFrictionToSide1",tolua_get_stKarBaseParam_wheelFrictionToSide1,tolua_set_stKarBaseParam_wheelFrictionToSide1);
 tolua_variable(tolua_S,"wheelFrictionToSide2",tolua_get_stKarBaseParam_wheelFrictionToSide2,tolua_set_stKarBaseParam_wheelFrictionToSide2);
 tolua_variable(tolua_S,"radiusCoe1",tolua_get_stKarBaseParam_radiusCoe1,tolua_set_stKarBaseParam_radiusCoe1);
 tolua_variable(tolua_S,"radiusCoe2",tolua_get_stKarBaseParam_radiusCoe2,tolua_set_stKarBaseParam_radiusCoe2);
 tolua_variable(tolua_S,"radiusCoe3",tolua_get_stKarBaseParam_radiusCoe3,tolua_set_stKarBaseParam_radiusCoe3);
 tolua_variable(tolua_S,"radiusCoe4",tolua_get_stKarBaseParam_radiusCoe4,tolua_set_stKarBaseParam_radiusCoe4);
 tolua_variable(tolua_S,"radiusCoe5",tolua_get_stKarBaseParam_radiusCoe5,tolua_set_stKarBaseParam_radiusCoe5);
 tolua_array(tolua_S,"torqueCurCoe",tolua_get_ScripFunctions_stKarBaseParam_torqueCurCoe,tolua_set_ScripFunctions_stKarBaseParam_torqueCurCoe);
 tolua_variable(tolua_S,"m_fVMax_NormalAccel",tolua_get_stKarBaseParam_m_fVMax_NormalAccel,tolua_set_stKarBaseParam_m_fVMax_NormalAccel);
 tolua_variable(tolua_S,"m_fVMax_UseItemAccel",tolua_get_stKarBaseParam_m_fVMax_UseItemAccel,tolua_set_stKarBaseParam_m_fVMax_UseItemAccel);
 tolua_variable(tolua_S,"m_fVTopAccel_NormalAccel",tolua_get_stKarBaseParam_m_fVTopAccel_NormalAccel,tolua_set_stKarBaseParam_m_fVTopAccel_NormalAccel);
 tolua_variable(tolua_S,"m_fVTopAccel_UseItemAccel",tolua_get_stKarBaseParam_m_fVTopAccel_UseItemAccel,tolua_set_stKarBaseParam_m_fVTopAccel_UseItemAccel);
 tolua_variable(tolua_S,"m_fAcel_OriForward",tolua_get_stKarBaseParam_m_fAcel_OriForward,tolua_set_stKarBaseParam_m_fAcel_OriForward);
 tolua_variable(tolua_S,"m_fAcel_OriBack",tolua_get_stKarBaseParam_m_fAcel_OriBack,tolua_set_stKarBaseParam_m_fAcel_OriBack);
 tolua_variable(tolua_S,"m_fAcelMax_NormalAccel",tolua_get_stKarBaseParam_m_fAcelMax_NormalAccel,tolua_set_stKarBaseParam_m_fAcelMax_NormalAccel);
 tolua_variable(tolua_S,"m_fAcelMax_UseItemAccel",tolua_get_stKarBaseParam_m_fAcelMax_UseItemAccel,tolua_set_stKarBaseParam_m_fAcelMax_UseItemAccel);
 tolua_variable(tolua_S,"m_fDeAcelMax",tolua_get_stKarBaseParam_m_fDeAcelMax,tolua_set_stKarBaseParam_m_fDeAcelMax);
 tolua_variable(tolua_S,"m_fAcel_Vfu_NormalAccel",tolua_get_stKarBaseParam_m_fAcel_Vfu_NormalAccel,tolua_set_stKarBaseParam_m_fAcel_Vfu_NormalAccel);
 tolua_variable(tolua_S,"m_fAcel_Vfu_UseItemAccel",tolua_get_stKarBaseParam_m_fAcel_Vfu_UseItemAccel,tolua_set_stKarBaseParam_m_fAcel_Vfu_UseItemAccel);
 tolua_variable(tolua_S,"m_fVMin",tolua_get_stKarBaseParam_m_fVMin,tolua_set_stKarBaseParam_m_fVMin);
 tolua_variable(tolua_S,"m_fAcel_NonePress",tolua_get_stKarBaseParam_m_fAcel_NonePress,tolua_set_stKarBaseParam_m_fAcel_NonePress);
 tolua_variable(tolua_S,"m_fAcel_Vfu_NonePress",tolua_get_stKarBaseParam_m_fAcel_Vfu_NonePress,tolua_set_stKarBaseParam_m_fAcel_Vfu_NonePress);
 tolua_variable(tolua_S,"m_fAcel_AcelToNormal",tolua_get_stKarBaseParam_m_fAcel_AcelToNormal,tolua_set_stKarBaseParam_m_fAcel_AcelToNormal);
 tolua_variable(tolua_S,"bDreamDrifting",tolua_get_stKarBaseParam_unsigned_bDreamDrifting,tolua_set_stKarBaseParam_unsigned_bDreamDrifting);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"stFriction","stFriction","",NULL);
 tolua_beginmodule(tolua_S,"stFriction");
 tolua_variable(tolua_S,"m_fFricFactor",tolua_get_stFriction_m_fFricFactor,tolua_set_stFriction_m_fFricFactor);
 tolua_variable(tolua_S,"m_fRollFriction",tolua_get_stFriction_m_fRollFriction,tolua_set_stFriction_m_fRollFriction);
 tolua_variable(tolua_S,"m_fSlideFriction",tolua_get_stFriction_m_fSlideFriction,tolua_set_stFriction_m_fSlideFriction);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"stKartCollisionParam","stKartCollisionParam","",NULL);
 tolua_beginmodule(tolua_S,"stKartCollisionParam");
 tolua_variable(tolua_S,"m_fBackSpeedCoef",tolua_get_stKartCollisionParam_m_fBackSpeedCoef,tolua_set_stKartCollisionParam_m_fBackSpeedCoef);
 tolua_variable(tolua_S,"m_fAlpha",tolua_get_stKartCollisionParam_m_fAlpha,tolua_set_stKartCollisionParam_m_fAlpha);
 tolua_variable(tolua_S,"m_fAlpha1",tolua_get_stKartCollisionParam_m_fAlpha1,tolua_set_stKartCollisionParam_m_fAlpha1);
 tolua_variable(tolua_S,"m_fTurnSpeed",tolua_get_stKartCollisionParam_m_fTurnSpeed,tolua_set_stKartCollisionParam_m_fTurnSpeed);
 tolua_variable(tolua_S,"m_fLinearSpeedLostRate",tolua_get_stKartCollisionParam_m_fLinearSpeedLostRate,tolua_set_stKartCollisionParam_m_fLinearSpeedLostRate);
 tolua_variable(tolua_S,"m_fAlphaDrift1",tolua_get_stKartCollisionParam_m_fAlphaDrift1,tolua_set_stKartCollisionParam_m_fAlphaDrift1);
 tolua_variable(tolua_S,"m_fAlpha2",tolua_get_stKartCollisionParam_m_fAlpha2,tolua_set_stKartCollisionParam_m_fAlpha2);
 tolua_variable(tolua_S,"m_fTurnSpeed1",tolua_get_stKartCollisionParam_m_fTurnSpeed1,tolua_set_stKartCollisionParam_m_fTurnSpeed1);
 tolua_variable(tolua_S,"m_fLinearSpeedLostRate1",tolua_get_stKartCollisionParam_m_fLinearSpeedLostRate1,tolua_set_stKartCollisionParam_m_fLinearSpeedLostRate1);
 tolua_variable(tolua_S,"m_fForwardSpeedCoef",tolua_get_stKartCollisionParam_m_fForwardSpeedCoef,tolua_set_stKartCollisionParam_m_fForwardSpeedCoef);
 tolua_endmodule(tolua_S);
#ifdef __cplusplus
 tolua_cclass(tolua_S,"stSendPkg","stSendPkg","",tolua_collect_stSendPkg);
#else
 tolua_cclass(tolua_S,"stSendPkg","stSendPkg","",NULL);
#endif
 tolua_beginmodule(tolua_S,"stSendPkg");
 tolua_variable(tolua_S,"_begin_dis",tolua_get_stSendPkg_unsigned__begin_dis,tolua_set_stSendPkg_unsigned__begin_dis);
 tolua_variable(tolua_S,"_end_dis",tolua_get_stSendPkg_unsigned__end_dis,tolua_set_stSendPkg_unsigned__end_dis);
 tolua_variable(tolua_S,"_frequence",tolua_get_stSendPkg_unsigned__frequence,tolua_set_stSendPkg_unsigned__frequence);
 tolua_endmodule(tolua_S);
 tolua_cclass(tolua_S,"stSuspension","stSuspension","",NULL);
 tolua_beginmodule(tolua_S,"stSuspension");
 tolua_variable(tolua_S,"sim_vPosition",tolua_get_stSuspension_sim_vPosition,tolua_set_stSuspension_sim_vPosition);
 tolua_variable(tolua_S,"sim_vSpecChassis",tolua_get_stSuspension_sim_vSpecChassis,tolua_set_stSuspension_sim_vSpecChassis);
 tolua_variable(tolua_S,"sim_vSpecBase",tolua_get_stSuspension_sim_vSpecBase,tolua_set_stSuspension_sim_vSpecBase);
 tolua_variable(tolua_S,"sim_fSpecWheelRadius",tolua_get_stSuspension_sim_fSpecWheelRadius,tolua_set_stSuspension_sim_fSpecWheelRadius);
 tolua_variable(tolua_S,"sim_fMassChassis",tolua_get_stSuspension_sim_fMassChassis,tolua_set_stSuspension_sim_fMassChassis);
 tolua_variable(tolua_S,"sim_fMassWheel",tolua_get_stSuspension_sim_fMassWheel,tolua_set_stSuspension_sim_fMassWheel);
 tolua_variable(tolua_S,"sim_fZBases",tolua_get_stSuspension_sim_fZBases,tolua_set_stSuspension_sim_fZBases);
 tolua_variable(tolua_S,"sim_fZChassis",tolua_get_stSuspension_sim_fZChassis,tolua_set_stSuspension_sim_fZChassis);
 tolua_variable(tolua_S,"sim_fZPillar",tolua_get_stSuspension_sim_fZPillar,tolua_set_stSuspension_sim_fZPillar);
 tolua_variable(tolua_S,"sim_fZWheels",tolua_get_stSuspension_sim_fZWheels,tolua_set_stSuspension_sim_fZWheels);
 tolua_variable(tolua_S,"sim_fAnchorZSuspensions",tolua_get_stSuspension_sim_fAnchorZSuspensions,tolua_set_stSuspension_sim_fAnchorZSuspensions);
 tolua_variable(tolua_S,"sim_fAnchorZPillar",tolua_get_stSuspension_sim_fAnchorZPillar,tolua_set_stSuspension_sim_fAnchorZPillar);
 tolua_variable(tolua_S,"sim_fAnchorChassis",tolua_get_stSuspension_sim_fAnchorChassis,tolua_set_stSuspension_sim_fAnchorChassis);
 tolua_variable(tolua_S,"sim_fSusSpring",tolua_get_stSuspension_sim_fSusSpring,tolua_set_stSuspension_sim_fSusSpring);
 tolua_variable(tolua_S,"sim_fSusDamping",tolua_get_stSuspension_sim_fSusDamping,tolua_set_stSuspension_sim_fSusDamping);
 tolua_variable(tolua_S,"sim_fSusValue",tolua_get_stSuspension_sim_fSusValue,tolua_set_stSuspension_sim_fSusValue);
 tolua_variable(tolua_S,"sim_fSusRestitution",tolua_get_stSuspension_sim_fSusRestitution,tolua_set_stSuspension_sim_fSusRestitution);
 tolua_variable(tolua_S,"sim_fSusLimitPlane",tolua_get_stSuspension_sim_fSusLimitPlane,tolua_set_stSuspension_sim_fSusLimitPlane);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"DescSuspension",1);
 tolua_beginmodule(tolua_S,"DescSuspension");
 tolua_variable(tolua_S,"suspension",tolua_get_DescSuspension_suspension,tolua_set_DescSuspension_suspension);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"DescSendPkg",1);
 tolua_beginmodule(tolua_S,"DescSendPkg");
 tolua_array(tolua_S,"SendPkg",tolua_get_ScripFunctions_DescSendPkg_SendPkg,tolua_set_ScripFunctions_DescSendPkg_SendPkg);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"DescConstant",1);
 tolua_beginmodule(tolua_S,"DescConstant");
 tolua_variable(tolua_S,"KarType",tolua_get_DescConstant_KarType,tolua_set_DescConstant_KarType);
 tolua_array(tolua_S,"KarBaseParam",tolua_get_ScripFunctions_DescConstant_KarBaseParam,tolua_set_ScripFunctions_DescConstant_KarBaseParam);
 tolua_variable(tolua_S,"KartCollisionParam",tolua_get_DescConstant_KartCollisionParam,tolua_set_DescConstant_KartCollisionParam);
 tolua_variable(tolua_S,"KartFriction",tolua_get_DescConstant_KartFriction,tolua_set_DescConstant_KartFriction);
 tolua_variable(tolua_S,"KartRotate",tolua_get_DescConstant_KartRotate,tolua_set_DescConstant_KartRotate);
 tolua_variable(tolua_S,"KartRotate1",tolua_get_DescConstant_KartRotate1,tolua_set_DescConstant_KartRotate1);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"DescCamera",1);
 tolua_beginmodule(tolua_S,"DescCamera");
 tolua_variable(tolua_S,"cameraNormalHeight",tolua_get_DescCamera_cameraNormalHeight,tolua_set_DescCamera_cameraNormalHeight);
 tolua_variable(tolua_S,"cameraNormalMaxDis",tolua_get_DescCamera_cameraNormalMaxDis,tolua_set_DescCamera_cameraNormalMaxDis);
 tolua_variable(tolua_S,"cameraNormalMinDis",tolua_get_DescCamera_cameraNormalMinDis,tolua_set_DescCamera_cameraNormalMinDis);
 tolua_variable(tolua_S,"cameraNormalFocusDis",tolua_get_DescCamera_cameraNormalFocusDis,tolua_set_DescCamera_cameraNormalFocusDis);
 tolua_variable(tolua_S,"cameraNormalCoef",tolua_get_DescCamera_cameraNormalCoef,tolua_set_DescCamera_cameraNormalCoef);
 tolua_variable(tolua_S,"cameraNormalInterp",tolua_get_DescCamera_cameraNormalInterp,tolua_set_DescCamera_cameraNormalInterp);
 tolua_variable(tolua_S,"cameraSpeedUpHeight",tolua_get_DescCamera_cameraSpeedUpHeight,tolua_set_DescCamera_cameraSpeedUpHeight);
 tolua_variable(tolua_S,"cameraSpeedUpMaxDis",tolua_get_DescCamera_cameraSpeedUpMaxDis,tolua_set_DescCamera_cameraSpeedUpMaxDis);
 tolua_variable(tolua_S,"cameraSpeedUpMinDis",tolua_get_DescCamera_cameraSpeedUpMinDis,tolua_set_DescCamera_cameraSpeedUpMinDis);
 tolua_variable(tolua_S,"cameraSpeedUpFocusDis",tolua_get_DescCamera_cameraSpeedUpFocusDis,tolua_set_DescCamera_cameraSpeedUpFocusDis);
 tolua_variable(tolua_S,"cameraSpeedUpCoef",tolua_get_DescCamera_cameraSpeedUpCoef,tolua_set_DescCamera_cameraSpeedUpCoef);
 tolua_variable(tolua_S,"cameraSpeedUpInterp",tolua_get_DescCamera_cameraSpeedUpInterp,tolua_set_DescCamera_cameraSpeedUpInterp);
 tolua_variable(tolua_S,"cameraHeightIterp",tolua_get_DescCamera_cameraHeightIterp,tolua_set_DescCamera_cameraHeightIterp);
 tolua_variable(tolua_S,"cameraFocusIterp",tolua_get_DescCamera_cameraFocusIterp,tolua_set_DescCamera_cameraFocusIterp);
 tolua_variable(tolua_S,"cameraMaxSpeed",tolua_get_DescCamera_cameraMaxSpeed,tolua_set_DescCamera_cameraMaxSpeed);
 tolua_variable(tolua_S,"CAMERA_FAR",tolua_get_DescCamera_CAMERA_FAR,tolua_set_DescCamera_CAMERA_FAR);
 tolua_variable(tolua_S,"CAMERA_NEAR",tolua_get_DescCamera_CAMERA_NEAR,tolua_set_DescCamera_CAMERA_NEAR);
 tolua_variable(tolua_S,"CAMERA_AspectRatio",tolua_get_DescCamera_CAMERA_AspectRatio,tolua_set_DescCamera_CAMERA_AspectRatio);
 tolua_variable(tolua_S,"CAMERA_FOV",tolua_get_DescCamera_CAMERA_FOV,tolua_set_DescCamera_CAMERA_FOV);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"GameSetting",1);
 tolua_beginmodule(tolua_S,"GameSetting");
 tolua_variable(tolua_S,"SpeedLineShowVelo",tolua_get_GameSetting_SpeedLineShowVelo,tolua_set_GameSetting_SpeedLineShowVelo);
 tolua_variable(tolua_S,"SpeedDispCoef",tolua_get_GameSetting_SpeedDispCoef,tolua_set_GameSetting_SpeedDispCoef);
 tolua_variable(tolua_S,"ShowShadow",tolua_get_GameSetting_ShowShadow,tolua_set_GameSetting_ShowShadow);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"ContactConfig",1);
 tolua_beginmodule(tolua_S,"ContactConfig");
 tolua_variable(tolua_S,"smallangle",tolua_get_ContactConfig_smallangle,tolua_set_ContactConfig_smallangle);
 tolua_variable(tolua_S,"midangle",tolua_get_ContactConfig_midangle,tolua_set_ContactConfig_midangle);
 tolua_variable(tolua_S,"bigangle",tolua_get_ContactConfig_bigangle,tolua_set_ContactConfig_bigangle);
 tolua_variable(tolua_S,"inversevelocityrate",tolua_get_ContactConfig_inversevelocityrate,tolua_set_ContactConfig_inversevelocityrate);
 tolua_endmodule(tolua_S);
 tolua_module(tolua_S,"PhysSceneConfig",1);
 tolua_beginmodule(tolua_S,"PhysSceneConfig");
 tolua_variable(tolua_S,"gravity",tolua_get_PhysSceneConfig_gravity,tolua_set_PhysSceneConfig_gravity);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}
