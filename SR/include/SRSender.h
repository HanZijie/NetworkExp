#pragma once
#include "RdtSender.h"

class SRSender:public RdtSender{
private:
    int expectSequenceNumberSend;
    int waittingAckNumber;
    int ack[5];
    int base;
    Packet pckBuf[5];

public:
    SRSender();
    bool send(Message &message);
    bool getWaitingState();
	void receive(Packet &ackPkt);
	void timeoutHandler(int seqNum);
	virtual ~SRSender();
};