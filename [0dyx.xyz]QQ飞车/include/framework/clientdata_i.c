////////////////////////////////////////////////
//     FileName:    GameApp_i.c
//     Author:      Fish
//     Date:        2006-10-10 16:03:04
//     Description: []
////////////////////////////////////////////////
#pragma once

#define IMPL_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
	const GUID name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

IMPL_GUID(IID_IPlayerData,0x479a4d71, 0x24ee, 0x4a76, 0xb2, 0x36, 0xd5, 0x1e, 0xdb, 0x1f, 0x56, 0x4);
