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
#include "OPCItem.h"
#include "OPCGroup.h"





COPCItem::COPCItem(CString &itemName, bool active, COPCGroup &g):
group(g),
name(itemName){
	USES_CONVERSION;
	WCHAR* wideName = T2OLE(itemName);

	OPCITEMDEF itemDef;
	itemDef.szItemID = wideName;
	itemDef.szAccessPath = NULL;//wideName;
	itemDef.bActive = active;
	itemDef.hClient = (DWORD)this;
	itemDef.dwBlobSize = 0;
	itemDef.pBlob = NULL;
	itemDef.vtRequestedDataType = VT_EMPTY;

	HRESULT *itemResult;
	OPCITEMRESULT *itemDetails;
	HRESULT	result = group.getItemManagementInterface()->AddItems(1, &itemDef, &itemDetails, &itemResult);
	if (FAILED(result)){
		throw OPCException("Failed to add item");
	}
	

	if(itemDetails[0].pBlob){ 
		COPCClient::comFree(itemDetails[0].pBlob);
	}

	if (FAILED(itemResult[0])){
		COPCClient::comFree(itemDetails);
		COPCClient::comFree(itemResult);
		throw OPCException("Failed to add item");
	}

	serversItemHandle	= itemDetails[0].hServer;
	vtCanonicalDataType = itemDetails[0].vtCanonicalDataType;
	dwAccessRights		= itemDetails[0].dwAccessRights;

	COPCClient::comFree(itemDetails);
	COPCClient::comFree(itemResult);
}





COPCItem::~COPCItem()
{
	HRESULT *itemResult;
	group.getItemManagementInterface()->RemoveItems(1, &serversItemHandle, &itemResult);
	COPCClient::comFree(itemResult);
}



void COPCItem::writeSync(VARIANT &data){
	HRESULT * itemWriteErrors; 
	HRESULT result = group.getSychIOInterface()->Write(1, &serversItemHandle, &data, &itemWriteErrors);
	if (FAILED(result))
	{
		throw OPCException("write failed");
	} 

	if (FAILED(itemWriteErrors[0])){
		COPCClient::comFree(itemWriteErrors);
		throw OPCException("write failed");
	}

	COPCClient::comFree(itemWriteErrors);
}



void COPCItem::readSync(OPCItemData &data, OPCDATASOURCE source){
	if ((dwAccessRights && OPC_READABLE) != OPC_READABLE){
		throw OPCException("Item is not readable");
	}

	HRESULT *itemResult;
	OPCITEMSTATE *itemState;

	HRESULT	result = group.getSychIOInterface()->Read(source, 1, &serversItemHandle, &itemState, &itemResult);
	if (FAILED(result))
	{
		throw OPCException("Read failed");
	} 

	if (FAILED(itemResult[0])){
		COPCClient::comFree(itemResult);
		COPCClient::comFree(itemState);
		throw OPCException("Read failed");
	}
		
	COPCClient::comFree(itemResult);
	
	data.set(itemState[0]);

	VariantClear(&itemState[0].vDataValue);
	COPCClient::comFree(itemState);
}



CTransaction * COPCItem::readAsynch(ITransactionComplete *transactionCB){
	DWORD cancelID;
	HRESULT * individualResults;
	CAtlArray<COPCItem *> items;
	items.Add(this);
	CTransaction * trans = new CTransaction(items,transactionCB);

	HRESULT result = group.getAsych2IOInterface()->Read(1, &serversItemHandle, (DWORD)trans, &cancelID, &individualResults);
	if (FAILED(result)){
		delete trans;
		throw OPCException("Asynch Read failed");
	}

	trans->setCancelId(cancelID);
	if (FAILED(individualResults[0])){
		trans->setItemError(this,individualResults[0]);
		trans->setCompleted(); // if all items return error then no callback will occur. p 101
	}

	COPCClient::comFree(individualResults);
	return trans;
}



CTransaction * COPCItem::writeAsynch(VARIANT &data, ITransactionComplete *transactionCB){
	DWORD cancelID;
	HRESULT * individualResults;
	CAtlArray<COPCItem *> items;
	items.Add(this);
	CTransaction * trans = new CTransaction(items,transactionCB);

	HRESULT result = group.getAsych2IOInterface()->Write(1,&serversItemHandle,&data,(DWORD)trans,&cancelID,&individualResults); 
	
	if (FAILED(result)){
		delete trans;
		throw OPCException("Asynch Write failed");
	}

	trans->setCancelId(cancelID);
	if (FAILED(individualResults[0])){
		trans->setItemError(this,individualResults[0]);
		trans->setCompleted(); // if all items return error then no callback will occur. p 104
	}

	COPCClient::comFree(individualResults);
	return trans;
}

void COPCItem::getSupportedProperties(CAtlArray<CPropertyDescription> &desc){
	DWORD noProperties = 0;
	DWORD *pPropertyIDs;
	LPWSTR *pDescriptions;
	VARTYPE *pvtDataTypes;

	USES_CONVERSION;
	HRESULT res = group.getServer().getPropertiesInterface()->QueryAvailableProperties(T2OLE(name), &noProperties, &pPropertyIDs, &pDescriptions, &pvtDataTypes);
	if (FAILED(res)){
		throw OPCException("Failed to restrieve properties", res);
	}

	for (unsigned i = 0; i < noProperties; i++){
		desc.Add(CPropertyDescription(pPropertyIDs[i], pDescriptions[i], pvtDataTypes[i]));
	}

	COPCClient::comFree(pPropertyIDs);
	COPCClient::comFree(pDescriptions);
	COPCClient::comFree(pvtDataTypes);
}


void COPCItem::getProperties(const CAtlArray<CPropertyDescription> &propsToRead, ATL::CAutoPtrArray<SPropertyValue> &propsRead){
	unsigned noProperties = propsToRead.GetCount();
	VARIANT *pValues = NULL;
	HRESULT *pErrors = NULL;
	DWORD *pPropertyIDs = new DWORD[noProperties];
	for (unsigned i = 0; i < noProperties; i++){
		pPropertyIDs[i] = propsToRead.GetAt(i).id;
	}
	propsRead.RemoveAll();
	propsRead.SetCount(noProperties);
	
	USES_CONVERSION;
	HRESULT res = group.getServer().getPropertiesInterface()->GetItemProperties(T2OLE(name), noProperties, pPropertyIDs, &pValues, &pErrors);
	delete []pPropertyIDs;
	if (FAILED(res)){
		throw OPCException("Failed to restrieve property values", res);
	}

	for (unsigned i = 0; i < noProperties; i++){
		CAutoPtr<SPropertyValue> v;
		if (!FAILED(pErrors[i])){
			v.Attach(new SPropertyValue(propsToRead[i], pValues[i]));
		}
		propsRead[i]=v;
	}

	COPCClient::comFree(pErrors);
	COPCClient::comFreeVariant(pValues, noProperties);
}