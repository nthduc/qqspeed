#ifndef OV_RESLOADER_H
#define OV_RESLOADER_H

#ifdef __cplusplus
extern "C"
{
#endif

int rl_comp_func(const void* pv1, const void* pv2);

int rl_check_head(LPRESHEAD pstHead, int iData);

void rl_set_head(LPRESHEAD pstHead, int iCount, int iUnit);

int rl_sload(const char* pszPath, char* pszBuff, int iBuff, int iUnit);
int rl_cload(void** ppvBuff, const char* pszPath, int *piBuff, int *piUnit);

char* rl_find(char* pszBuff, int iCount, int iUnit, int iKey);

#ifdef __cplusplus
}
#endif

#endif /* OV_RESLOADER_H */
