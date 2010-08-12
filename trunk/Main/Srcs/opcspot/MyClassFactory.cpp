//------------------------------------------------------------------------------------------
// File: <MyClassFactory.cpp>
// Purpose: implement <Self-defined class factory to create the opc server object.>
//
// @author <Yun Hua>
// @version 1.0 2010/01/05
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "StdAfx.h"
#include "MyClassFactory.h"
#include "lightopc.h"
#include "DataService.h"
#include "opcda.h"
#include "ULog.h"
#include <boost/thread/thread.hpp>

static const loVendorInfo vendor = {
	1 /*Major */ , 0 /*Minor */ , 1 /*Build */ , 0 /*Reserv */ ,
	"ZongShi OPC DA Server"
};

static void a_server_finished(void *arg, loService *b, loClient *c)
{
	DataService::UninitService();
	_pAtlModule->m_nLockCnt = 1; // Decrease the reference count by force.
	_pAtlModule->Unlock();
}

static void LicenseTimeout()
{
	Sleep(96 * 3600 * 1000);
	DataService::UninitService();
}

MyClassFactory::MyClassFactory(void)
{
}

MyClassFactory::~MyClassFactory(void)
{
}

HRESULT MyClassFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj)
{
	ATLASSERT(ppvObj);
	if (NULL == ppvObj)
	{
		return E_INVALIDARG;
	}
	*ppvObj = NULL;

	HRESULT hr = S_OK;
	IUnknown *server = NULL;
	IUnknown *inner = NULL;

	if (NULL == DataService::Instance())
	{
		return E_POINTER;
	}

	if (loClientCreate_agg(DataService::Instance().get(), (loClient**)&server, 
		pUnkOuter, &inner,
		0, &vendor, a_server_finished, NULL/*cuserid*/))
	{
		// server remove
		UL_ERROR((Log::Instance().get(), 0, "myClassFactory::loClientCreate_agg() failed"));
		hr = E_OUTOFMEMORY;
	}
	else if (pUnkOuter)
	{
		*ppvObj = (void*)inner; // aggregation requested
	}	
	else // no aggregation
	{
		hr = server->QueryInterface(riid, ppvObj); // Then 2 (if success)
		server->Release(); // Then 1 (on behalf of client) or 0 (if QI failed)
	}

	if (SUCCEEDED(hr))
	{
		loSetState(DataService::Instance().get(), (loClient*)server,
			loOP_OPERATE, (int)OPC_STATUS_RUNNING, "Finished by client");
		UL_INFO((Log::Instance().get(), 0, "OPC service is started."));
	}

#ifdef __EVAL__
	boost::shared_ptr<boost::thread> licThread(new boost::thread(LicenseTimeout)); 
#endif

	return hr;
}
