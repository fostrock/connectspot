//------------------------------------------------------------------------------------------
// File: <MyClassFactory.h>
// Purpose: declare <Self-defined class factory to create the opc server object.>
//
// @author <Yun Hua>
// @version 1.0 2010/01/05
// 
// Copyright (C) 2010, Yun Hua
//-----------------------------------------------------------------------------------------//

#include "stdafx.h"

#pragma once

class MyClassFactory :
	public CComClassFactory
{
public:
	MyClassFactory(void);
	~MyClassFactory(void); // The class can not be used as a base.

	STDMETHOD(CreateInstance)(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj);
};
