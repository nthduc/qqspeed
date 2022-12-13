#ifndef __PARAMHELPER_H__
#define __PARAMHELPER_H__

#include "../uilib/variableparam_i.h"
#include "../layer/bgdefine.h"
#include "../uiproperty/propertyid.h"
#include "../tstring.h"

namespace Tenio
{
#define MAX_VALUE_LENGTH 1024
	class CParamHelper
	{
	public:
		CParamHelper()
		{
		}

		void String2Margins(LPCTSTR lpszValue, int szMargins[4])
		{
			if(lpszValue != NULL)
			{
				_stscanf(lpszValue, _T("%d,%d,%d,%d"), 
					&szMargins[0],
					&szMargins[1],
					&szMargins[2],
					&szMargins[3]);
			}
		}

		void Margins2String(int l, int t, int r, int b, LPTSTR lpszValue, int nMaxLen)
		{
			if(lpszValue != NULL)
			{
				TCHAR szValue[128] = {0};
				_stprintf(szValue, _T("%d,%d,%d,%d"), l, t, r, b);
				if(_tcslen(szValue) < nMaxLen)
				{
					_tcscpy(lpszValue, szValue);
				}
			}
		}

		void SetRectParam(RECT& rc, IVariableParam* pParam)
		{
			pParam->SetValue(PROPERTYID_XOFFSET, rc.left);
			pParam->SetValue(PROPERTYID_YOFFSET, rc.top);
			pParam->SetValue(PROPERTYID_WIDTH, rc.right - rc.left);
			pParam->SetValue(PROPERTYID_HEIGHT, rc.bottom - rc.top);		
		}

		void SetFontSetParam(LOGFONT& lf, IVariableParam* pParam)
		{
			pParam->SetValue(PROPERTYID_LOGFONT_HEIGHT,		(int)lf.lfHeight);
			pParam->SetValue(PROPERTYID_LOGFONT_WIDTH,		(int)lf.lfWidth);
			pParam->SetValue(PROPERTYID_LOGFONT_ESCAPEMENT, (int)lf.lfEscapement);
			pParam->SetValue(PROPERTYID_LOGFONT_ORIENTATION, (int)lf.lfOrientation);
			pParam->SetValue(PROPERTYID_LOGFONT_WEIGHT,		(int)lf.lfWeight);
			pParam->SetValue(PROPERTYID_LOGFONT_ITALIC,		(BYTE)lf.lfItalic);
			pParam->SetValue(PROPERTYID_LOGFONT_UNDERLINE,	(BYTE)lf.lfUnderline);
			pParam->SetValue(PROPERTYID_LOGFONT_STRIKEOUT,	(BYTE)lf.lfStrikeOut);
			pParam->SetValue(PROPERTYID_LOGFONT_CHARSET,	(BYTE)lf.lfCharSet);
			pParam->SetValue(PROPERTYID_LOGFONT_OUTPRECISION, (BYTE)lf.lfOutPrecision);
			pParam->SetValue(PROPERTYID_LOGFONT_CLIPPRECISION, (BYTE)lf.lfClipPrecision);
			pParam->SetValue(PROPERTYID_LOGFONT_QUALITY,	(BYTE)lf.lfQuality);
			pParam->SetValue(PROPERTYID_LOGFONT_PITCHANDFAMILY, (BYTE)lf.lfPitchAndFamily);
			pParam->SetValue(PROPERTYID_LOGFONT_FACENAME,	lf.lfFaceName);			
		}

		void Int2String(int nValue, LPTSTR lpszValue, UINT nMaxLen)
		{
			if(lpszValue != NULL && nMaxLen > 0)
			{
				_stprintf(lpszValue, _T("%d"), nValue);
			}
		}

