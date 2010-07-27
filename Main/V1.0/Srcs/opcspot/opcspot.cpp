// opcspot.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "opcspot_i.h"
#include "mdump.h"

static MiniDumper globalDumper(_T("opcspot.DAServer"));

class CopcspotModule : public CAtlExeModuleT< CopcspotModule >
{
public :
	DECLARE_LIBID(LIBID_opcspotLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_OPCSPOT, "{6133F149-AD09-4FAE-A8B4-8A87503E2CFB}")
};

CopcspotModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

