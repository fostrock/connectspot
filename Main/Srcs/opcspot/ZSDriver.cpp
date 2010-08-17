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
#include "ULog.h"

#include "opcda.h"
#include <iostream>


std::vector<boost::shared_ptr<ZSSerial> >* ZSDriver::serials = NULL;
std::vector<loTagValue>* ZSDriver::tags = NULL;
boost::shared_ptr<loService> ZSDriver::dataService;
boost::shared_ptr<ZSSerialProtocol> ZSDriver::protocol;
boost::shared_ptr<boost::thread_group> ZSDriver::threadGp;
boost::shared_ptr<boost::mutex> ZSDriver::mutex;
boost::shared_ptr<boost::shared_mutex> ZSDriver::rwMutex;
bool ZSDriver::isKeepRunning = false;

static const long TIMEOUT_IN_SEC = 2;
static const std::wstring DRV_PREFIX = L"ZS06A";
static const int ZSDRV_COMMON_CMD_START = 61;
static const int ZSDRV_READDATA_GROUP_SWITCH_INTERVAL = 2;
static const unsigned ZSDRV_READ_DATA_GROUP_I = 0;
static const unsigned ZSDRV_READ_DATA_GROUP_II = 1;
static const unsigned REFRESH_MIN = 10; // ms

bool ZSDriver::Init(const std::string& protocolPath)
{
	bool isOK = false;

	// protocol
	protocol.reset(new ZSSerialProtocol(protocolPath));
	if (!protocol->Parse())
	{
		return isOK;
	}

	// thread group
	threadGp.reset(new boost::thread_group);

	// mutex
	mutex.reset(new boost::mutex);
	rwMutex.reset(new boost::shared_mutex);

	// serial objects
	std::size_t total = 0;
	std::size_t fixed = protocol->GetDataSetInfo().size();
	serials = new std::vector<boost::shared_ptr<ZSSerial> >;
	const std::vector<ZSSerialSetting>& ports = protocol->GetPortSetting();
	for (std::size_t i = 0; i < ports.size(); ++i)
	{
		boost::shared_ptr<ZSSerial> p(new ZSSerial(ports.at(i).devName, *protocol));
		p->SetTimeout(boost::posix_time::seconds(TIMEOUT_IN_SEC));	// timeout
		serials->push_back(p);
		total += ports.at(i).stations.size() * fixed;
	}

	tags = new std::vector<loTagValue>(total);

	return true;
}

void ZSDriver::Destroy()
{
	{ // the scope is necessary for the lock
		boost::lock_guard<boost::mutex> guard(*mutex);
		isKeepRunning = false;
	}
	threadGp->join_all();
	if (serials)
	{
		delete serials;
		serials = NULL;
	}
	if (tags)
	{
		delete tags;
		tags = NULL;
	}
}

void ZSDriver::activation_monitor(const loCaller *ca, int count, loTagPair *til)
{

}

