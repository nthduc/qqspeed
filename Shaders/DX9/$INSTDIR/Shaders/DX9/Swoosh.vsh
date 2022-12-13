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

vs_1_1

dcl_position v0
dcl_color v2
dcl_texcoord0 v3

;Emit to oPos.
m4x4 oPos, v0, c0

;Pass through the packed texture coordinates.
mov oT0, v3

;Pass through the vertex color.  The application code should have values in
;alpha to make the swoosh fade out.
mov oD0, v2
