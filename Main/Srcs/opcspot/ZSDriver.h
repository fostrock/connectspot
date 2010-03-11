//------------------------------------------------------------------------------------------
// File: <ZSDriver.h>
// Purpose: declare <Instrument driver for ZS weight.>
//
// @author <Yun Hua>
// @version 1.0 2010/01/08
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#pragma once

#include "lightopc.h"
#include <vector>
#include <string>
#include "boost/smart_ptr.hpp"

class ZSSerial;

class ZSDriver
{
private:
	ZSDriver(void);
	ZSDriver& operator=(const ZSDriver&);
	ZSDriver(const ZSDriver&);

public:
	// Initialize the driver
	// @param <protocolPath> the path of the protocol used by the driver
	// @return true if the process run successfully.
	static bool Init(const std::string& protocolPath);

	static void activation_monitor(const loCaller *ca, int count, loTagPair *til);

	// Write tag method
	static int WriteTags(const loCaller *ca,
		unsigned count, loTagPair taglist[],
		VARIANT values[], HRESULT error[], HRESULT *master, LCID lcid);

	// Convert tag method
	static void ConvertTags(const loCaller *ca,
		unsigned count, const loTagPair taglist[],
		VARIANT *values, WORD *qualities, HRESULT *errs,
		HRESULT *master_err, HRESULT *master_qual,
		const VARIANT src[], const VARTYPE vtypes[], LCID lcid);

	// Read tag method
	static loTrid ReadTags(const loCaller *ca,
		unsigned count, loTagPair taglist[],
		VARIANT *values, WORD *qualities,
		FILETIME *stamps, HRESULT *errs,
		HRESULT *master_err, HRESULT *master_qual,
		const VARTYPE vtypes[], LCID lcid);

private:
	static std::vector<boost::shared_ptr<ZSSerial> >* serials;
};
