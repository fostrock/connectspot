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

class ZSDriver
{
private:
	ZSDriver(void);
	~ZSDriver(void);
	ZSDriver& operator=(const ZSDriver&);

public:

	static void activation_monitor(const loCaller *ca, int count, loTagPair *til);

	static int WriteTags(const loCaller *ca,
		unsigned count, loTagPair taglist[],
		VARIANT values[], HRESULT error[], HRESULT *master, LCID lcid);

	static void ConvertTags(const loCaller *ca,
		unsigned count, const loTagPair taglist[],
		VARIANT *values, WORD *qualities, HRESULT *errs,
		HRESULT *master_err, HRESULT *master_qual,
		const VARIANT src[], const VARTYPE vtypes[], LCID lcid);

	static loTrid ReadTags(const loCaller *ca,
		unsigned count, loTagPair taglist[],
		VARIANT *values, WORD *qualities,
		FILETIME *stamps, HRESULT *errs,
		HRESULT *master_err, HRESULT *master_qual,
		const VARTYPE vtypes[], LCID lcid);
};
