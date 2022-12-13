
//
// Based directly on the "Scheduling Game Events" Gem from 
// Game Programming Gems 3, which requires:
//
// Intel Open Source License for Game Scheduler Gem sample code
// Copyright (c) 2002, Intel Corporation.
// All rights reserved. 
//

#include "Top-KartPCH.h"

#include "Task.h"

//----------------------------------------------------------------------------
Task::Task()
{   
    m_pkNext = 0;
}
//----------------------------------------------------------------------------
Task::~Task()
{
    NiDelete m_pkNext;
}
//----------------------------------------------------------------------------
