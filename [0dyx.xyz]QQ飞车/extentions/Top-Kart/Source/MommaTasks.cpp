/********************************************************************
	����:	��wincoqin
	ʱ��:	2007/01/22
	Ŀ��:	  ����
*********************************************************************/


#include "Top-KartPCH.h"
#include "MommaTasks.h"



bool MommaUpdateAnimationsTask::ms_bCreated = false;


MommaUpdateAnimationsTask::~MommaUpdateAnimationsTask()
{
	ms_bCreated = false;
}