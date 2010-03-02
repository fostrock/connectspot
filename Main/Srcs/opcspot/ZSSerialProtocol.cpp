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
#include <iostream>

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
			xmlpp::NodeSet set = root->find("//zsdriver/protocol/dataset/data");
			for (std::size_t i = 0; i < set.size(); ++i)
			{	
				const xmlpp::Element* dataItem = static_cast<const xmlpp::Element*>(set.at(i));
				_ASSERTE(dataItem != NULL);

				int dataID = boost::lexical_cast<int>(dataItem->get_attribute("id")->get_value());
				std::string chsName = boost::lexical_cast<std::string>(dataItem->get_attribute("name_chs")->get_value());
				std::string engName = dataItem->get_attribute("name_eng")->get_value();
				unsigned short length = boost::lexical_cast<unsigned short>(dataItem->get_attribute("length")->get_value());			
				bool isFloat = (dataItem->get_attribute("length")->get_value().uppercase() == "TRUE");

				dataset.insert(std::make_pair(dataID, boost::make_tuple(chsName, engName , length, isFloat)));
			}

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
				vecReadDataCmd.at(i).refresh = boost::lexical_cast<int>(dataItem->get_attribute("refresh")->get_value());
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
						readDataInfo.length = (*itData).second.get<2>();
						vecReadDataCmd.at(i).info.push_back(readDataInfo);
					}		
				}
			}

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

			set = root->find("//zsdriver/protocol/command/data");
			for (std::size_t i = 0; i < set.size(); ++i)
			{	
				const xmlpp::Element* dataItem = static_cast<const xmlpp::Element*>(set.at(i));
				_ASSERTE(dataItem != NULL);

				unsigned short matchID = boost::lexical_cast<unsigned short>(dataItem->get_attribute("match_id")->get_value());
				// cmd in HEX
				long lCmd = strtol(dataItem->get_attribute("cmd")->get_value().c_str(), NULL, 0);
				commonCmd.insert(std::make_pair(matchID, static_cast<unsigned short>(lCmd)));
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
