
#pragma once

#include "typedef.h"
#include "list"
using namespace std;
typedef list<SOCKET_CREATE_INFO>	SOCKET_CREATE_INFO_LIST;
BOOL LoadServerCFG(ServerEntity Entity, SOCKET_CREATE_INFO_LIST& listSocketInfo);
void LoadServerCFGFromFile(const char* szProfile, SOCKET_CREATE_INFO_LIST& listSocketInfo);
const char* GetAddrByName(const char*);