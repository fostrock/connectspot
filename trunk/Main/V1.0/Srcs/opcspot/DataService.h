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
	// Initialize the data service including the device driver.
	// @return 0 if it is successful, otherwise failed
	static int InitService();

	// Uninitialize the data service.
	static void UninitService();

	// Get the internal instance
	// @return the service instance
	static loService* Instance();

private:
	static loService* instance;
};
