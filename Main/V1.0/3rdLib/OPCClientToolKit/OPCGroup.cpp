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

#include "OPCServer.h"
#include "OPCGroup.h"
#include "OPCItem.h"





/**
* Handles OPC (DCOM) callbacks at the group level. It deals with the receipt of data from asynchronous operations.
* This is a fake COM object.
*/
class CAsynchDataCallback : public IOPCDataCallback
{
private:
	DWORD mRefCount;

	/**
	* group this is a callback for
	*/
	COPCGroup &callbacksGroup;

	/**
	* Enter the OPC items data that resulted from an operation
	*/
	static void updateOPCData(COPCItem_DataMap &opcData, DWORD count, OPCHANDLE * clienthandles, 
		VARIANT* values, WORD * quality,FILETIME * time, HRESULT * errors){
		// see page 136 - returned arrays may be out of order
		for (unsigned i = 0; i < count; i++){
			// TODO this is bad  - server could corrupt address - need to use look up table
			COPCItem * item = (COPCItem *)clienthandles[i];
			CAtlMap<COPCItem *, OPCItemData *>::CPair* pair = opcData.Lookup(item);
			OPCItemData * data = NULL;
			if (FAILED(errors[i])){
				data = new OPCItemData(errors[i]);
			} else {	
				data = new OPCItemData(time[i],quality[i],values[i],errors[i]);
				
			}
			if (pair == NULL){
				opcData.SetAt(item,data);
			} else {
				opcData.SetValueAt(pair,data);
			}
		}
		
	}

public:
	CAsynchDataCallback(COPCGroup &group):callbacksGroup(group){
		mRefCount = 0;
	}


	~CAsynchDataCallback(){

	}

	/**
	* Functions associated with IUNKNOWN
	*/
	STDMETHODIMP QueryInterface( REFIID iid, LPVOID* ppInterface){
		if ( ppInterface == NULL){
			return E_INVALIDARG;
		}

		if ( iid == IID_IUnknown ){
			*ppInterface = (IUnknown*) this;
		} else if ( iid == IID_IOPCDataCallback){
			*ppInterface = (IOPCDataCallback*) this;
		} else
		{
			*ppInterface = NULL;
			return E_NOINTERFACE;
		}


		AddRef();
		return S_OK;
	}


	STDMETHODIMP_(ULONG) AddRef(){
		return ++mRefCount;
	}


	STDMETHODIMP_(ULONG) Release(){
		--mRefCount; 

		if ( mRefCount == 0){
			delete this;
		}
		return mRefCount;
	}

	/**
	* Functions associated with IOPCDataCallback
	*/
	STDMETHODIMP OnDataChange(DWORD Transid, OPCHANDLE grphandle, HRESULT masterquality,
		HRESULT mastererror, DWORD count, OPCHANDLE * clienthandles, 
		VARIANT * values, WORD * quality, FILETIME  * time,
		HRESULT * errors)
	{
		IAsynchDataCallback * usrHandler = callbacksGroup.getUsrAsynchHandler();

		if (Transid != 0){
			// it is a result of a refresh (see p106 of spec)
			CTransaction & trans = *(CTransaction *)Transid; 
			updateOPCData(trans.opcData, count, clienthandles, values,quality,time,errors);
			trans.setCompleted();	
			return S_OK;	
		}

		if (usrHandler){
			COPCItem_DataMap dataChanges;
			updateOPCData(dataChanges, count, clienthandles, values,quality,time,errors);
			usrHandler->OnDataChange(callbacksGroup, dataChanges);
		}
		return S_OK;
	}


	STDMETHODIMP OnReadComplete(DWORD Transid, OPCHANDLE grphandle, 
		HRESULT masterquality, HRESULT mastererror, DWORD count, 
		OPCHANDLE * clienthandles, VARIANT* values, WORD * quality,
		FILETIME * time, HRESULT * errors)
	{
		// TODO this is bad  - server could corrupt address - need to use look up table
		CTransaction & trans = *(CTransaction *)Transid; 
		updateOPCData(trans.opcData, count, clienthandles, values,quality,time,errors);
		trans.setCompleted();
		return S_OK;
	}


