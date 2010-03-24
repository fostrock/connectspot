//------------------------------------------------------------------------------------------
// File: <filesystem.h>
// Purpose: declare <file system utility function.>
//
// @author <Yun Hua>
// @version 1.0 2010/01/24
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#pragma once

#include <string>

namespace CommonLib
{
	// get the running application's directory
	// @return the application's directory
	std::wstring GetAppDir();
}
