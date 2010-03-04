//------------------------------------------------------------------------------------------
// File: <stringstext.h>
// Purpose: declare <string and text manipulating lib>
//
// @author <Yun Hua>
// @version 1.0 2010/03/04
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#ifndef STRINGS_TEXT_H__ 
#define STRINGS_TEXT_H__

#include <string>

namespace CommonLib
{
	class StringsText
	{
	public:
		// Case-insensitive string comparison
		static bool CaseInsCharCompareN(char a, char b) 
		{
			return(toupper(a) == toupper(b));
		}

		static bool CaseInsCharCompareW(wchar_t a, wchar_t b) 
		{
			return(towupper(a) == towupper(b));
		}

		static bool CaseInsCompare(const std::string& s1, const std::string& s2);

		static bool CaseInsCompare(const std::wstring& s1, const std::wstring& s2);
	};
}


#endif

