#pragma once
#include "RdtSender.h"
#include "GBNSender.h"
#include <stdio.h>

class TCPSender :
	public GBNSender
{
public:
	using GBNSender::expectSequenceNumberSend;
	using GBNSender::base;
	using GBNSender::waittingAckNumber;
	using GBNSender::backing;
	using GBNSender::pckBuf;
	int flag;
	FILE *file;

public:
	TCPSender();
	void receive(Packet &ackPkt);
	void m_goback();
	virtual ~TCPSender();
	
};