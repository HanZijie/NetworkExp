#include "../include/Base.h"
#include "../include/Global.h"
#include "../include/SRReciever.h"

SRReciever::SRReciever( ):expectSequenceNumberRcvd(0),recv_base(0){
    recv_ack[0] = 0;
    recv_ack[1] = 0;
    recv_ack[2] = 0;
    recv_ack[3] = 0;
    recv_ack[4] = 0;
}

SRReciever::~SRReciever( ){
}

void SRReciever::receive(Packet& packet){
    int checkNum = pUtils->calculateCheckSum(packet);
    if(checkNum == packet.checksum){
        if(packet.seqnum <= recv_base){

        }else{
            recv_ack[packet.seqnum - recv_base -1] = 1;
            memcpy(msgBuf[packet.seqnum - recv_base -1].data,packet.payload,sizeof(packet.payload));
        }
        Packet ack_packet;
        ack_packet.acknum = packet.seqnum;
        ack_packet.checksum = pUtils->calculateCheckSum(ack_packet);
        printf("\n\nReceiver Send Ack\n");
        pns->sendToNetworkLayer(SENDER,ack_packet);

        int i = 0;
        while(i <= 3 && recv_ack[i] == 1){
            pns->delivertoAppLayer(RECEIVER,msgBuf[0]);

           msgBuf[0] = msgBuf[1];
           msgBuf[1] = msgBuf[2];
           msgBuf[2] = msgBuf[3];
           msgBuf[3] = msgBuf[4];

            recv_ack[0] = recv_ack[1];
            recv_ack[1] = recv_ack[2];
            recv_ack[2] = recv_ack[3];
            recv_ack[3] = recv_ack[4];

            recv_base++;
            i++;
        }
    }
}