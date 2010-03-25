//------------------------------------------------------------------------------------------
// File: <ULog.cpp>
// Purpose: implement <uilog for this application.>
//
// @author <Yun Hua>
// @version 1.0 2010/03/25
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "StdAfx.h"
#include "ULog.h"

boost::shared_ptr<unilog> Log::log;

boost::shared_ptr<unilog> Log::Instance()
{
	if (NULL == log)
	{
		unilog* p = unilog_Create("opcspot.DAServer", // Windows registry entry
			"%TEMP%\\opcspot.log", NULL, 
			1000,      // in KB, max filesize: -1 unlimited, -2 -don't change
			ll_DEBUG   // Level [ll_FATAL...ll_DEBUG]
			);
		log.reset(p, unilog_Delete);  // template<class Y, class D> void reset(Y * p, D d); 
	}

	return log;
}