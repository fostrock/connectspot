//------------------------------------------------------------------------------------------
// File: <ZSSerial.cpp>
// Purpose: implement <Data access for ZS weight through RS-232.>
//
// @author <Yun Hua>
// @version 1.0 2010/02/22
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "StdAfx.h"
#include "ZSSerial.h"

ZSSerial::ZSSerial(std::string port, unsigned int baudRate)  : io(), serial(io, port)
{

}

ZSSerial::~ZSSerial(void)
{
}
