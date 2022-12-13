////////////////////////////////////////////////
//     FileName:    log.h
//     Author:      Fish
//     Date:        2006-9-26 16:56:00
//     Description: []
////////////////////////////////////////////////

#pragma once

#include "common.h"

#ifndef _NOLOG
#define LOG(x)		LogText(x)
#define LOGLINE(x)	LOG(x##"\n")
#else
#define LOG(x)
#define LOGLINE(x)
#endif