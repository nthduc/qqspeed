#include "NiMenu/MenuManager.h"
#include "GameMain.h"
/*
//---------------------------------------------------------------------------
inline MenuTask::MenuTask()
{
    SetType(Task::TYPE_FRAME);
    SetTimeParams(0.0f, 1.0f, 0.0f);
}
//---------------------------------------------------------------------------
inline void MenuTask::Execute(unsigned int, float fTime)
{
    MenuManager::Get()->UpdateMenu(fTime);
}
*/

//---------------------------------------------------------------------------
inline FrameRateTask::FrameRateTask()
{
    SetType(Task::TYPE_FRAME);
    SetTimeParams(0.0f, 1.0f, 0.0f);
}
//---------------------------------------------------------------------------
inline void FrameRateTask::Execute(unsigned int, float fTime)
{
    ((GameMain*)NiApplication::ms_pkApplication)->UpdateFrameRate();
}
//---------------------------------------------------------------------------
