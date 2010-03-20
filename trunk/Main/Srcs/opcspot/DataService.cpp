//------------------------------------------------------------------------------------------
// File: <DataService.cpp>
// Purpose: implement <data service which consumed by the opc da server.>
//
// @author <Yun Hua>
// @version 1.0 2010/02/08
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "StdAfx.h"
#include "DataService.h"
#include "ZSDriver.h"

loService* DataService::instance = NULL;

int DataService::InitService()
{
	loDriver driver;
	memset(&driver, 0, sizeof(loDriver));
	ZSDriver::Init("d:\\zsdriver.xml");

	driver.ldFlags = loDF_IGNCASE | loDf_NOFORCE & 0;
    driver.ldRefreshRate = 500;
	driver.ldWriteTags = ZSDriver::WriteTags;
	driver.ldReadTags = ZSDriver::ReadTags;

	driver.ldBranchSep = '/'; // Hierarchical branch separator

	int ecode = loServiceCreate(&instance, &driver, 4096 /* number of tags in the cache */);
	if (ecode)
	{
		return -1;
	}

	std::vector<ZSDriver::TAG_DEF> tagDef = ZSDriver::GetTagDef();
	loTagId tagID = 0;
	for (std::size_t i = 0; i < tagDef.size(); ++i)
	{
		CComVariant var;
		if (VT_R8 == tagDef.at(i).type)
		{
			var = 0.0;
		}
		else if (VT_UI4 == tagDef.at(i).type)
		{
			var = (unsigned)0;
		}
		loAddRealTagW(instance, &tagID, (loRealTag)(tagDef.at(i).dataID), 
			tagDef.at(i).name.c_str(), 0, tagDef.at(i).right, &var, 0, NULL);
		ZSDriver::AssignTagIDIndexMap(tagID, tagDef.at(i).dataID);
	}

	ZSDriver::RefreshData(instance);

	return 0;
}