int ZSDriver::WriteTags(const loCaller *ca,
						unsigned count, loTagPair taglist[],
						VARIANT values[], HRESULT error[], HRESULT *master, LCID lcid)
{
	// A guard but unsafe
	if (!isKeepRunning || NULL == dataService)
	{
		return loDW_ALLDONE;
	}

	const ZSSerialProtocol::DataSetDef& dataDef = protocol->GetDataSetInfo();
	unsigned fixed = dataDef.size();
	const std::vector<ZSSerialSetting>& ports = protocol->GetPortSetting();
	std::vector<unsigned> segments;
	segments.push_back(0);
	for (std::size_t i = 0; i < ports.size(); ++i)
	{
		unsigned temp = ports.at(i).stations.size() * fixed;
		segments.push_back(temp + segments.at(i));
	}

	for (unsigned i = 0; i < count; ++i)
	{
		std::size_t whichPort = 0;
		unsigned drvIndex = reinterpret_cast<unsigned>(taglist[i].tpRt);
		for (; whichPort < segments.size() - 1; ++whichPort)
		{
			if (drvIndex < segments.at(whichPort + 1))
			{
				break; // the port is matched
			}
		}
		if (segments.size() - 1 == whichPort)
		{
			continue; // no port is matched. Go to the next tag
		}

		unsigned short whichStation = (drvIndex - segments.at(whichPort)) / fixed;
		unsigned short index = (drvIndex - segments.at(whichPort)) % fixed;

		ZSSerialProtocol::DataSetDef::const_iterator iter = dataDef.begin();
		std::advance(iter, index);
		_ASSERTE(iter != dataDef.end());
		if (iter == dataDef.end())
		{
			continue; // error, we do nothing, add log here.
		}

		// check whether the stations is enabled
		if (ZSSerialProtocol::ZS_DEV_DISABLED == 
			ports.at(whichPort).stations.at(whichStation).second && 
			protocol->GetFaultSignalDataIndex() != index)
		{
			continue; // the station is disabled, jump to the next tag.
		}

		HRESULT hr;
		CComVariant var = values[i];
		{
			// write data or command
			boost::upgrade_lock<boost::shared_mutex> upLock(*rwMutex);
			boost::upgrade_to_unique_lock<boost::shared_mutex> uniLock(upLock);

			try
			{
				if (iter->first < ZSDRV_COMMON_CMD_START) // write data
				{
					ZSDataItem dataItem;
					dataItem.index = iter->first;
					if (iter->second.get<ZSSerialProtocol::ZS_DATA_TYPE_INDEX>())
					{
						hr = var.ChangeType(VT_R8);
						_ASSERTE(S_OK == hr);
						if (S_OK != hr)
						{
							continue; // add log here
						}
						dataItem.variant = var.dblVal;
					}
					else
					{
						hr = var.ChangeType(VT_UI4);
						_ASSERTE(S_OK == hr);
						if (S_OK != hr)
						{
							continue; // add log here
						}
						dataItem.variant = var.uintVal;
					}
					serials->at(whichPort)->WriteData(dataItem, 
						ports.at(whichPort).stations.at(whichStation).first);
				}
				else if (protocol->GetFaultSignalDataIndex() != index)// write the common command
				{
					hr = var.ChangeType(VT_UI4);
					_ASSERTE(S_OK == hr);
					if (S_OK != hr)
					{
						continue; // add log here
					}
					if (var.uintVal > 0)
					{
						serials->at(whichPort)->WriteCommand(iter->first, 
							ports.at(whichPort).stations.at(whichStation).first);
					}
					else
					{
						continue;
					}
				}
				else  // check whether the restore the device healthy status is set.	
				{
					hr = var.ChangeType(VT_UI4);
					_ASSERTE(S_OK == hr);
					if (S_OK == hr && 0 == var.uintVal)
					{
						protocol->UpdateStationStatus(whichPort, whichStation, ZSSerialProtocol::ZS_DEV_OK);
					}
					else
					{
						continue;
					}
				}
			}
			catch (timeout_exception& e)
			{
				UL_ERROR((Log::Instance().get(), 0, "WriteData() error: %s", e.what()));		
			}
			catch (boost::system::system_error& e)
			{
				UL_ERROR((Log::Instance().get(), 0, "WriteData() error: %s", e.what()));
			}

		}

		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);
		tags->at(drvIndex).tvState.tsTime = ft;
		tags->at(drvIndex).tvState.tsQuality = OPC_QUALITY_GOOD;
		tags->at(drvIndex).tvValue = var;
		{
			boost::lock_guard<boost::mutex> guard(*mutex);
			loCacheUpdate(dataService.get(), 1, &(tags->at(drvIndex)), 0);
		}
	}

	return loDW_ALLDONE;
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

void ZSDriver::AssignTagIDIndexMap(unsigned tagID, unsigned dataIndex)
{
	_ASSERTE(tags != NULL);
	_ASSERTE(dataIndex < tags->size());
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	(*tags).at(dataIndex).tvValue = CComVariant();
	(*tags).at(dataIndex).tvTi = tagID;
	(*tags).at(dataIndex).tvState.tsTime = ft;
	(*tags).at(dataIndex).tvState.tsError = S_OK;
	(*tags).at(dataIndex).tvState.tsQuality = OPC_QUALITY_GOOD;
}

// Get the tag definitions for the outer data service
std::vector<ZSDriver::TAG_DEF> ZSDriver::GetTagDef()
{
	_ASSERTE(tags != NULL);
	std::vector<TAG_DEF> tagDef;
	tagDef.reserve(tags->size());

	std::size_t inc = 0;
	unsigned fixed = protocol->GetDataSetInfo().size();
	const std::vector<ZSSerialSetting>& ports = protocol->GetPortSetting();
	const ZSSerialProtocol::DataSetDef& dataDef = protocol->GetDataSetInfo();

	for (std::size_t i = 0; i < ports.size(); ++i)
	{
		for (std::size_t j = 0; j < ports.at(i).stations.size(); ++j)
		{
			for (ZSSerialProtocol::DataSetDef::const_iterator it = dataDef.begin();
				it != dataDef.end();
				it++)
			{
				TAG_DEF def;
				def.dataID = inc;
				std::wstringstream wss;

				// Format: COM1_S3_02_ÏÔÊ¾¿ØÖÆ×Ö
				wss << DRV_PREFIX << L"/" << ports.at(i).devName_w << L"/Station_" 
					<< ports.at(i).stations.at(j).first << L"/" 
					<< ports.at(i).devName_w << L"_S"
					<< ports.at(i).stations.at(j).first << L"_"
					<< std::setw(2) << std::setfill(L'0') << (*it).first << L"_"
					<< (*it).second.get<ZSSerialProtocol::ZS_DATA_NAME_INDEX>();
				def.name = wss.str();
				def.type = (*it).second.get<ZSSerialProtocol::ZS_DATA_TYPE_INDEX>() ? VT_R8 : VT_UI4; 
				ZSSerialDataAttr attr = (*it).second.get<ZSSerialProtocol::ZS_DATA_ACCESS_INDEX>();
				if (readonly == attr)
				{
					def.right = OPC_READABLE;
				}
				else if (writeonly == attr)
				{
					def.right = OPC_WRITEABLE;
				} 
				else
				{
					def.right = OPC_READABLE | OPC_WRITEABLE;
				}
				tagDef.push_back(def);
				inc++;
			}

		}
	}

	return tagDef;
}

