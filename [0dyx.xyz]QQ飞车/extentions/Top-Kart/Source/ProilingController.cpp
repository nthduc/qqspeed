#include "Top-KartPCH.h"
#include "ProilingController.h"
#include "NiRenderer.h"
#include "NiVertexColorProperty.h"
#include "NiApplication.h"

#define MAX_STATUS_STR_LEN 512

ProfilingController::ProfilingController(void)
	: m_pProfileIterator(NULL)	
	, m_pScreenTextManager(NULL)
	, m_spBackground(NULL)
{
	m_bHelp=false;
	m_bVisible=false;
	m_pProfileManager = NiNew CProfileManager;
	m_pScreenTextManager = NiNew ScreenTextManager;
	m_pScreenTextManager->SetMaxCharacter(MAX_STATUS_STR_LEN);
	
	m_spBackground = NiNew NiScreenElements(NiNew NiScreenElementsData(false, true, 1));
	// We know that the polygon handle is zero and will use it directly in
	// the vertex, texture coordinate, and color assignments.
	m_spBackground->Insert(4);

	// The default values are not relevant because they will be reassigned
	// when the background is reset.
	m_spBackground->SetRectangle(0, 0.f, 0.f, 0.9f, 0.30f);
	m_spBackground->UpdateBound();

	// Select texture coordinates for the full texture image.
	m_spBackground->SetTextures(0, 0, 0.0f, 0.0f, 1.0f, 1.0f);

	// Use the background color for vertex colors.
	m_spBackground->SetColors(0, NiColorA::BLACK);
	
	NiAlphaProperty* pAlpha = NiNew NiAlphaProperty;
	NiVertexColorProperty* pVertex = NiNew NiVertexColorProperty;
	pVertex->SetSourceMode(NiVertexColorProperty::SOURCE_EMISSIVE);
	pVertex->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
	m_spBackground->AttachProperty(pVertex);

	pAlpha->SetAlphaBlending(true);
	pAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	pAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);
	m_spBackground->AttachProperty(pAlpha);

	m_spBackground->UpdateProperties();
	m_spBackground->Update(0.0f);

}

ProfilingController::~ProfilingController(void)
{
	NiDelete m_pProfileIterator;
	m_pProfileIterator=NULL;
	NiDelete m_pScreenTextManager;
	m_pScreenTextManager = NULL;
	NiDelete m_pProfileManager;
	m_spBackground = 0;
}
static NiColorA g_kProfillingColor(1.0f, 1.0f, 1.0f, 1.0f);

