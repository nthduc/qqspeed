////////////////////////////////////////////////
//     FileName:    datadef.h
//     Author:      Fish
//     Date:        2006-10-23 11:51:38
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "vector"
#include "network/msgheader.h"

struct RoomDesc
{
	void Reset()
	{
		memset(this, 0, sizeof(RoomDesc));
		Ready = State_UnReadied;
		for ( int i = 0; i < MAXSEATNUMINROOM; i++ )
		{
			SeatStatus[i] = Seat_Free;
			ReadyStatus[i] = State_UnReadied;
		}
	}
	BOOL	ValidFlag;
	SHORT	RoomID;
	SHORT	GameModeBase;   
	SHORT	GameModeSub;   
	CHAR 	RoomName[MAXROOMNAMELEN]; 
	UCHAR	SeatNum;
	UCHAR	Flag; 	
	UCHAR 	SeatID;  
	UCHAR	TeamID;
	SHORT	MapID;   
	UCHAR	Ready;
	SHORT	RoomOwnerID; 
	UCHAR 	SeatStatus[MAXSEATNUMINROOM];         
	UCHAR	ReadyStatus[MAXSEATNUMINROOM];
	UCHAR 	PlayerCount;         
	STPlayerRoomVisibleInfo 	m_stPlayerInfo[MAXSEATNUMINROOM];		
};
