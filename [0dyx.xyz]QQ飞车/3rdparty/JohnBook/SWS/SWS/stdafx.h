/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _STDAFX_H
#define _STDAFX_H

#pragma once

#include <windows.h>
#include <stdio.h>
#include <BugslayerUtil.h>

#define NEW_INLINE_WORKAROUND new ( _NORMAL_BLOCK ,\
                                    __FILE__      , \
                                    __LINE__       )
#define new NEW_INLINE_WORKAROUND

#endif
