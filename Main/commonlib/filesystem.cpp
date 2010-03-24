//------------------------------------------------------------------------------------------
// File: <filesystem.cpp>
// Purpose: implement <file system utility function.>
//
// @author <Yun Hua>
// @version 1.0 2010/01/24
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "stdafx.h"
#include "filesystem.h"
#include "windows.h"

namespace CommonLib
{
	// get the running application's directory
	std::wstring GetAppDir()
	{
		wchar_t buffer[MAX_PATH];
		::GetModuleFileNameW(NULL, buffer, MAX_PATH);
		std::wstring path(buffer);
		std::size_t pos = path.rfind(L"\\");
		return path.substr(0, pos + 1);
	}
}
