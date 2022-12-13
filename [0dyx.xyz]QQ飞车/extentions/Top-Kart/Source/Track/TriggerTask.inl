

TriggerHintTask::TriggerHintTask(CKartStatusManager::enMark mark, float fStart, float fEnd)
{
	m_enMark = mark;
	Task::SetType(Task::TYPE_TIME);
	Task::SetTimeParams(fStart, fEnd - fStart, fEnd - fStart);

	CKartStatusManager::Get()->ShowHint(m_enMark, true);
}

TriggerHintTask::~TriggerHintTask()
{
	CKartStatusManager::Get()->ShowHint(m_enMark, false);
}