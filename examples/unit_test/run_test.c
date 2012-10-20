/*
 ============================================================================
 Name        : run_test.c
 Author      : Jiang Wen Long
 Version     : 1.0.0.0
 Copyright   : 2012 Shanghai Qiniu Information Technologies Co., Ltd.
 Description : QBOX TEST
 ============================================================================
 */

#include "qbox_test.h"
#include "../../qbox/rs.h"

int main()
{
	QBox_Error err;
	QBox_Client client;

	printf("如果运行出错且错误码为401，请打开run_test.c。\n");
	printf("将QBOX_ACCESS_KEY和QBOX_SECRET_KEY替换成您的帐号对应的key信息。\n");

	//QBOX_ACCESS_KEY = "<Please apply your access key>";
	//QBOX_SECRET_KEY = "<Dont send your secret key to anyone>";
	QBOX_ACCESS_KEY = "iN7NgwM31j4-BZacMjPrOQBs34UG1maYCAQmhdCV";
	QBOX_SECRET_KEY = "6QTOr2Jg1gcZEWDQXKOGZh5PziC2MCV5KsntT70j";
	//QBOX_ACCESS_KEY = "qeCgeE4u5DGXr592MVxk-bTvdmw8mMOKc_Bz2WpT";
	//QBOX_SECRET_KEY = "xN_lbSbrrCRhbsuhlN-3KWq_Tefkiv0Ao5s-Uxyg";
	//QBOX_RS_HOST = "http://127.0.0.1:9400";
	//QBOX_IO_HOST = "http://127.0.0.1:9200";
	//QBOX_FS_HOST = "http://127.0.0.1:9300";
	//QBOX_UP_HOST = "http://127.0.0.1:11200";

	QBox_Zero(client);
	QBox_Global_Init(-1);

	QBox_Client_Init(&client, 1024);

	QBT_Do(&client);

	QBox_Client_Cleanup(&client);
	QBox_Global_Cleanup();

	return 0;
}


