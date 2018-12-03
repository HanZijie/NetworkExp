#pragma once
#include "RdtReceiver.h"

class SRReciever:public RdtReceiver 
{
private:
	int expectSequenceNumberRcvd;
	int recv_base ;
	int recv_ack[5];
	Message msgBuf[5];

public:
	SRReciever();
	void receive(Packet &packet);
	virtual ~SRReciever();
};