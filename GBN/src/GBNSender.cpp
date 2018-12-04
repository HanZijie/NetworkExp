#include "../include/Base.h"
#include "../include/Global.h"
#include "../include/GBNSender.h"


GBNSender::GBNSender():expectSequenceNumberSend(0),waittingAckNumber(0),base(0)
{
	file = fopen("slide_info.txt","w+");
}


GBNSender::~GBNSender()
{
	fclose(file);
}

bool GBNSender::send(Message &message) {
	Packet* pckSend = (Packet *)malloc(sizeof(Packet));
	this->expectSequenceNumberSend++;

	pckSend->acknum = -1;
	pckSend->seqnum = this->expectSequenceNumberSend;
	pckSend->checksum = 0;
	memcpy(pckSend->payload, message.data, sizeof(message.data));
	pckSend->checksum = pUtils->calculateCheckSum(*pckSend);
	pUtils->printPacket("发送方发送报文", *pckSend);
	pckBuf[waittingAckNumber] = *pckSend;
	printf("\nsave:%d  and %d\n", (*pckSend).seqnum, pckBuf[waittingAckNumber].seqnum);
	pns->startTimer(SENDER, Configuration::TIME_OUT, pckSend->seqnum);
	pns->sendToNetworkLayer(RECEIVER, *pckSend);

	waittingAckNumber++;
	return true;
}

void GBNSender::receive(Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	if (checkSum == ackPkt.checksum) {
		printf("\n\n### 发送方收到确认%d\n",ackPkt.acknum);
		if (ackPkt.acknum >= base + 1)
		{
			pns->stopTimer(SENDER, ackPkt.acknum);
			int newbase = ackPkt.acknum;
			fprintf(file,"From %d slide to %d\n",base,newbase);
			int i = base;
			while (i < newbase)
			{
				printf("\n------replace:%d with %d\n", pckBuf[i-base].seqnum,pckBuf[i-base+1].seqnum);
				pckBuf[0] = pckBuf[1];
				pckBuf[1] = pckBuf[2];
				pckBuf[2] = pckBuf[3];
				pckBuf[3] = pckBuf[4];
				i++;
			}
			waittingAckNumber-=(newbase-base);
			base = newbase;
		}else if (ackPkt.acknum < base) {
			pns->stopTimer(SENDER, ackPkt.acknum);
		}
		else if (ackPkt.acknum == base)
		{
			pns->stopTimer(SENDER, ackPkt.acknum);
			goBack();
		}
	}
	return;
}

void GBNSender::timeoutHandler(int seqNum) {
	printf("\n发送超时 %d\n", seqNum);
	pns->stopTimer(SENDER, seqNum);
	if (seqNum == base + 1) {
		goBack();
	}
}

void GBNSender::goBack() {
	printf("\n重发\n\n");
	int i = 0;
	while (i < waittingAckNumber) {
		if (base + 1 <= pckBuf[i].seqnum) {
			pns->sendToNetworkLayer(RECEIVER, pckBuf[i]);
			pns->startTimer(SENDER, Configuration::TIME_OUT, pckBuf[i].seqnum);
		}
		i++;
	}
}

bool GBNSender::getWaitingState() {
	if (waittingAckNumber > 3) {
		return true;
	} 
	else {
		return false;
	}
}