void ProfilingController::ProfileDraw(void)
{   
	static char acOut[MAX_STATUS_STR_LEN];
	m_pScreenTextManager->SetVisible(false);
	
	if (m_bHelp)
	{
		NiSprintf(acOut,MAX_STATUS_STR_LEN,"Show Enable the profile display and show root.");
		m_pScreenTextManager->SetText(acOut,0,g_kProfillingColor);
		NiSprintf(acOut,MAX_STATUS_STR_LEN,"Quit Disable the profile display.");
		m_pScreenTextManager->SetText(acOut,1,g_kProfillingColor);
		NiSprintf(acOut,MAX_STATUS_STR_LEN,"%s",g_kProfillingColor);
        m_pScreenTextManager->SetText(acOut,2,g_kProfillingColor);
		NiSprintf(acOut,MAX_STATUS_STR_LEN,"%s","0 Show Upper layer of current profile.");
	    m_pScreenTextManager->SetText(acOut,3,g_kProfillingColor);
		NiSprintf(acOut,MAX_STATUS_STR_LEN,"%s","1..n Show one child of current profile according to the parameter.");
        m_pScreenTextManager->SetText(acOut,4,g_kProfillingColor);
	}
	else
	{
		if (m_bVisible)
		{
			NiSprintf(acOut,MAX_STATUS_STR_LEN,"%s%15s%15s%15s%15s%15s",
				"CurrentName", "Ave_Time","Max_Time","Min_Time","One_Time","Total_Calls");

			m_pScreenTextManager->SetText(acOut,0,g_kProfillingColor);
			int nNodeNumber=0;
			CProfileIterator pTempProfileIterator = *m_pProfileIterator;
			pTempProfileIterator.First();
			if (pTempProfileIterator.Is_Done())
			{
				if (strcmp(m_pProfileIterator->Get_Current_Parent_Name(),"Root")!=0)
				{  
					NiSprintf(acOut,MAX_STATUS_STR_LEN,"%12s %3.2f %12.2f %12.2f %12.2f %12d",
						pTempProfileIterator.Get_Current_Parent_Name(), pTempProfileIterator.Get_Current_Parent_Ave_Time()*1000,
						pTempProfileIterator.Get_Current_Parent_Max_Time()*1000, pTempProfileIterator.Get_Current_Parent_Min_Time()*1000, 
						pTempProfileIterator.Get_Current_Parent_One_Time()*1000, pTempProfileIterator.Get_Current_Total_Calls());

					m_pScreenTextManager->SetText(acOut,1,g_kProfillingColor);
					nNodeNumber++;
				}

	            int nIndex=1;

				while (pTempProfileIterator.Is_Done())
				{   
					nNodeNumber++;
					NiSprintf(acOut,MAX_STATUS_STR_LEN,"%s(%d) %13.2f %13.2f %13.2f %13.2f %13d",
						pTempProfileIterator.Get_Current_Name(),nIndex,
						pTempProfileIterator.Get_Current_Total_Ave_Time()*1000,pTempProfileIterator.Get_Current_Total_Max_Time()*1000,
						pTempProfileIterator.Get_Current_Total_Min_Time()*1000,pTempProfileIterator.Get_Current_Total_One_Time()*1000,
						pTempProfileIterator.Get_Current_Total_Calls());
                     
					nIndex++;
					pTempProfileIterator.Next();
					m_pScreenTextManager->SetText(acOut,nNodeNumber,g_kProfillingColor);
				}
			}
		}
	}

	if (m_bVisible || m_bHelp)
	{
		int winWidth = NiApplication::ms_pkApplication->GetAppWindow()->GetWidth();
		int winHeight = NiApplication::ms_pkApplication->GetAppWindow()->GetHeight();

		m_spBackground->SetRectangle(0, 0.f, 0.f, 
			((float)m_pScreenTextManager->GetManagerWidth() + 32)/winWidth,
			((float)m_pScreenTextManager->GetManagerHeight() + 10)/winHeight);
		m_spBackground->Update(0.0f);
		m_spBackground->Draw(NiRenderer::GetRenderer());
		m_pScreenTextManager->Draw();
	}	
}



void  ProfilingController::Profiling(const char *csCommand)
{   
	char * sCommand = strlwr(_strdup(csCommand));

	if ((strcmp(sCommand,"help")==0)||(strcmp(sCommand,"?")==0))
	{
		m_bHelp=true;
	}
	else
	if (strcmp(sCommand,"reset")==0)
	{    
		if (CProfileIterator::bProfileIterator==true)
		{
			CProfileManager::Reset();
		}
	}
	else
		if (strcmp(sCommand,"quit")==0)
		{
			m_bHelp = false;
			m_bVisible = false;
		}
		else
		if (strcmp(sCommand,"show")==0)
		{   
			m_bHelp=false;
			m_bVisible=true;
			if (CProfileIterator::bProfileIterator!=true)
			{     
				m_pProfileIterator=CProfileManager::Get_Iterator();//Get Root_Iterator
				CProfileIterator::bProfileIterator=true;
				m_pProfileIterator->First();
				m_pProfileIterator->Enter_Parent();
			}
			else
			{
				m_pProfileIterator->First();
				m_pProfileIterator->Enter_Parent();
			}
		}
		else
		{  
			m_bHelp=false;
			int iCommandMark=atoi(sCommand);

			if (iCommandMark==0)
			{
				if (CProfileIterator::bProfileIterator==true)
					m_pProfileIterator->Enter_Parent();
			}
			else
			{
				if (CProfileIterator::bProfileIterator==true)
					m_pProfileIterator->Enter_Child(iCommandMark-1);
			}
		}
}
