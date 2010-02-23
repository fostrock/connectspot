//------------------------------------------------------------------------------------------
// File: <Patterns.h>
// Purpose: implement <Declaration of Singleton and other patterns.>
//
// @author <Yun Hua>
// @version 1.1 2010/02/23
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#pragma once

#include "libxml++\libxml++.h"

class XmlDocument
{
private:
	XmlDocument(const XmlDocument&);
	XmlDocument& operator=(const XmlDocument&);
	XmlDocument() {};

public:
	static xmlpp::DomParser& Instance()
	{
		if (xmlFile.empty())
		{
			//char buffer[MAX_PATH];
			//::GetModuleFileNameA(NULL, buffer, MAX_PATH);
			//std::string file(buffer);
			//std::string::size_type pos = file.rfind("\\");
			//pos = file.rfind("\\", pos - 1);
			//xmlFile = file.substr(0, pos + 1) + "config\\zsdriver.xml";
			xmlFile = "E:\\My Projects\\connectspot\\Main\\bin\\config\\zsdriver.xml";
		}

		static xmlpp::DomParser theInstance(xmlFile);
		return theInstance;
	}

	static xmlpp::Node* Root()
	{
		return Instance().get_document()->get_root_node();
	}

	static xmlpp::NodeSet Find(const std::string& xpath)
	{
		return Root()->find(xpath);
	}

	static void SaveFile()
	{
		Instance().get_document()->write_to_file_formatted(xmlFile);
	}

private:
	static std::string xmlFile;	
};