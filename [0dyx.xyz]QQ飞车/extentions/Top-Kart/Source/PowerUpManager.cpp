

#include "Top-KartPCH.h"

#include "PowerUpManager.h"

//---------------------------------------------------------------------------
void PowerUpManager::QueuePowerUp(PowerUp* pkPowerUp)
{
    m_kPowerUpQueue.AddTail(pkPowerUp);
}
//---------------------------------------------------------------------------
PowerUp* PowerUpManager::PopPowerUp()
{
    if (m_kPowerUpQueue.IsEmpty())
        return 0;
    else
        return m_kPowerUpQueue.RemoveHead();
}
//---------------------------------------------------------------------------

