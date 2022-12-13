////////////////////////////////////////////////
//     FileName:    d3dstate.h
//     Author:      Fish
//     Date:        2007-3-28 17:41:43
//     Description: []
////////////////////////////////////////////////

#ifndef __D3DSTATE_H__
#define __D3DSTATE_H__

#include "d3d9.h"
#include "map"
using namespace std;

#define MAX_TEXTURESTAGE	8
#define MAX_SAMPLER			8

class CD3DStateMgr
{
public:
	typedef map<DWORD, DWORD> STATEMAP;
	typedef map<DWORD, IDirect3DBaseTexture9*> TEXMAP;

	CD3DStateMgr(LPDIRECT3DDEVICE9 Device):
	m_pDevice(Device),
	m_dwFVF(0),
	m_bFVFModified(false)
	{
		assert(m_pDevice);
		m_pDevice->GetFVF(&m_dwFVF);
	}
	HRESULT SetFVF(DWORD FVF)
	{
		m_bFVFModified = true;
		return m_pDevice->SetFVF(FVF);
	}
	
	HRESULT SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
	{
		CComPtr<IDirect3DBaseTexture9> spTex;
		HRESULT hr = m_pDevice->GetTexture(Stage, &spTex);
		if ( SUCCEEDED(hr) )
		{
			m_mapTex[Stage] = spTex;
		}
		return m_pDevice->SetTexture(Stage, pTexture);
	}

	HRESULT SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)
	{
		/*
		DWORD dwValueOld = 0;
		HRESULT hr = m_pDevice->GetRenderState(State, &dwValueOld);
		if ( SUCCEEDED(hr) )
		{
			m_mapRenderState[State] = dwValueOld;
		}
		*/
		return m_pDevice->SetRenderState(State, Value);
	}

	HRESULT SetTextureStageState (DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
	{
		/*
		assert(Stage < MAX_TEXTURESTAGE);
		DWORD dwValueOld = 0;
		HRESULT hr = m_pDevice->GetTextureStageState(Stage, Type, &dwValueOld);
		if ( SUCCEEDED(hr) )
		{
			m_mapTextureStageState[Stage][Type] =dwValueOld;
		}
		*/
		return m_pDevice->SetTextureStageState(Stage, Type, Value);
	}

	HRESULT SetSamplerState (DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)
	{
		/*
		assert(Sampler < MAX_SAMPLER);
		DWORD dwValueOld = 0;
		HRESULT hr = m_pDevice->GetSamplerState(Sampler, Type, &dwValueOld);
		if ( SUCCEEDED(hr) )
		{
			m_mapSamplerState[Sampler][Type] = dwValueOld;
		}
		*/
		return m_pDevice->SetSamplerState(Sampler, Type, Value);
	}

	HRESULT Restore()
	{
		if ( m_bFVFModified )
		{
			m_pDevice->SetFVF(m_dwFVF);
		}
		for ( TEXMAP::iterator it = m_mapTex.begin(); it != m_mapTex.end(); ++it )
		{
			m_pDevice->SetTexture(it->first, it->second);
		}
		for ( STATEMAP::iterator it = m_mapRenderState.begin(); it != m_mapRenderState.end(); ++it )
		{
			m_pDevice->SetRenderState((D3DRENDERSTATETYPE)it->first, it->second);
		}
		for ( DWORD i = 0; i < MAX_TEXTURESTAGE; i++ )
		{
			for ( STATEMAP::iterator it = m_mapTextureStageState[i].begin(); 
				it != m_mapTextureStageState[i].end(); ++it )
			{
				m_pDevice->SetTextureStageState(i, (D3DTEXTURESTAGESTATETYPE)it->first, it->second);
			}
		}
		for ( DWORD i = 0; i < MAX_SAMPLER; i++ )
		{
			for ( STATEMAP::iterator it = m_mapSamplerState[i].begin(); 
				it != m_mapSamplerState[i].end(); ++it )
			{
				m_pDevice->SetSamplerState(i, (D3DSAMPLERSTATETYPE)it->first, it->second);
			}
		}
		return S_OK;
	}

private:
	LPDIRECT3DDEVICE9	m_pDevice;
	DWORD		m_dwFVF;
	bool		m_bFVFModified;
	TEXMAP		m_mapTex;
	STATEMAP	m_mapRenderState;
	STATEMAP	m_mapTextureStageState[MAX_TEXTURESTAGE];
	STATEMAP	m_mapSamplerState[MAX_SAMPLER];
};

#endif