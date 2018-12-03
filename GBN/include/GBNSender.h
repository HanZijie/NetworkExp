#pragma once
#include "RdtSender.h"
class GBNSender :
	public RdtSender
{
private:
	int expectSequenceNumberSend;
	int waittingAckNumber;
	int base;
	bool backing = false;
	int ackFlag[10];
	Packet pckBuf[5];

public:
	GBNSender();
	bool send(Message &message);
	bool getWaitingState();
	void receive(Packet &ackPkt);
	void timeoutHandler(int seqNum);
	void goBack();
	virtual ~GBNSender();
};

