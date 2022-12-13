#pragma once

/*
#include "Encoder_h.h"

class CEncoder
{
public:
	CEncoder(void);
	~CEncoder(void);

	IEncoder* GetEncoder()
	{
		return m_pEncoder;
	}

protected:
	IEncoder* m_pEncoder;
};

IEncoder* GetDefaultEncoder();
*/


BOOL EncodeData(UINT DataID, const BYTE* Data, BYTE* Buffer, UINT* OutLen);
BOOL DecodeData(UINT DataID, const BYTE* Data, UINT DataLen, BYTE* Buffer, UINT* OutLen);