// Tell the driver to refresh data
// @param <service> opc data service
void ZSDriver::RefreshData(boost::shared_ptr<loService> service)
{
	dataService = service;
	boost::lock_guard<boost::mutex> guard(*mutex);
	isKeepRunning = true;
	for (std::size_t i = 0; i < serials->size(); ++i)
	{
		if (serials->at(i)->IsOpened())
		{
			// equal to: boost::thread(boost::bind(RefreshDataTask, dataService, i));
			boost::thread* p = new boost::thread(RefreshDataTask, dataService, i);
			threadGp->add_thread(p);
		}
	}
}

// Refresh data worker function
void ZSDriver::RefreshDataTask(boost::shared_ptr<loService> service, unsigned serialIndex)
{
	boost::shared_ptr<ZSSerial> serial = serials->at(serialIndex);
	const std::vector<std::pair<unsigned char, unsigned short> >& stations =
		protocol->GetPortSetting().at(serialIndex).stations;
	std::string devName = protocol->GetPortSetting().at(serialIndex).devName;
	std::size_t gpOneCount = protocol->GetReadDataCmd().at(0).info.size();
	std::size_t gpTwoCount = protocol->GetReadDataCmd().at(1).info.size();
	std::size_t dataTotalCount = protocol->GetDataSetInfo().size();

	unsigned refreshBase = protocol->GetReadDataCmd().at(ZSDRV_READ_DATA_GROUP_I).refresh;
	refreshBase = (0 == refreshBase) ? REFRESH_MIN : refreshBase; // at lease REFRESH_MIN
	unsigned interval = 
		protocol->GetReadDataCmd().at(ZSDRV_READ_DATA_GROUP_II).refresh / refreshBase;
	interval = // at least ZSDRV_READDATA_GROUP_SWITCH_INTERVAL
		(interval < ZSDRV_READDATA_GROUP_SWITCH_INTERVAL) ? ZSDRV_READDATA_GROUP_SWITCH_INTERVAL : interval;

	std::size_t grpStartOffset = 0;
	for (std::size_t i = 0; i < serialIndex; i++)
	{
		grpStartOffset += 
			dataTotalCount * protocol->GetPortSetting().at(i).stations.size();
	}

	while (isKeepRunning)
	{
		for (std::size_t i = 0; i < stations.size(); ++i)
		{
			// the station is disabled
			if (ZSSerialProtocol::ZS_DEV_DISABLED == stations.at(i).second)
			{
				::Sleep(REFRESH_MIN);
				continue;
			}

			unsigned dataGroup;
			if (interval > 0)
			{
				dataGroup = ZSDRV_READ_DATA_GROUP_I;
			}
			else
			{
				dataGroup = ZSDRV_READ_DATA_GROUP_II;
			}

			try // refresh data
			{
				RefreshDataSubJob(service, serial, stations.at(i).first, 
					dataGroup, grpStartOffset + i * dataTotalCount);
				::Sleep(refreshBase);
				protocol->UpdateStationStatus(serialIndex, i, ZSSerialProtocol::ZS_DEV_OK); // restore
			}
			catch (timeout_exception& e)
			{
				NotifyDevFault(service, serialIndex, i, grpStartOffset + i * dataTotalCount);
				UL_ERROR((Log::Instance().get(), 0, "ReadData error: %s - %s", 
					devName.c_str(), e.what() ));
			}
			catch (boost::system::system_error& e)
			{
				NotifyDevFault(service, serialIndex, i, grpStartOffset + i * dataTotalCount);
				UL_ERROR((Log::Instance().get(), 0, "ReadData error: %s - %s", 
					devName.c_str(), e.what() ));		
			}
		}
		if (0 == interval)
		{
			interval = ZSDRV_READDATA_GROUP_SWITCH_INTERVAL;
		}
		else
		{
			interval--;
		}
	}
}