	STDMETHODIMP OnWriteComplete(DWORD Transid, OPCHANDLE grphandle, HRESULT mastererr, 
		DWORD count, OPCHANDLE * clienthandles, HRESULT * errors)
	{
		// TODO this is bad  - server could corrupt address - need to use look up table
		CTransaction & trans = *(CTransaction *)Transid; 

		// see page 145 - number of items returned may be less than sent
		for (unsigned i = 0; i < count; i++){
			// TODO this is bad  - server could corrupt address - need to use look up table
			COPCItem * item = (COPCItem *)clienthandles[i];
			trans.setItemError(item, errors[i]); // this records error state - may be good
		}

		trans.setCompleted();
		return S_OK;
	}



	STDMETHODIMP OnCancelComplete(DWORD transid, OPCHANDLE grphandle){
		printf("OnCancelComplete: Transid=%ld GrpHandle=%ld\n", transid, grphandle);
		return S_OK;
	}
};








COPCGroup::COPCGroup(const CString & groupName, bool active, unsigned long reqUpdateRate_ms, unsigned long &revisedUpdateRate_ms, float deadBand, COPCServer &server):
name(groupName),
opcServer(server)
{
	USES_CONVERSION;
	WCHAR* wideName = T2OLE(groupName);


	HRESULT result = opcServer.getServerInterface()->AddGroup(wideName, active, reqUpdateRate_ms, 0, 0, &deadBand,
		0, &groupHandle, &revisedUpdateRate_ms, IID_IOPCGroupStateMgt, (LPUNKNOWN*)&iStateManagement);
	if (FAILED(result))
	{
		throw OPCException("Failed to Add group");
	} 

	result = iStateManagement->QueryInterface(IID_IOPCSyncIO, (void**)&iSychIO);
	if (FAILED(result)){
		throw OPCException("Failed to get IID_IOPCSyncIO");
	}

	result = iStateManagement->QueryInterface(IID_IOPCAsyncIO2, (void**)&iAsych2IO);
	if (FAILED(result)){
		throw OPCException("Failed to get IID_IOPCAsyncIO2");
	}

	result = iStateManagement->QueryInterface(IID_IOPCItemMgt, (void**)&iItemManagement);
	if (FAILED(result)){
		throw OPCException("Failed to get IID_IOPCItemMgt");
	}
}








COPCGroup::~COPCGroup()
{
	opcServer.getServerInterface()->RemoveGroup(groupHandle, FALSE);
}





void COPCGroup::readSync(int noItems, COPCItem **items, OPCItemData data[], HRESULT errors[], OPCDATASOURCE source){

	OPCHANDLE *handles = new OPCHANDLE[noItems];

	for (int i = 0; i < noItems; i++){
		if ((items[i]->getAccessRights() && OPC_READABLE) != OPC_READABLE){
			throw OPCException("Item is not readable");
		}
		handles[i] = items[i]->getHandle();
	}

	HRESULT *itemResult;
	OPCITEMSTATE *itemState;

	HRESULT	result = iSychIO->Read(source, noItems, handles, &itemState, &itemResult);
	if (FAILED(result))
	{
		throw OPCException("Read failed");
	} 


	for (i = 0; i < noItems; i++){
		errors[i] = itemResult[i];
		if (!FAILED(itemResult[i])){
			data[i].set(itemState[i]);
			VariantClear(&itemState[i].vDataValue);
		}
	}

	COPCClient::comFree(itemResult);
	COPCClient::comFree(itemState);
}



CTransaction * COPCGroup::refresh(OPCDATASOURCE source, ITransactionComplete *transactionCB){
	DWORD cancelID;
	CTransaction * trans = new CTransaction(items, transactionCB);

	HRESULT result = iAsych2IO->Refresh2(source, (DWORD)trans, &cancelID);
	if (FAILED(result)){
		delete trans;
		throw OPCException("refresh failed");
	}

	return trans;
}



