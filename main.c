/***********************************
8-5��
��Ϊ��arduino���Ծ������������ɵĴ��ڵ�ԭ�����Է���arduino��ר��stm15w���������Ҵ��͵�git���棬




************************************/
#include <STC15F2K60S2.H>
#include "wifi.h"
#define uint unsigned int
#define uchar unsigned char
/*****************************************/
uchar ssid[]={"ih"};
uchar passwd[]={"yixing123456"};
uchar IPAddress[]={"192,168,1,104"};
uint Port=5000;
uchar RemoteIP[4]={0};
char GetCmd[]={"GET / HTTP/1.1\r\nHost: 192.168.1.104\r\nUser-Agent: abc\r\nConnection: close\r\n"};
char PostCmd[]={"POST / HTTP/1.1\r\nHost: 192.168.1.104\r\nUser-Agent: abc\r\nContent-type: text/plain\r\nContent-Length:10\r\n\r\nabcdefghij\r\nConnection: close\r\n"};


/*****************************************/
#define CWJAP 1
#define CWMODE 2
#define CWQAP 3
#define CIPSTART 4
#define CIPSEND 5
#define CIPMODE 6
#define CIPCLOSE 7


/**************************
�������ƣ�
�������ã�
����������
������ע��
***************************/
/**************************
��������:sendCmd
�������ã�����wifi����
����������cmd
������ע��
***************************/
int sendCmd(uint cmd)
{
	switch(cmd)
	{
		case 1:
		WifiSend("AT+CWJAP=\"ih\",\"yixing123456\"");
			break;
		case 2:
		WifiSend("AT+CWMODE=3");
		WifiSend("AT+RST");
			break;
		case 3:
		WifiSend("AT+CWQAP");
			break;
		case 4:
		WifiSend("AT+CIPSTART=\"TCP\",\"192.168.1.104\",5000");
			break;
		case 5:
		WifiSend("AT+CIPSEND");
			break;
		case 6:
		WifiSend("AT+CIPMODE=1");
			break;
		case 7:
		WifiSend("AT+CIPCLOSE");
			break;
		default :WifiSend("AT+RST"); 
	}
	return 0;
}
/**************************
��������:sendDate
�������ã�����wifi����
����������cmd
������ע��
***************************/
char sendDate(uint cmd)
{
	switch(cmd)
	{
		case 1:
		WifiSend(GetCmd);
			break;
		case 2:		
		WifiSend(PostCmd);
			break;
		default :WifiSend("AT+RST"); 
	}
	return '0';
}
/**************************
�������ƣ�delay()
�������ã���ʱ
������������ʱʱ�䣬��λ ��
������ע��
***************************/
void delay(uint time)
{
	uint y=0;
	while(time--)
	{
		y=250;
		while(y--);
	}
}

int main()
{
	int i=0,t;
	UartInit();
	while(1)
	{
		for(i=0;i<7;i++)
		{
			t=sendCmd(i);
			delay(10000);	
		}
		sendDate(1);
		delay(10000);			
		sendDate(2);
		delay(10000);	
	}
	return 0;
}