// It is the real job body for RefreshDataTask
void ZSDriver::RefreshDataSubJob(boost::shared_ptr<loService> service, boost::shared_ptr<ZSSerial> serial, 
								 unsigned char station, unsigned group, 
								 std::size_t startOffset
								 )
{
	_ASSERTE(service != NULL);
	std::vector<ZSDataItem> items;
	std::size_t curIndex;
	std::size_t gpCount;
	std::size_t gpOneCount = protocol->GetReadDataCmd().at(ZSDRV_READ_DATA_GROUP_I).info.size();
	std::size_t gpTwoCount = protocol->GetReadDataCmd().at(ZSDRV_READ_DATA_GROUP_II).info.size();
	{
		boost::shared_lock<boost::shared_mutex> rLock(*rwMutex);
		if (ZSDRV_READ_DATA_GROUP_I == group)
		{
			items = serial->ReadData(ZSSerial::one, station);
			_ASSERTE(gpOneCount == items.size());
			if (gpOneCount != items.size())
			{
				// add log here
				return;
			}
			curIndex = startOffset;
			gpCount = gpOneCount;
		}
		else if (ZSDRV_READ_DATA_GROUP_II == group)
		{
			items = serial->ReadData(ZSSerial::two, station);
			_ASSERTE(gpTwoCount == items.size());
			if (gpTwoCount != items.size())
			{
				// add log here
				return;
			}
			curIndex = startOffset + gpOneCount;
			gpCount = gpTwoCount;
		}
		else
		{
			_ASSERTE(!"unsupported read data group indicator.");
			return;
		}
	}

	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	for (std::size_t j = 0; j < items.size(); ++j)
	{
		tags->at(curIndex + j).tvState.tsTime = ft;
		tags->at(curIndex + j).tvState.tsQuality = OPC_QUALITY_GOOD;
		CComVariant var;
		if (unsigned* pui = boost::get<unsigned>(&items.at(j).variant))
		{
			var = *pui;
		}
		else if (double* pd = boost::get<double>(&items.at(j).variant))
		{
			var = *pd;
		}
		else
		{
			_ASSERTE(!"unexpected variant type");
		}
		tags->at(curIndex + j).tvValue = var;
	}

	// update the service cache
	{
		boost::lock_guard<boost::mutex> guard(*mutex);
		loCacheUpdate(service.get(), gpCount, &(tags->at(curIndex)), 0);
	}
}


void ZSDriver::NotifyDevFault(boost::shared_ptr<loService> service, unsigned serialIndex, 
						   unsigned char stationIndex, unsigned int startOffset)
{
	_ASSERTE(service != NULL);
	unsigned short runSign = 
		protocol->GetPortSetting().at(serialIndex).stations.at(stationIndex).second;

	// The station is already disabled
	if (ZSSerialProtocol::ZS_DEV_DISABLED == runSign)
	{
		return;
	}

	// Not exceeds the maximum retry time
	if (runSign >= ZSSerialProtocol::ZS_DEV_OK && 
		runSign <=  ZSSerialProtocol::ZS_DEV_FAULT_MAX_TRY_TIMES)
	{
		protocol->UpdateStationStatus(serialIndex, stationIndex, runSign + 1);
		return;
	}

	// Fetal error occurred. The data refresh to the device shall be disabled.
	protocol->UpdateStationStatus(serialIndex, stationIndex, ZSSerialProtocol::ZS_DEV_DISABLED);

	// Signal the error to the user.
	unsigned int signalIndex = startOffset + protocol->GetFaultSignalDataIndex();
	unsigned int gpCount = protocol->GetReadDataCmd().at(ZSDRV_READ_DATA_GROUP_I).info.size() + 
		protocol->GetReadDataCmd().at(ZSDRV_READ_DATA_GROUP_II).info.size();

	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	CComVariant var((unsigned)1);

	for (std::size_t i = startOffset; i < startOffset + gpCount; ++i)
	{
		tags->at(i).tvState.tsTime = ft;
		tags->at(i).tvState.tsQuality = OPC_QUALITY_DEVICE_FAILURE;
	}

	tags->at(signalIndex).tvState.tsTime = ft;
	tags->at(signalIndex).tvState.tsQuality = OPC_QUALITY_GOOD;
	tags->at(signalIndex).tvValue = var;
	{
		boost::lock_guard<boost::mutex> guard(*mutex);
		loCacheUpdate(service.get(), gpCount, &(tags->at(startOffset)), 0);
		loCacheUpdate(service.get(), 1, &(tags->at(signalIndex)), 0);
	}
}