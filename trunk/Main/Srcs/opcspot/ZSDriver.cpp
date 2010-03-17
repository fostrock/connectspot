//------------------------------------------------------------------------------------------
// File: <ZSDriver.cpp>
// Purpose: implement <Instrument driver for ZS weight.>
//
// @author <Yun Hua>
// @version 1.0 2010/01/08
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "StdAfx.h"
#include "ZSDriver.h"
#include "ZSSerial.h"
#include <iostream>

std::vector<boost::shared_ptr<ZSSerial> >* ZSDriver::serials = 0;
const loService* ZSDriver::dataService = 0;

bool ZSDriver::Init(const std::string& protocolPath)
{
	bool isOK = false;
	boost::shared_ptr<ZSSerialProtocol> protocol(new ZSSerialProtocol(protocolPath));
	if (!protocol->Parse())
	{
		return isOK;
	}

	serials = new std::vector<boost::shared_ptr<ZSSerial> >;
	const std::vector<ZSSerialSetting>& ports = protocol->GetPortSetting();
	for (std::size_t i = 0; i < ports.size(); ++i)
	{
		try
		{
			boost::shared_ptr<ZSSerial> p(new ZSSerial(ports.at(i).devName, *protocol));
			serials->push_back(p);
		}
		catch (boost::system::system_error& e)
		{
#ifdef _DEBUG
			std::cout << e.what() << std::endl;
#endif
			// add log here
		}
	}
	
	return false;
}

void ZSDriver::activation_monitor(const loCaller *ca, int count, loTagPair *til)
{

}

int ZSDriver::WriteTags(const loCaller *ca,
					 unsigned count, loTagPair taglist[],
					 VARIANT values[], HRESULT error[], HRESULT *master, LCID lcid)
{
	return loDW_TOCACHE;
}

void ZSDriver::ConvertTags(const loCaller *ca,
						unsigned count, const loTagPair taglist[],
						VARIANT *values, WORD *qualities, HRESULT *errs,
						HRESULT *master_err, HRESULT *master_qual,
						const VARIANT src[], const VARTYPE vtypes[], LCID lcid)
{

}

loTrid ZSDriver::ReadTags(const loCaller *ca,
					   unsigned count, loTagPair taglist[],
					   VARIANT *values, WORD *qualities,
					   FILETIME *stamps, HRESULT *errs,
					   HRESULT *master_err, HRESULT *master_qual,
					   const VARTYPE vtypes[], LCID lcid)
{
	return loDR_CACHED;
}

