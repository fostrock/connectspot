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
#include "boost/lexical_cast.hpp"
#include "boost/tokenizer.hpp"
#include <iostream>
#include <algorithm>
#include "commonlib/stringstext.h"

using namespace CommonLib;

static const std::string LANG_CHS = "chs";
static const std::string LANG_ENG = "eng";

ZSSerialProtocol::ZSSerialProtocol(const std::string& cfgFile) : parser(cfgFile)
{
}

ZSSerialProtocol::~ZSSerialProtocol(void)
{
}

// Parse protocol using the DOM parser
bool ZSSerialProtocol::Parse()
{
	bool ret = false;
	try
	{
		const xmlpp::Node* root = parser.get_document()->get_root_node();
		if(root)
		{
			// device setting
			xmlpp::NodeSet set = root->find("//zsdriver/serialport");
			if (set.empty())
			{
				return false;
			}

			for (std::size_t i = 0; i < set.size(); ++i)
			{
				const xmlpp::Element* dataItem = static_cast<const xmlpp::Element*>(set.at(i));
				_ASSERTE(dataItem != NULL);

				ZSSerialSetting setting;
				setting.devName = dataItem->get_attribute("port")->get_value();


				std::string strSetting = dataItem->get_attribute("setting")->get_value();
				boost::char_separator<char> sep(",");
				typedef boost::tokenizer<boost::char_separator<char> >  tokenizer;
				tokenizer tokSetting(strSetting, sep);

				_ASSERTE(4 == std::distance(tokSetting.begin(), tokSetting.end()));
				if (4 != std::distance(tokSetting.begin(), tokSetting.end()))
				{
					throw std::runtime_error("serial device setting error");
				}
				tokenizer::iterator itSetting = tokSetting.begin();
				setting.baudRate = boost::lexical_cast<unsigned int>(*itSetting);
				++itSetting;
				setting.csize = boost::lexical_cast<unsigned short>(*itSetting);
				++itSetting;
				if (StringsText::CaseInsCompare("E", *itSetting))	// setting.parity's default value is none
				{
					setting.parity = even;
				}
				else if (StringsText::CaseInsCompare("O", *itSetting))
				{
					setting.parity = odd;
				}
				++itSetting;	// setting.stopBits' default value is one
				unsigned int stopBits = boost::lexical_cast<unsigned int>(*itSetting);
				if (1.5 == stopBits)
				{
					setting.stopBits = onepointfive;
				}
				else if (2 == stopBits)
				{
					setting.stopBits = two;
				}

				// check R485 stations
				std::string strStations = dataItem->get_attribute("stations")->get_value();
				tokenizer tokStation(strStations, sep);
				_ASSERTE(0 == (std::distance(tokStation.begin(), tokStation.end()) % 2));
				if (0 != (std::distance(tokStation.begin(), tokStation.end()) % 2))
				{
					throw std::runtime_error("RS485 station config error");
				}
				for (tokenizer::iterator itStation = tokStation.begin(); itStation != tokStation.end();
					++itStation)
				{
					// Attention: boost::lexical_cast<unsigned char>(1) -> 0x31 or character '1'
					unsigned char station = 
						static_cast<unsigned char>(boost::lexical_cast<unsigned short>(*itStation));
					unsigned short enable = 0;
					++itStation;
					if (StringsText::CaseInsCompare("Y", *itStation))
					{
						enable = 1;
					}
					setting.stations.push_back(std::make_pair(station, enable));
				}

				// add setting
				vecSetting.push_back(setting);
			}

			// data lang definition
			set = root->find("//zsdriver/protocol/dataset");
			_ASSERTE(set.size() > 0);
			const xmlpp::Element* datasetItem = static_cast<const xmlpp::Element*>(set.at(0));
			_ASSERTE(datasetItem != NULL);
			std::string lang = datasetItem->get_attribute("lang")->get_value();

			// device data
			set = root->find("//zsdriver/protocol/dataset/data");
			for (std::size_t i = 0; i < set.size(); ++i)
			{	
				const xmlpp::Element* dataItem = static_cast<const xmlpp::Element*>(set.at(i));
				_ASSERTE(dataItem != NULL);

				int dataID = boost::lexical_cast<int>(dataItem->get_attribute("id")->get_value());
				std::wstring dataName = L"error_parse_name";
				if (StringsText::CaseInsCompare(LANG_ENG, lang))
				{
					LPWSTR pName = StringsText::StrToWChar(
						dataItem->get_attribute("name_eng")->get_value().c_str());
					_ASSERTE(pName != NULL);
					dataName = pName;
					HeapFree(GetProcessHeap(), 0, pName);
				}
				else if (StringsText::CaseInsCompare(LANG_CHS, lang))
				{
					LPWSTR pName = StringsText::StrToWChar(
						dataItem->get_attribute("name_chs")->get_value().c_str());
					_ASSERTE(pName != NULL);
					dataName = pName;
					HeapFree(GetProcessHeap(), 0, pName);
				}
				else
				{
					_ASSERTE(!"unsupported language type");
				}
				unsigned short length = boost::lexical_cast<unsigned short>(dataItem->get_attribute("length")->get_value());			
				bool isFloat = StringsText::CaseInsCompare("TRUE", dataItem->get_attribute("float")->get_value());
				std::string strAttr = dataItem->get_attribute("access")->get_value();
				ZSSerialDataAttr attr = readonly;
				if (StringsText::CaseInsCompare("r", strAttr))
				{
					attr = readonly;
				}
				else if (StringsText::CaseInsCompare("w", strAttr))
				{
					attr = writeonly;
				}
				else if (StringsText::CaseInsCompare("rw", strAttr))
				{
					attr = readwrite;
				}
				else
				{
					_ASSERTE(!"unsupported data access type");
				}

				dataset.insert(std::make_pair(dataID, boost::make_tuple(dataName, length, isFloat, attr)));
			}

			// read commands
			set = root->find("//zsdriver/protocol/read");
			for (std::size_t i = 0; i < set.size(); ++i)
			{
				const xmlpp::Element* dataItem = static_cast<const xmlpp::Element*>(set.at(i));
				_ASSERTE(dataItem != NULL);

				vecReadDataCmd.push_back(ZSReadDataCmd());
				// cmd in HEX
				long lCmd = strtol(dataItem->get_attribute("cmd")->get_value().c_str(), NULL, 0);
				vecReadDataCmd.at(i).cmd = static_cast<unsigned char>(lCmd);
				// refresh rate
				vecReadDataCmd.at(i).refresh = 
					boost::lexical_cast<unsigned>(dataItem->get_attribute("refresh")->get_value());
				// child nodes
				const xmlpp::Node::NodeList children = dataItem->get_children("data");
				for (xmlpp::Node::NodeList::const_iterator it = children.begin(); it != children.end(); ++it)
				{
					const xmlpp::Element* child = static_cast<const xmlpp::Element*>(*it);
					int matchID = boost::lexical_cast<int>(child->get_attribute("match_id")->get_value());
					unsigned short offset = boost::lexical_cast<unsigned short>(child->get_attribute("offset")->get_value());
					// search the parsed data set
					DataSetDef::iterator itData = dataset.find(matchID);
					_ASSERTE(itData != dataset.end());
					if (itData != dataset.end())
					{
						ZSReadDataInfo readDataInfo;
						readDataInfo.index = matchID;
						readDataInfo.offset = offset;
						readDataInfo.length = (*itData).second.get<ZS_DATA_LENGTH_INDEX>();
						readDataInfo.isFloat = (*itData).second.get<ZS_DATA_TYPE_INDEX>();
						vecReadDataCmd.at(i).info.push_back(readDataInfo);
					}		
				}
			}

			// write commands
			set = root->find("//zsdriver/protocol/write");
			_ASSERTE(set.size() == 1);
			{
				const xmlpp::Element* dataItem = static_cast<const xmlpp::Element*>(set.at(0));
				_ASSERTE(dataItem != NULL);

				// cmd in HEX
				long lCmd = strtol(dataItem->get_attribute("cmd")->get_value().c_str(), NULL, 0);
				writeDataCmd.cmd = static_cast<unsigned char>(lCmd);
				const xmlpp::Node::NodeList children = dataItem->get_children("data");
				for (xmlpp::Node::NodeList::const_iterator it = children.begin(); it != children.end(); ++it)
				{
					const xmlpp::Element* child = static_cast<const xmlpp::Element*>(*it);
					unsigned short matchID = boost::lexical_cast<unsigned short>(child->get_attribute("match_id")->get_value());
					unsigned short param = boost::lexical_cast<unsigned short>(child->get_attribute("param")->get_value());;
					writeDataCmd.param.insert(std::make_pair(matchID, param));
				}
			}

			// common commands
			set = root->find("//zsdriver/protocol/command/data");
			for (std::size_t i = 0; i < set.size(); ++i)
			{	
				const xmlpp::Element* dataItem = static_cast<const xmlpp::Element*>(set.at(i));
				_ASSERTE(dataItem != NULL);

				unsigned short matchID = boost::lexical_cast<unsigned short>(dataItem->get_attribute("match_id")->get_value());
				// cmd in HEX
				long lCmd = strtol(dataItem->get_attribute("cmd")->get_value().c_str(), NULL, 0);
				commonCmd.insert(std::make_pair(matchID, static_cast<unsigned char>(lCmd)));
			}

			ret = true;
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << "Exception caught: " << ex.what() << std::endl;
	}

	return ret;
}
