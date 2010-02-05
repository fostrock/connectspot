// DAServer.h : Declaration of the DAServer

#pragma once
#include "resource.h"       // main symbols

#include "opcspot_i.h"


// {63D5F432-CFE4-11d1-B2C8-0060083BA1FB}
static const GUID CATID_OPCDAServer20 = {0x63D5F432, 0xCFE4, 0x11d1, {0xB2, 0xC8, 0x00, 
0x60, 0x08, 0x3B, 0xA1, 0xFB}};
// DAServer

class ATL_NO_VTABLE DAServer :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<DAServer, &CLSID_DAServer>,
	public IDispatchImpl<IDAServer, &IID_IDAServer, &LIBID_opcspotLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	DAServer()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_DASERVER)

	
	BEGIN_COM_MAP(DAServer)
		COM_INTERFACE_ENTRY(IDAServer)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	// Register OPC DA 2.0 category, so
	// 'Implemented Categories'
	//	{
	//		{63D5F432-CFE4-11D1-B2C8-0060083BA1FB}
	//	}
	// in DAServer.rgs is redundant.
	BEGIN_CATEGORY_MAP(DAServer)
		IMPLEMENTED_CATEGORY(CATID_OPCDAServer20)
	END_CATEGORY_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

};

OBJECT_ENTRY_AUTO(__uuidof(DAServer), DAServer)
