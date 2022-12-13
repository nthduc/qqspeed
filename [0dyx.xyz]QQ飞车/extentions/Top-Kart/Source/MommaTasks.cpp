/********************************************************************
	作者:	　wincoqin
	时间:	2007/01/22
	目的:	  创建
*********************************************************************/


#include "Top-KartPCH.h"
#include "MommaTasks.h"



bool MommaUpdateAnimationsTask::ms_bCreated = false;


MommaUpdateAnimationsTask::~MommaUpdateAnimationsTask()
{
	ms_bCreated = false;
}