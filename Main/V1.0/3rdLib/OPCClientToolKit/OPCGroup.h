/*
OPCClientToolKit
Copyright (C) 2005 Mark C. Beharrell

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
*/

#if !defined(AFX_OPCGROUP_H__BE6D983E_3D18_4952_A2B3_84A9FCDFC5CE__INCLUDED_)
#define AFX_OPCGROUP_H__BE6D983E_3D18_4952_A2B3_84A9FCDFC5CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OPCClient.h"


/**
* Forward decl.
*/
class COPCItem;

/**
* used internally to implement the asynch callback
*/
class CAsynchDataCallback; 




/**
* Client sided abstraction of an OPC group, wrapping the COM interfaces to the group within the OPC server.
*/
class COPCGroup  
{
private:
	ATL::CComPtr<IOPCGroupStateMgt>	iStateManagement;
	ATL::CComPtr<IOPCSyncIO>			iSychIO;
	ATL::CComPtr<IOPCAsyncIO2>		iAsych2IO;
	ATL::CComPtr<IOPCItemMgt>		iItemManagement;

	/**
	* Used to keep track of the connection point for the
	* AsynchDataCallback
	*/
	ATL::CComPtr<IConnectionPoint> iAsynchDataCallbackConnectionPoint;


	/**
	* handle given the group by the server
	*/
	DWORD groupHandle;

	/**
	* The server this group belongs to
	*/
	COPCServer &opcServer;


	/**
	* Callback for asynch data at the group level
	*/
	ATL::CComPtr<CAsynchDataCallback> asynchDataCallBackHandler;


	/**
	* list of OPC items associated with this goup. Not owned (at the moment!)
	*/
	CAtlArray<COPCItem *> items;


	/**
	* Name of the group
	*/
	const CString name;


	/**
	* Handle given to callback by server.
	*/
	DWORD callbackHandle;


	/**
	* Users hander to handle asynch data 
	* NOT OWNED.
	*/
	IAsynchDataCallback *userAsynchCBHandler;
	CAsynchDataCallback* _CAsynchDataCallback;

public:
	COPCGroup(const CString & groupName, bool active, unsigned long reqUpdateRate_ms, unsigned long &revisedUpdateRate_ms, float deadBand, COPCServer &server);

	virtual ~COPCGroup();


	COPCItem * addItem(CString &itemName, bool active);

	/**
	* returns the number of failed item creats
	* item[x] will be null if could not create
	*/
	int addItems(CAtlArray<CString>& itemName, CAtlArray<COPCItem *>& items, bool active);


	/**
	* enable Asynch IO
	*/
	void enableAsynch(IAsynchDataCallback &handler);


	/**
	* disable Asych IO 
	*/
	void disableAsynch();


	/**
	* set the group state values.
	*/
	void setState(DWORD reqUpdateRate_ms, DWORD &returnedUpdateRate_ms, float deadBand, BOOL active);



	/**
	* Read set of OPC items
	*/
	void readSync(int noItems, COPCItem **items, OPCItemData data[], HRESULT errors[], OPCDATASOURCE source);


	/**
	* Refresh is an asysnch operation.
	* retreives all active items in the group, which will be stored in the transaction object
	* Transaction object is owned by caller.
	* If group asynch is disabled then this call will not work
	*/ 
	CTransaction * refresh(OPCDATASOURCE source, ITransactionComplete *transactionCB = NULL);



	ATL::CComPtr<IOPCSyncIO> & getSychIOInterface(){
		return iSychIO;
	}


	ATL::CComPtr<IOPCAsyncIO2> & getAsych2IOInterface(){
		return iAsych2IO;
	}


	ATL::CComPtr<IOPCItemMgt> &getItemManagementInterface(){
		return iItemManagement;
	}

	const CString & getName() const {
		return name;
	}

	IAsynchDataCallback *getUsrAsynchHandler(){
		return userAsynchCBHandler;
	}

	/**
	* returns reaference to the OPC server that this group belongs to.
	*/
	COPCServer & getServer(){
		return opcServer;
	}
};

#endif // !defined(AFX_OPCGROUP_H__BE6D983E_3D18_4952_A2B3_84A9FCDFC5CE__INCLUDED_)
