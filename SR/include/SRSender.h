#pragma once
#include "RdtSender.h"
#include <stdio.h>

class SRSender:public RdtSender{
private:
    int expectSequenceNumberSend;
    int waittingAckNumber;
    int ack[5];
    int base;
    Packet pckBuf[5];
    FILE* file;

public:
    SRSender();
    bool send(Message &message);
    bool getWaitingState();
	void receive(Packet &ackPkt);
	void timeoutHandler(int seqNum);
	virtual ~SRSender();
};