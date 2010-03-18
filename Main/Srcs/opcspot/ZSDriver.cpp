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
#include "opcda.h"

std::vector<boost::shared_ptr<ZSSerial> >* ZSDriver::serials = NULL;
const loService* ZSDriver::dataService = NULL;
std::vector<unsigned>* ZSDriver::tagIDs = NULL;
std::map<unsigned, unsigned>* ZSDriver::tagID2Index = NULL;
boost::shared_ptr<ZSSerialProtocol> ZSDriver::protocol;

bool ZSDriver::Init(const std::string& protocolPath)
{
	bool isOK = false;
	protocol.reset(new ZSSerialProtocol(protocolPath));
	if (!protocol->Parse())
	{
		return isOK;
	}

	unsigned total = 0;
	unsigned fixed = protocol->GetDataSetInfo().size();
	serials = new std::vector<boost::shared_ptr<ZSSerial> >;
	const std::vector<ZSSerialSetting>& ports = protocol->GetPortSetting();
	for (std::size_t i = 0; i < ports.size(); ++i)
	{
		boost::shared_ptr<ZSSerial> p(new ZSSerial(ports.at(i).devName, *protocol));
		serials->push_back(p);
		total += ports.at(i).stations.size() * fixed;
	}

	tagIDs = new std::vector<unsigned>(total);
	tagID2Index = new std::map<unsigned, unsigned>;
	
	return true;
}

void ZSDriver::Destroy()
{
	if (serials)
	{
		delete serials;
		serials = NULL;
	}
	if (tagIDs)
	{
		delete tagIDs;
		tagIDs = NULL;
	}
	if (tagID2Index)
	{
		delete tagID2Index;
		tagID2Index = NULL;
	}
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

void ZSDriver::AssignTagIDIndexMap(unsigned tagID, unsigned dataIndex)
{
	_ASSERTE(tagIDs != NULL && tagID2Index != NULL);
	_ASSERTE(dataIndex < tagIDs->size());
	(*tagIDs).at(dataIndex) = tagID;
	tagID2Index->insert(std::make_pair(tagID, dataIndex));
}

// Get the tag definitions for the outer data service
std::vector<ZSDriver::TAG_DEF> ZSDriver::GetTagDef()
{
	_ASSERTE(tagIDs != NULL);
    std::vector<TAG_DEF> tagDef;
	tagDef.reserve(tagIDs->size());

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
				def.name = (*it).second.get<ZSSerialProtocol::ZS_DATA_NAME_INDEX>();
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

