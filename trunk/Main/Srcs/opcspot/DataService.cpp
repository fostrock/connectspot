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

DataService::DataService(void)
{
}

DataService::~DataService(void)
{
}

int DataService::InitService()
{
	loDriver driver;
	memset(&driver, 0, sizeof(loDriver));
	driver.ldSubscribe = ZSDriver::activation_monitor;

	driver.ldFlags = loDF_IGNCASE | loDf_NOFORCE;

	driver.ldWriteTags = ZSDriver::WriteTags;
	driver.ldReadTags = ZSDriver::ReadTags;
	driver.ldConvertTags = ZSDriver::ConvertTags;

	driver.ldBranchSep = '/'; // Hierarchical branch separator

	int ecode = loServiceCreate(&instance, &driver, 4096 /* number of tags in the cache */);
	if (ecode)
	{
		return -1;
	}

	return 0;
}