COPCItem * COPCGroup::addItem(CString &itemName, bool active)
{
	COPCItem * item = new COPCItem(itemName, active, *this);
	items.Add(item);
	return item;
}




int COPCGroup::addItems(CAtlArray<CString>& itemName, CAtlArray<COPCItem *>& itemsCreated, bool active){
	// TODO - this does not work at the moment
 	OPCITEMDEF *itemDef = new OPCITEMDEF[itemName.GetCount()];
	for (unsigned i = 0; i < itemName.GetCount(); i++){
		USES_CONVERSION;
		WCHAR* wideName = T2OLE(itemName[i]);
		itemDef[i].szItemID = wideName;
		itemDef[i].szAccessPath = wideName;
		itemDef[i].bActive = active;
		itemDef[i].hClient = (DWORD)this;
		itemDef[i].dwBlobSize = 0;
		itemDef[i].pBlob = NULL;
		itemDef[i].vtRequestedDataType = VT_EMPTY;
	}


	HRESULT *itemResult;
	OPCITEMRESULT *itemDetails;
	HRESULT	result = getItemManagementInterface()->AddItems(itemName.GetCount(), itemDef, &itemDetails, &itemResult);
	if (FAILED(result)){
		throw OPCException("Failed to add items");
	}



	int errorCount = 0;
	for (i = 0; i < itemName.GetCount(); i++){
		if(itemDetails[i].pBlob){ 
			COPCClient::comFree(itemDetails[0].pBlob);
		}

		if (FAILED(itemResult[i])){
			errorCount++;
		}

		OPCHANDLE serversItemHandle	= itemDetails[i].hServer;
		VARTYPE vtCanonicalDataType = itemDetails[i].vtCanonicalDataType;
		DWORD dwAccessRights		= itemDetails[i].dwAccessRights;
	}


	COPCClient::comFree(itemDetails);
	COPCClient::comFree(itemResult);

	return errorCount;
}




void COPCGroup::enableAsynch(IAsynchDataCallback &handler){
	if (!asynchDataCallBackHandler == false){
		throw OPCException("Asynch already enabled");
	}


	ATL::CComPtr<IConnectionPointContainer> iConnectionPointContainer = 0;
	HRESULT result = iStateManagement->QueryInterface(IID_IConnectionPointContainer, (void**)&iConnectionPointContainer);
	if (FAILED(result))
	{
		throw OPCException("Could not get IID_IConnectionPointContainer");
	}


	result = iConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback, &iAsynchDataCallbackConnectionPoint);
	if (FAILED(result))
	{
		throw OPCException("Could not get IID_IOPCDataCallback");
	}


	asynchDataCallBackHandler = new CAsynchDataCallback(*this);
	result = iAsynchDataCallbackConnectionPoint->Advise(asynchDataCallBackHandler, &callbackHandle);
	if (FAILED(result))
	{
		iAsynchDataCallbackConnectionPoint = NULL;
		asynchDataCallBackHandler = NULL;
		throw OPCException("Failed to set DataCallbackConnectionPoint");
	}

	userAsynchCBHandler = &handler;
}




void COPCGroup::setState(DWORD reqUpdateRate_ms, DWORD &returnedUpdateRate_ms, float deadBand, BOOL active){
	HRESULT result = iStateManagement->SetState(&reqUpdateRate_ms, &returnedUpdateRate_ms, &active,0, &deadBand,0,0);
	if (FAILED(result))
	{
		throw OPCException("Failed to set group state");
	}
}




void COPCGroup::disableAsynch(){
	if (asynchDataCallBackHandler == NULL){
		throw OPCException("Asynch is not exabled");
	}
	iAsynchDataCallbackConnectionPoint->Unadvise(callbackHandle);
	iAsynchDataCallbackConnectionPoint = NULL;
	asynchDataCallBackHandler = NULL;// WE DO NOT DELETE callbackHandler, let the COM ref counting take care of that
	userAsynchCBHandler = NULL;
}