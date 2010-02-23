//------------------------------------------------------------------------------------------
// File: <ZSSerialProtocol.h>
// Purpose: implement <protocol for ZS weight device>
//
// @author <Yun Hua>
// @version 1.0 2010/02/23
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "StdAfx.h"
#include "ZSSerialProtocol.h"

ZSSerialProtocol::ZSSerialProtocol(const std::string& cfgFile) : parser(cfgFile)
{
}

ZSSerialProtocol::~ZSSerialProtocol(void)
{
}
