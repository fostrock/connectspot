//------------------------------------------------------------------------------------------
// File: <ULog.h>
// Purpose: declare <uilog for this application.>
//
// @author <Yun Hua>
// @version 1.0 2010/03/25
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#pragma once

#include "boost/smart_ptr.hpp"
#include "unilog.h"

class Log
{
private:
	Log(void);
	Log(const Log&);
	Log& operator=(const Log&);

public:
	static boost::shared_ptr<unilog> Instance();

private:
	static boost::shared_ptr<unilog> log;
};
