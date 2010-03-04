//------------------------------------------------------------------------------------------
// File: <stringstext.cpp>
// Purpose: implement <string and text manipulating lib>
//
// @author <Yun Hua>
// @version 1.0 2010/03/04
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "stdafx.h"
#include "stringstext.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cwctype>

using namespace std;

namespace CommonLib
{
	bool StringsText::CaseInsCompare(const string& s1, const string& s2) 
	{
		return((s1.size( ) == s2.size( )) &&
			equal(s1.begin( ), s1.end( ), s2.begin( ), CaseInsCharCompareN));
	}

	bool StringsText::CaseInsCompare(const wstring& s1, const wstring& s2) 
	{
		return((s1.size( ) == s2.size( )) &&
			equal(s1.begin( ), s1.end( ), s2.begin( ), CaseInsCharCompareW));
	}
}

