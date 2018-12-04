#include "../include/Base.h"
#include "../include/Global.h"
#include "../include/TCPSender.h"


TCPSender::TCPSender():GBNSender()
{
    flag = 0;
	file = fopen("slide_info.txt","w+");
}


TCPSender::~TCPSender()
{
	fclose(file);
}

void TCPSender::receive(Packet &ackPkt){
    int checkSum = pUtils->calculateCheckSum(ackPkt);
	if (checkSum == ackPkt.checksum) {
		printf("\n\n### 发送方收到确认%d\n",ackPkt.acknum);
		if (ackPkt.acknum >= base + 1)
		{
			pns->stopTimer(SENDER, ackPkt.acknum);
			int newbase = ackPkt.acknum;
			fprintf(file,"from %d slide to %d\n",base,newbase);
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
            flag = 0;
		}else if (ackPkt.acknum < base) {
			pns->stopTimer(SENDER, ackPkt.acknum);
		}
		else if (ackPkt.acknum == base)
		{
            pns->stopTimer(SENDER, ackPkt.acknum);
            flag++;
            if(flag == 3){
				printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>Quick Resend!>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			    m_goback();
            }
		}
	}
	return;
}

void TCPSender::m_goback(){
    flag = 0;
    goBack();
}
