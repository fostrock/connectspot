#include "StdAfx.h"
#include "MyClassFactory.h"

MyClassFactory::MyClassFactory(void)
{
}

MyClassFactory::~MyClassFactory(void)
{
}

HRESULT MyClassFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj)
{
	ATLASSERT(ppvObj);
	return S_OK;
}
