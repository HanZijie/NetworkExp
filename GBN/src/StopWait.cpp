// StopWait.cpp : 定义控制台应用程序的入口点。
//
#include "../include/Base.h"
#include "../include/Global.h"
#include "../include/RdtSender.h"
#include "../include/RdtReceiver.h"
#include "../include/StopWaitRdtSender.h"
#include "../include/StopWaitRdtReceiver.h"
#include "../include/GBNSender.h"
#include "../include/GBNReciever.h"

int main(int argc, char** argv[])
{
	RdtSender *ps = new GBNSender();
	RdtReceiver * pr = new GBNReciever();
	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr);
	pns->setInputFile("/home/zijiehan/Desktop/NetworkExp/input.txt");
	pns->setOutputFile("/home/zijiehan/Desktop/NetworkExp/output.txt");
	pns->start();

	delete ps;
	delete pr;
	delete pUtils;									//指向唯一的工具类实例，只在main函数结束前delete
	delete pns;										//指向唯一的模拟网络环境类实例，只在main函数结束前delete
	
	return 0;
}

