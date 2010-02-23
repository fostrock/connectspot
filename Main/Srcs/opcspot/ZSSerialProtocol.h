//------------------------------------------------------------------------------------------
// File: <ZSSerialProtocol.h>
// Purpose: declare <protocol for ZS weight device>
//
// @author <Yun Hua>
// @version 1.0 2010/02/23
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#pragma once

#include "boost/utility.hpp"
#include "libxml++/libxml++.h"

class ZSSerialProtocol : 
	public boost::noncopyable
{
public:
	ZSSerialProtocol(const std::string& cfgFile);
	~ZSSerialProtocol(void);

private:
	xmlpp::DomParser parser;
};
