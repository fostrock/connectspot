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
#include <map>
#include "boost/smart_ptr.hpp"
#include "boost/tuple/tuple.hpp"

class ZSSerial;
class ZSSerialProtocol;

class ZSDriver
{
private:
	ZSDriver(void);
	ZSDriver& operator=(const ZSDriver&);
	ZSDriver(const ZSDriver&);

public:
	struct TAG_DEF 
	{
		std::string name;
		unsigned dataID;
		bool isFloat;
		WORD quality;
	};

	// Initialize the driver
	// @param <protocolPath> the path of the protocol used by the driver
	// @return true if the process run successfully.
	static bool Init(const std::string& protocolPath);

	// Destroy the driver's resource
	static void Destroy();

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

	// Assign the data ID and the OPC tag ID
	// @param <tagID> the OPC tag ID
	// @param <dataIndex> the driver's data index
	static void AssignTagIDIndexMap(unsigned tagID, unsigned dataIndex);
	
	// Get the tag definitions for the outer data service
	// @return the tag definitions
	static std::vector<TAG_DEF> GetTagDef();

private:
	static std::vector<boost::shared_ptr<ZSSerial> >* serials;
	static const loService* dataService;
	static std::vector<unsigned>* tagIDs;
	static std::map<unsigned/*tag ID*/, unsigned/*driver index*/>* tagID2Index;
	static boost::shared_ptr<ZSSerialProtocol> protocol;
};
