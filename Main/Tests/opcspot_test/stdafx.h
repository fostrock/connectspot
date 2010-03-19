// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define BOOST_ALL_DYN_LINK
#define WIN32_LEAN_AND_MEAN // Solve fatal error C1189: #error : WinSock.h has already been included
							// If you use <atlbase.h>, it can be not defined.

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <atlbase.h>