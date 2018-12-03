#include "../include/Base.h"
#include "../include/Global.h"
#include "../include/SRSender.h"

SRSender::SRSender():expectSequenceNumberSend(0),waittingAckNumber(0),base(0){
    ack[0] = 0;
    ack[1] = 0;
    ack[2] = 0;
    ack[3] = 0;
    ack[4] = 0;

}

SRSender::~SRSender(){
}

bool SRSender::send(Message &message){
    Packet* pckSend = (Packet *)malloc(sizeof(Packet)); 
	this->expectSequenceNumberSend++;

    //make a packet
	pckSend->acknum = -1;
	pckSend->seqnum = this->expectSequenceNumberSend;
	pckSend->checksum = 0;
	memcpy(pckSend->payload, message.data, sizeof(message.data));
	pckSend->checksum = pUtils->calculateCheckSum(*pckSend);

    // send packet and cache
	pUtils->printPacket("发送方发送报文", *pckSend);
	pckBuf[waittingAckNumber] = *pckSend;
	pns->startTimer(SENDER, Configuration::TIME_OUT, pckSend->seqnum);
	pns->sendToNetworkLayer(RECEIVER, *pckSend);

	waittingAckNumber++;
	return true;
}

void SRSender::receive(Packet &ackPkt){
    int checknum = pUtils->calculateCheckSum(ackPkt);
    if(checknum == ackPkt.checksum){
        pns->stopTimer(SENDER,ackPkt.acknum);
        ack[ackPkt.acknum-base-1] = 1;
        int i = 0;
        while(i <= 3 && ack[i] == 1){

            pckBuf[0] = pckBuf[1];
            pckBuf[1] = pckBuf[2];
            pckBuf[2] = pckBuf[3];
            pckBuf[3] = pckBuf[4];

            ack[0] = ack[1];
            ack[1] = ack[2];
            ack[2] = ack[3];
            ack[3] = ack[4];

            waittingAckNumber--;
            base++;
            i++;
        }

    }
}

void SRSender::timeoutHandler(int seqNum){
    pns->stopTimer(SENDER , seqNum);
    printf("\n\nResend Packet: %d",seqNum);
    pns->sendToNetworkLayer(RECEIVER,pckBuf[seqNum-base-1]);
    pns->startTimer(SENDER,Configuration::TIME_OUT,seqNum);
}

bool SRSender::getWaitingState(){
    if(waittingAckNumber > 3){
        return true;
    }else{
        return false;
    }
}