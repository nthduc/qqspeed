#ifndef _CLIENT_BIN_READER_H_
#define _CLIENT_BIN_READER_H_
extern "C" {
int rl_cload(void** ppvBuff, const char* pszPath, int *piBuff, int *piUnit);
int rl_sloadvf(const char* pszPath, char* pszBuff, int iBuff, int iUnit);
int rl_cloadvf(void** ppvBuff, const char* pszPath, int *piBuff, int *piUnit);
char* rl_find(char* pszBuff, int iCount, int iUnit, int iKey);
}

#endif