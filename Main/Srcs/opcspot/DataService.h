//------------------------------------------------------------------------------------------
// File: <DataService.h>
// Purpose: declare <data service which consumed by the opc da server.>
//
// @author <Yun Hua>
// @version 1.0 2010/02/08
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#pragma once

#include "lightopc.h"

class DataService
{
private:
	DataService(void);
	DataService& operator=(const DataService&);
	DataService(const DataService&);

public:
	static int InitService();

public:
	static loService* instance;
};
