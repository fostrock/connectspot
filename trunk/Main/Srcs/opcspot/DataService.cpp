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
#include "commonlib/filesystem.h"
#include "commonlib/stringstext.h"
#include "ULog.h"

using namespace CommonLib;

loService* DataService::instance = NULL;

int DataService::InitService()
{
	if (instance)
	{
		return 0;
	}

	LPSTR path = NULL;
	std::wstring cfgFile = GetAppDir() + L"config\\zsdriver.xml";
	bool isOK = StringsText::WCharToStr(cfgFile.c_str(), &path);
	_ASSERTE(isOK);
	if (!isOK)
	{
		return -1;
	}
	ZSDriver::Init(path);
	HeapFree(GetProcessHeap(), 0, path);

	loDriver driver;
	memset(&driver, 0, sizeof(loDriver));
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
		ecode = loAddRealTagW(instance, &tagID, (loRealTag)(tagDef.at(i).dataID), 
			tagDef.at(i).name.c_str(), 0, tagDef.at(i).right, &var, 0, NULL);
		if (ecode)
		{
			UL_ERROR((Log::Instance().get(), 0, "loAddRealTagW() failed: %d", ecode));
			return -1;
		}
		ZSDriver::AssignTagIDIndexMap(tagID, tagDef.at(i).dataID);
	}

	ZSDriver::RefreshData(instance);

	return 0;
}

// Uninitialize the data service.
void DataService::UninitService()
{
	ZSDriver::Destroy();
	if (instance)
	{
		int ecode = loServiceDestroy(instance);
		UL_INFO((Log::Instance().get(), 0, "OPC service is closed."));
		instance = NULL;
	}
}

loService* DataService::Instance()
{
	if (NULL == instance)
	{
		int res = InitService();
		if (res)
		{
			UL_ERROR((Log::Instance().get(), 0, "Init the data service failed"));
			return NULL;
		}
	}

	return instance;
}
