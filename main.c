/***********************************
8-5��
��Ϊ��arduino���Ծ������������ɵĴ��ڵ�ԭ�����Է���arduino��ר��stm15w���������Ҵ��͵�git���棬




************************************/
#include "main.h"
/*****************************************/

/*****************************************/
uchar ssid[]={"ih"};
uchar passwd[]={"yixing123456"};
uchar IPAddress[]={"192,168,1,100"};
uint Port=5000;
uchar RemoteIP[4]={0};
char GetCmd[]={"GET / HTTP/1.1\r\nHost: 192.168.1.100\r\nUser-Agent: abc\r\nConnection: close\r\n"};
char PostCmd[]={"POST / HTTP/1.1\r\nHost: 192.168.1.100\r\nUser-Agent: abc\r\nContent-type: text/plain\r\nContent-Length:10\r\n\r\nabcdefghij\r\nConnection: close\r\n"};
char buffer[1024];


sbit RST = P2^6;

unsigned char xdata Rec_Buf[Buf_Max];
unsigned char i = 0;   
void CLR_Buf(void);				   
bit  Hand(unsigned char *a); 	



sbit LED1 = P5^0;		    // ����LED1ΪP5.1 
sbit LED2 = P5^1;				// ����LED2ΪP5.2
sbit LED3 = P5^2;				// ����LED3ΪP5.3

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
void sendCmd(uint cmd)
{
	switch(cmd)
	{
		case 1:
		WifiSend("AT+CWJAP=\"ih\",\"yixing123456\"");
			break;
		case 2:
		WifiSend("AT+CWMODE=3");
		delay(100);
	//	WifiSend("AT+RST");
			break;
		case 3:
		WifiSend("AT+CWQAP");
			break;
		case 4:
		WifiSend("AT+CIPSTART=\"TCP\",\"192.168.1.100\",5000");
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
}
/**************************
��������:sendDate
�������ã�����wifi����
����������cmd
������ע��
***************************/
void sendDate(uint cmd)
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
/**************************
�������ƣ�Init
�������ã���ʼ��һЩ����
����������
������ע��
***************************/
void Init()
{		
	RST = 1;                                      // ESP8266��λ���ܽţ����ͻὫESP8266��λ
	UartInit();								      // ��ʼ������
	ES = 1;                                       // ����1�жϴ�
	IE2 = 0x01;                                   // ����2�жϴ�
	EA = 1;                                       // ���жϴ�
	DelayMS(1000);							                  // ��ʱһ��ʱ�䣬��ESP8266����

  	U1SendString(Rec_Buf);                        // ��ESP8266������Ϣͨ������1��ӡ��    	
  	U1SendString("Welcome to LSE STUDIO,\r\n");	
	CLR_Buf();                                 		//�����������	
	while(!Hand("OK"))                            //�ж��Ƿ����ֳɹ�,������ɹ���ʱһ��,�ٷ���AT����ָ��
	{	
		CLR_Buf();	
		WifiSendString("AT");                         //��������ָ�� 
		DelayMS(500);
		U1SendString(Rec_Buf);
	}
	CLR_Buf();                                    //�����������
  	U1SendString("OK,Succeed Establish connection with ESP8266\r\n");		
	LED1 = 0;

	
	while(!(Hand("OK")))        //�ж��Ƿ����óɹ����粻�ɹ�����ʱ���ٴη���
	{	
		CLR_Buf();	
		sendCmd(CWMODE);                         //��������ESP8266����ģʽָ��	
		DelayMS(500);
		U1SendString(Rec_Buf);
		if(Hand("no change"))		
		{
			U1SendString(Rec_Buf);
			break;
		}		
	}
	CLR_Buf();                 
  	U1SendString("OK,ESP8266 has been set as Station Mode\r\n");	

	
	while(!Hand("OK"))    										 		//�ж��Ƿ�����WiFi·�������粻�ɹ�����ʱ���ٴη���
	{	
		CLR_Buf();	
		sendCmd(CWJAP); 	
		DelayMS(2000);
		if(Hand("no change"))		//���IP��ַ�Ѿ�����
		{
			U1SendString(Rec_Buf);
			break;
		}
		U1SendString(Rec_Buf);		
	}
	LED2 = 0;
	CLR_Buf();              
  	U1SendString("OK,Succeed establish connection with WiFi AP\r\n");			
	while(!Hand("Linked"))                        //�ж��Ƿ�����TCP sever���粻�ɹ�����ʱ���ٴη���
	{
		CLR_Buf();
		sendCmd(CIPSTART);  
		DelayMS(3000);
		U1SendString(Rec_Buf);
	}
	CLR_Buf();                
  	U1SendString("OK,Succeed establish connection with TCP sever\r\n");			
	LED1 = 1;
	LED2 = 1;	
	sendCmd(CIPMODE); //���ݷ���ָ�� 
	DelayMS(1000);	
	while(!Hand("Unlink"))     									//�ж��Ƿ������ݳɹ����粻�ɹ�����ʱ���ٴη���
	{
		CLR_Buf();		
		sendCmd(CIPSEND); //��������	
		DelayMS(500);
		sendDate(1); //��������	
		DelayMS(500);
		U1SendString(Rec_Buf);
	}
	CLR_Buf();                
  	U1SendString("Congratulations, You can send commands through TCP sever now\r\n");							
}
/**************************
�������ƣ�
�������ã�
����������
������ע��
***************************/
bit setWifi(int cmd,unsigned char *str)
{
	while(!Hand("OK"))
	{
		CLR_Buf();	
		sendCmd(cmd);
		DelayMS(2000);
		U1SendString(Rec_Buf);
		if(Hand("no change")|Hand("Linked"))
		{
			break;			
		}
	}			
}



int main()
{
	int i=0;
	Init();
	init_port();
	U1SendString("tooo\n");
	while(1)
	{
		U1SendString("wangdaye\r\n");	
		delay(1000);
	}
	return 0;
}

 /**************************
�������ƣ�Hand
�������ã��ж��Ƿ����гɹ�
��������������Ҫ�жϵ��ַ���
������ע��
***************************/
bit Hand(unsigned char *a)
{ 
    if(strstr(Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}
/**************************
�������ƣ�CLR_Buf
�������ã���ս��ջ���
����������
������ע��
***************************/
void CLR_Buf(void)
{
	unsigned char k;
    for(k=0;k<Buf_Max;k++)    
    {
		Rec_Buf[k] = 0;
	}
    i = 0;                    
}

void Uart1() interrupt 4 using 1
{
	ES = 0;
	if (RI)
    {
      RI = 0;                 //���RIλ
	  Rec_Buf[i] = SBUF; 
	  i++;               
	  if(i>Buf_Max)          
	  {
		i = 0;
	  } 
    }
    if (TI)
    {
        TI = 0;                 //���TIλ

    }
	ES =  1;
}



void Uart2() interrupt 8
{
	IE2 = 0x00;
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;         
		Rec_Buf[i] = S2BUF; 
		i++;               
		if(i>Buf_Max)          
		{
			i = 0;
		}     
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;            
    }
	IE2 = 0x01;
}






/**************************
�������ƣ�init_port
�������ã���ʼ��һЩ�������ݣ���ÿ��IO�ڶ���ͨ��IO
����������
������ע��
***************************/
void init_port()
{
P0M0 = 0x00;
P0M1 = 0x00;
P1M0 = 0x00;
P1M1 = 0x00;
P2M0 = 0x00;
P2M1 = 0x00;
P3M0 = 0x00;
P3M1 = 0x00;
P4M0 = 0x00;
P4M1 = 0x00;
P5M0 = 0x00;
P5M1 = 0x00;
P6M0 = 0x00;
P6M1 = 0x00;
P7M0 = 0x00;
P7M1 = 0x00;
}
