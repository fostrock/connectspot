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

#if !defined(AFX_OPCCLIENT_H__1C1AA002_F7C5_4537_B569_8352FBA27544__INCLUDED_)
#define AFX_OPCCLIENT_H__1C1AA002_F7C5_4537_B569_8352FBA27544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <atlbase.h>
#include <atlstr.h>
#include <atlexcept.h>
#include <atlcoll.h>
#include <objbase.h>
#include <COMCat.h>
#include <stdexcept>
#include "opcda.h"



class COPCHost;
class COPCServer;
class COPCGroup;
class COPCItem;


/**
* Basic OPC expection
*/
class OPCException:public ATL::CAtlException{
private:
	CString why;
public:
	OPCException(const CString& what,HRESULT code = 0):ATL::CAtlException(code),why(what){}

	const CString & reasonString() const{
		return why;
	}
};







/**
*Wrapper for OPC data. Hides COM memory management.
*/
struct OPCItemData{
	FILETIME ftTimeStamp;
	WORD wQuality;
	VARIANT vDataValue;
	HRESULT error;


	OPCItemData(HRESULT err):error(err){
		vDataValue.vt = VT_EMPTY;
	}



	OPCItemData(FILETIME time, WORD qual, VARIANT & val, HRESULT err){
		vDataValue.vt = VT_EMPTY;
		HRESULT result = VariantCopy( &vDataValue, &val);
		if (FAILED(result)){
			throw OPCException("VarCopy failed");
		}

		ftTimeStamp = time;
		wQuality = qual;
		error = err;
	}


	OPCItemData(){
		vDataValue.vt = VT_EMPTY;
	}



	~OPCItemData(){
		VariantClear(&vDataValue);
	}


	void set(OPCITEMSTATE &itemState){
		HRESULT result = VariantCopy( &vDataValue, &itemState.vDataValue);
		if (FAILED(result)){
			throw OPCException("VarCopy failed");
		}

		ftTimeStamp = itemState.ftTimeStamp;
		wQuality = itemState.wQuality;
	}


	void set(FILETIME time, WORD qual, VARIANT & val){
		HRESULT result = VariantCopy( &vDataValue, &val);
		if (FAILED(result)){
			throw OPCException("VarCopy failed");
		}

		ftTimeStamp = time;
		wQuality = qual;
	}
};



/**
* Can't find an ATL autoptr map - this class provides the functionality I want.
*/
class COPCItem_DataMap:public CAtlMap<COPCItem *, OPCItemData *>{
public:
	~COPCItem_DataMap(){
		POSITION pos = GetStartPosition();
		while (pos != NULL){
			OPCItemData * data = GetNextValue(pos);
			if (data){
				delete data;
			}
		}
		RemoveAll();
	}
};




class CTransaction;



/**
* Interface which provides means by which the client can be notified when an asynchronous operation
* is completed. The implementer must implement this interface overriding the complete method to provide
* the desired behaviour. An instance of the implementation can be passed as a parameter to an asynchronous 
* operation and used as a means of completion notification.
*/
class ITransactionComplete{
public:
	virtual void complete(CTransaction &transaction) = 0;
};





/**
* Used to indicate completion of an asynchronous operation. 
* Will contain the results of that operation.
*/
class CTransaction{

	/**
	* Optional transation complete callback - not owned
	*/
	ITransactionComplete * completeCallBack;

	// true when the transaction has completed
	bool completed;
	

	DWORD cancelID;


public:
	/**
	* keyed on OPCitem address (not owned)
	* OPCitem data is owned by the transaction - may be NULL
	*/
	COPCItem_DataMap opcData;


	CTransaction(ITransactionComplete * completeCB = NULL)
		:completed(FALSE), cancelID(0xffffffff), completeCallBack(completeCB){

		}



		CTransaction(CAtlArray<COPCItem *>&items, ITransactionComplete * completeCB)
			:completed(FALSE), cancelID(0xffffffff), completeCallBack(completeCB){
				for (unsigned i = 0; i < items.GetCount(); i++){
					opcData.SetAt(items[i],NULL);
				}
			}



			void setItemError(COPCItem *item, HRESULT error){
				CAtlMap<COPCItem *, OPCItemData *>::CPair* pair = opcData.Lookup(item);
				opcData.SetValueAt(pair,new OPCItemData(error));
			}


			void setItemValue(COPCItem *item, FILETIME time, WORD qual, VARIANT & val, HRESULT err){
				CAtlMap<COPCItem *, OPCItemData *>::CPair* pair = opcData.Lookup(item);
				opcData.SetValueAt(pair,new OPCItemData(time, qual, val, err));
			}


			const OPCItemData * getItemValue(COPCItem *item) const{
				const CAtlMap<COPCItem *, OPCItemData *>::CPair* pair = opcData.Lookup(item);
				if (!pair) return NULL; // abigious - we do'nt know if the key does not exist or there is no value - TODO throw exception

				return pair->m_value;
			}
			

			void setCompleted(){
				completed = TRUE;
				if (completeCallBack){
					completeCallBack->complete(*this);
				}
			}

			bool isCompeleted() const{
				return completed;
			}

			void setCancelId(DWORD id){
				cancelID = id;
			}

			DWORD getCancelId() const{
				return cancelID;
			}
};




/**
* Data received from the OnDataChange() method of the CAsynchDataCallback instance is delegated to an instance 
* of a child class implementing this interface. The Child class must obviously provide the desired behaviour
* in the overriden OnDataChange() method. This interface is active only when the corresponding group is active
* (achieved by the groups enableSynch() method.)
*/
class IAsynchDataCallback
{
public:
	virtual void OnDataChange(COPCGroup & group, CAtlMap<COPCItem *, OPCItemData *> & changes) = 0;
};












/**
* Starting point for 'everything'. Utility class that creates host objects and handles COM memory management. 
* Effectively a singleton.
*/
class COPCClient  
{
private:
	static ATL::CComPtr<IMalloc> iMalloc;

public:
	static void init();

	
	static void stop();

	static void comFree(void *memory);

	static void comFreeVariant(VARIANT *memory, unsigned size);

	/**
	* make a host machine abstraction.
	* @param hostname - may be empty (in which case a local host is created).
	* @ returns host object (owned by caller).
	*/
	static COPCHost * makeHost(const CString &hostName);


	static const GUID CATID_OPCDAv10;

	static const GUID CATID_OPCDAv20;
};





#endif // !defined(AFX_OPCCLIENT_H__1C1AA002_F7C5_4537_B569_8352FBA27544__INCLUDED_)
