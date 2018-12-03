#pragma once
#include "RdtReceiver.h"

class GBNReciever:public RdtReceiver 
{
private:
	int expectSequenceNumberRcvd;
	int bufNum = 0;
	Message msgBuf;
	Packet lastAckPkt;
public:
	GBNReciever();
	void receive(Packet &packet);
	virtual ~GBNReciever();
};

