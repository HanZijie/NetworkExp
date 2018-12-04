#pragma once
#include "RdtReceiver.h"
#include "GBNReciever.h"
class TCPReceiver :
	public GBNReciever
{
public:
	TCPReceiver();
	virtual ~TCPReceiver();
};