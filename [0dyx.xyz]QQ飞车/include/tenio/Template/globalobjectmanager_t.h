#ifndef __GLOBALOBJECTMANAGER_T_H__
#define __GLOBALOBJECTMANAGER_T_H__

#include "teniopointer_t.h"
#include "GlobalInterfaceManager\\globalinterfacemanager_i.h"

namespace Tenio
{
	template <class TInterface>
	class TGlobalObject
	{
	public:

		TGlobalObject(LPCTSTR lpszRegistry = NULL)
		{
			m_pComponent = NULL;

			DWORD dwComponentID = Type2ID<TInterface>::ComponentID();

			TTenioPtr<IGlobalInterfaceManager> spGlobalInterfaceManager(lpszRegistry);

			if(spGlobalInterfaceManager)
			{
				// °Ñ ComponentId ×÷Îª InterfaceId
				DWORD dwInterfaceId = dwComponentID;
				spGlobalInterfaceManager->GetGlobalInterface(0, dwInterfaceId,
					&m_pComponent);
			}
		}

		operator TInterface*() const
		{
			return static_cast<TInterface*>(m_pComponent);
		}

		TInterface& operator*() const
		{
			return *(static_cast<TInterface*>(m_pComponent));
		}

		TInterface* operator->() const
		{
			return static_cast<TInterface*>(m_pComponent);
		}

		bool operator!() const
		{
			return (m_pComponent == NULL);
		}

	protected:
		IComponent* m_pComponent;
	};
};

#endif