		void LogFont2String(LOGFONT& lf, LPTSTR lpszFontSet, UINT nMaxLen)
		{
			if(lpszFontSet != NULL)
			{
				std::tstring str = _T("");
				std::tstring strSub = _T("");
				std::tstring strSpan = _T(";");
				TCHAR szSub[MAX_VALUE_LENGTH] = {0};
				
				Int2String(lf.lfHeight, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfWidth, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfEscapement, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfOrientation, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfWeight, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfItalic, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfUnderline, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfStrikeOut, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfCharSet, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfOutPrecision, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfClipPrecision, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfQuality, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				Int2String(lf.lfPitchAndFamily, szSub, MAX_VALUE_LENGTH);
				strSub = szSub;
				str += strSub + strSpan;
				
				strSub = lf.lfFaceName;
				str += strSub;
				
				if(str.length() < nMaxLen)
				{
					_tcscpy(lpszFontSet, str.c_str());
				}
				else
				{
					_tcsncpy(lpszFontSet, str.c_str(), nMaxLen);
					lpszFontSet[nMaxLen - 1] = '\0';
				}	
			}

		}

		void SetBgParam(int nState, COLORREF cr, IVariableParam* pParam)
		{
			pParam->SetValue(PROPERTYID_BGMODE|nState, EBGM_COLOR);
			pParam->SetValue(PROPERTYID_BGCOLOR|nState, (DWORD)cr);
		}

		void SetBgParam(int nState, LPCTSTR lpszImageFile, int nMode, IVariableParam* pParam)
		{
			pParam->SetValue(PROPERTYID_BGMODE|nState, EBGM_ONEIMAGE);
			pParam->SetValue(PROPERTYID_BGIMAGE5|nState, lpszImageFile);
			pParam->SetValue(PROPERTYID_IMAGEPAINTMODE|nState, (int)nMode);
		}

		void SetBgParam(
			int nState, 
			LPCTSTR lpszImageFile1, 
			LPCTSTR lpszImageFile2, 
			LPCTSTR lpszImageFile3,
			BOOL bHorizontal, 
			IVariableParam* pParam)
		{
			if(bHorizontal)
			{
				pParam->SetValue(PROPERTYID_BGMODE|nState, EBGM_3HIMAGES);
				pParam->SetValue(PROPERTYID_BGIMAGE4|nState, lpszImageFile1);
				pParam->SetValue(PROPERTYID_BGIMAGE6|nState, lpszImageFile3);
			}
			else
			{
				pParam->SetValue(PROPERTYID_BGMODE|nState, EBGM_3VIMAGES);
				pParam->SetValue(PROPERTYID_BGIMAGE2|nState, lpszImageFile1);
				pParam->SetValue(PROPERTYID_BGIMAGE8|nState, lpszImageFile3);
			}
			pParam->SetValue(PROPERTYID_BGIMAGE5|nState, lpszImageFile2);
		}

		void SetBgParam(
			int nState,
			LPCTSTR lpszTopLeft,
			LPCTSTR lpszTop,
			LPCTSTR lpszTopRight,
			LPCTSTR lpszLeft,
			LPCTSTR lpszMiddle,
			LPCTSTR lpszRight,
			LPCTSTR lpszBottomLeft,
			LPCTSTR lpszBottom,
			LPCTSTR lpszBottomRight, 
			IVariableParam* pParam)
		{
			pParam->SetValue(PROPERTYID_BGMODE|nState, EBGM_9IMAGES);
			pParam->SetValue(PROPERTYID_BGIMAGE1|nState, lpszTopLeft);
			pParam->SetValue(PROPERTYID_BGIMAGE2|nState, lpszTop);
			pParam->SetValue(PROPERTYID_BGIMAGE3|nState, lpszTopRight);
			pParam->SetValue(PROPERTYID_BGIMAGE4|nState, lpszLeft);
			pParam->SetValue(PROPERTYID_BGIMAGE5|nState, lpszMiddle);
			pParam->SetValue(PROPERTYID_BGIMAGE6|nState, lpszRight);
			pParam->SetValue(PROPERTYID_BGIMAGE7|nState, lpszBottomLeft);
			pParam->SetValue(PROPERTYID_BGIMAGE8|nState, lpszBottom);
			pParam->SetValue(PROPERTYID_BGIMAGE9|nState, lpszBottomRight);
		}
			
			
		void CopyFontParam(IVariableParam* pSrc, IVariableParam* pDst)
		{
			pSrc->CopyInt(pDst, PROPERTYID_LOGFONT_WIDTH);
			pSrc->CopyInt(pDst, PROPERTYID_LOGFONT_HEIGHT);
			pSrc->CopyInt(pDst, PROPERTYID_LOGFONT_WEIGHT);
			pSrc->CopyInt(pDst, PROPERTYID_LOGFONT_ESCAPEMENT);
			pSrc->CopyInt(pDst, PROPERTYID_LOGFONT_ORIENTATION);
			pSrc->CopyByte(pDst, PROPERTYID_LOGFONT_ITALIC);
			pSrc->CopyByte(pDst, PROPERTYID_LOGFONT_QUALITY);
			pSrc->CopyByte(pDst, PROPERTYID_LOGFONT_CHARSET);
			pSrc->CopyByte(pDst, PROPERTYID_LOGFONT_STRIKEOUT);
			pSrc->CopyByte(pDst, PROPERTYID_LOGFONT_UNDERLINE);
			pSrc->CopyByte(pDst, PROPERTYID_LOGFONT_OUTPRECISION);
			pSrc->CopyByte(pDst, PROPERTYID_LOGFONT_CLIPPRECISION);
			pSrc->CopyByte(pDst, PROPERTYID_LOGFONT_PITCHANDFAMILY);
			pSrc->CopyString(pDst, PROPERTYID_LOGFONT_FACENAME);				
		}
		
		void CopyCommonParam(IVariableParam* pParamSrc, IVariableParam* pParamDst)
		{		
			CopyFontParam(pParamSrc, pParamDst);
			pParamSrc->CopyInt(pParamDst, PROPERTYID_TRANSPARENT);
			pParamSrc->CopyInt(pParamDst, PROPERTYID_DISABLE);
			pParamSrc->CopyByte(pParamDst, PROPERTYID_ALPHA);
		}
	};
}


#endif