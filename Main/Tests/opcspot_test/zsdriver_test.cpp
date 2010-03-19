#include "stdafx.h"
#include <boost/test/unit_test.hpp>
#include "opcspot/ZSDriver.h"
#include <iostream>


//BOOST_AUTO_TEST_CASE(zsdriver_init_test) 
//{
//	BOOST_CHECK_EQUAL(true, ZSDriver::Init("d:\\zsdriver.xml"));
//
//	std::vector<ZSDriver::TAG_DEF> tagDef = ZSDriver::GetTagDef();
//	BOOST_CHECK_EQUAL(tagDef.size(), 38 * 4);
//	for (std::size_t i = 0; i < tagDef.size(); i++)
//	{
//		ZSDriver::AssignTagIDIndexMap(tagDef.at(i).dataID + 1, tagDef.at(i).dataID);
//	}
//
//	ZSDriver::Destroy();
//}

BOOST_AUTO_TEST_CASE(ZSDriver_refreshdata_test)
{
	loDriver driver;
	memset(&driver, 0, sizeof(loDriver));
	ZSDriver::Init("d:\\zsdriver.xml");
	driver.ldSubscribe = ZSDriver::activation_monitor;

	driver.ldFlags = loDF_IGNCASE | loDf_NOFORCE;

	driver.ldWriteTags = ZSDriver::WriteTags;
	driver.ldReadTags = ZSDriver::ReadTags;
	driver.ldConvertTags = ZSDriver::ConvertTags;
	driver.ldBranchSep = '/'; // Hierarchical branch separator

	loService* instance = NULL;
	int ecode = loServiceCreate(&instance, &driver, 4096 /* number of tags in the cache */);
	BOOST_CHECK_EQUAL(0, ecode);

	std::vector<ZSDriver::TAG_DEF> tagDef = ZSDriver::GetTagDef();
	loTagId tagID = 0;
	for (std::size_t i = 0; i < tagDef.size(); ++i)
	{
		CComVariant var;
		if (VT_R8 == tagDef.at(i).type)
		{
			var = 0.0;
		}
		else if (VT_UI4 == tagDef.at(i).type)
		{
			var = (unsigned)0;
		}
		loAddRealTag(instance, 
			&tagID, 
			(loRealTag)0, 
			tagDef.at(i).name.c_str(), 
			loTF_EMPTY, 
			tagDef.at(i).right, 
			&var, 
			0, 
			NULL);
		ZSDriver::AssignTagIDIndexMap(tagID, tagDef.at(i).dataID);
	}

	ZSDriver::RefreshData(instance);

	Sleep(15000);

	ZSDriver::Destroy();

	ecode = loServiceDestroy(instance);
	BOOST_CHECK_EQUAL(0, ecode);
}