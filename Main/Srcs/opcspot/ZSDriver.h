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
#include "boost/thread.hpp"

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
		std::wstring name;
		unsigned dataID;
		VARTYPE type;
		WORD right;
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

	// Tell the driver to refresh data
	// @param <service> opc data service
	static void RefreshData(loService* service);

private:
	// Refresh data worker function
	// @param <service> opc data service
	// @param <serialIndex> serial object index
	static void RefreshDataTask(loService* service, unsigned serialIndex);

	// It is the real job body for RefreshDataTask
	// @param <serial> Zongshi serial port object
	// @param <group> data read group for zsserial, 
	//                0 for readData group one, 1 for readData group two
	// @param <startOffset> the start offset of tags
	// @param <stationIndex> RS485 station No. index in ZSSerialSetting::stations
	static void RefreshDataSubJob(loService* service, 
		boost::shared_ptr<ZSSerial> serial, 
		unsigned char station, unsigned group, 
		std::size_t startOffset);

private:
	static std::vector<boost::shared_ptr<ZSSerial> >* serials;
	static const loService* dataService;
	static std::vector<loTagValue>* tags;
	static std::map<unsigned/*tag ID*/, unsigned/*driver index*/>* tagID2Index;
	static boost::shared_ptr<ZSSerialProtocol> protocol;
	static boost::shared_ptr<boost::thread_group> threadGp;
	static boost::shared_ptr<boost::mutex> mutex;
	static boost::shared_ptr<boost::shared_mutex> rwMutex;
	static bool isKeepRunning;
};
