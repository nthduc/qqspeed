#ifndef _MASM_LIG_H_2005_7_20
#define _MASM_LIG_H_2005_7_20

extern void MemCopy(void* pDest,int DestPitch,
					void* pSrc,int SrcPitch,
			 int width,int height);

extern void MemCopy32(void* pDest,int DestPitch,
					void* pSrc,int SrcPitch,
			 int width,int height);

#endif