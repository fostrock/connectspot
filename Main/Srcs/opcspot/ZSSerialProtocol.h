//------------------------------------------------------------------------------------------
// File: <ZSSerialProtocol.h>
// Purpose: declare <protocol for ZS weight device>
//
// @author <Yun Hua>
// @version 1.0 2010/02/23
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#pragma once

// xmlpp::exception using glib::ustring, it needs to be exported in DLL interface
// we merely disable it here. More detail: 
// see http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#pragma warning(disable:4251)	

#include "boost/noncopyable.hpp"
#include "libxml++/libxml++.h"
#include <map>
#include "boost/tuple/tuple.hpp"


enum ZSSerialParity
{ none, odd, even };

enum ZSSerialStopBits
{ one, onepointfive, two };

enum ZSSerialDataAttr
{ readonly, readwrite, writeonly };

struct ZSSerialSetting
{
	ZSSerialSetting() : csize(8), stopBits(one), parity(none)
	{

	}

	std::string devName;
	unsigned short csize;
	ZSSerialStopBits stopBits; 
	ZSSerialParity parity; 
	unsigned int baudRate;
	std::vector<std::pair<unsigned short/*station No.*/, unsigned short/*1:enable, 0:disable*/> > stations;
};

struct ZSReadDataInfo
{
	int index;
	unsigned short offset;
	unsigned short length;
	bool isFloat;
};

struct ZSReadDataCmd 
{
	unsigned char cmd;
	int refresh; // ms
	std::vector<ZSReadDataInfo> info;
};

struct ZSWriteDataCmd 
{
	unsigned char cmd;
	typedef std::map<unsigned short/*mapping id*/, unsigned short/*param*/> ParamDef;
	ParamDef param;
};

class ZSSerialProtocol : 
	public boost::noncopyable
{
public:
	typedef std::map<int, boost::tuple<std::string, unsigned short, bool, ZSSerialDataAttr> > DataSetDef;
	typedef std::map<unsigned short/*mapping id*/, unsigned char/*cmd*/> CommonCmdDef;

	ZSSerialProtocol(const std::string& cfgFile);
	~ZSSerialProtocol(void);

	// Parse protocol using the DOM parser
	// @return true if the operation succeeded, otherwise false
	bool Parse();

	// Get the serial port setting
	// @return the reference to the serial port setting
	const std::vector<ZSSerialSetting>& GetPortSetting() const
	{
		return vecSetting;
	}

	// Get the data set containing some information
	// @return the reference to the data set
	const DataSetDef& GetDataSetInfo() const
	{
		return dataset;
	}

	// Get the command and the parsing protocol for the reading data
	// @return the reference to the reading data protocol
	const std::vector<ZSReadDataCmd>& GetReadDataCmd() const
	{
		return vecReadDataCmd;
	}

	// Get the command and the parsing protocol for the writing data
	// @return the reference to the writing data protocol
	const ZSWriteDataCmd& GetWriteDataCmd() const
	{
		return writeDataCmd;
	}

	// Get the common commands for controlling the device
	// @return the reference to the common commands
	const CommonCmdDef& GetCommonCmd() const
	{
		return commonCmd;
	}

public:
	static const unsigned short ZS_DATA_NAME_INDEX = 0;
	static const unsigned short ZS_DATA_LENGTH_INDEX = 1;
	static const unsigned short ZS_DATA_TYPE_INDEX = 2;
	static const unsigned short ZS_DATA_ACCESS_INDEX = 3;

private:
	xmlpp::DomParser parser;
	DataSetDef dataset;
	std::vector<ZSReadDataCmd> vecReadDataCmd;
	ZSWriteDataCmd writeDataCmd;
	CommonCmdDef commonCmd;
	std::vector<ZSSerialSetting> vecSetting;
};
