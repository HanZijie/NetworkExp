#include "../include/Base.h"
#include "../include/Global.h"
#include "../include/GBNReciever.h"


GBNReciever::GBNReciever():expectSequenceNumberRcvd(0)
{
	lastAckPkt.acknum = 0;
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
	pns->sendToNetworkLayer(SENDER, lastAckPkt);
}


GBNReciever::~GBNReciever()
{
}

void GBNReciever::receive(Packet& packet){
	int checkSum = pUtils->calculateCheckSum(packet);
	if (checkSum == packet.checksum) {
		if (packet.seqnum == expectSequenceNumberRcvd + 1) {
			expectSequenceNumberRcvd++;
			memcpy(msgBuf.data, packet.payload, sizeof(packet.payload));
			printf("\n\n!!!reciever: 接收到 seqNum: %d 且正确,checkSum = %d!!!\n\n", packet.seqnum,checkSum);
			lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("接收方发送确认报文", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);

			//递交
			pns->delivertoAppLayer(RECEIVER, msgBuf);
		}
		else if(packet.seqnum > expectSequenceNumberRcvd+1) {
			expectSequenceNumberRcvd = lastAckPkt.acknum;
			pns->sendToNetworkLayer(SENDER, lastAckPkt);
			printf("\n\nRECIEVER: 有包丢失!!!!!!!!!!,确认：%d",lastAckPkt.acknum);
		}
		else if(packet.seqnum <= expectSequenceNumberRcvd ){
			pns->sendToNetworkLayer(SENDER, lastAckPkt);
		}
	}
	else {
		printf("收到损坏的包!!!!!!!!!!\n");
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
	}
}
