#pragma once
#include "FBGUIConfig.h"		// bring in configuration options
#include "FBGUIVersion.h"		// add FBGUI version defines
#include "FBGUIDeclare.h"		// bring in FBGUI forward Declare
#include "FBGUIDef.h"			// FBGUI macro, error define, control style etc.
#include "FBGUISingleton.h"
#include <Math.h>
#include <vector>
#include <map>
#include <time.h>
#include <Windows.h>
#include "..\FreeType\ft2build.h"
#include FT_FREETYPE_H

#include "..\Lua\lua.h"
#include "..\Lua\lauxlib.h"

#pragma warning( disable : 4800 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4244 )
