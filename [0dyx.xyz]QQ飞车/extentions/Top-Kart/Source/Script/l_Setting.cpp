/*
** Lua binding: setting
** Generated automatically by tolua++-1.0.4 on 11/13/06 15:09:05.
*/
#include "../Top-KartPCH.h"

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int tolua_setting_open (lua_State* tolua_S);

#include "setting.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
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

/* Open function */
TOLUA_API int tolua_setting_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_module(tolua_S,"ClConfig",1);
 tolua_beginmodule(tolua_S,"ClConfig");
 tolua_variable(tolua_S,"window_width",tolua_get_ClConfig_window_width,tolua_set_ClConfig_window_width);
 tolua_variable(tolua_S,"window_height",tolua_get_ClConfig_window_height,tolua_set_ClConfig_window_height);
 tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}