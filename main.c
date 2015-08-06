/***********************************
8-5��
��Ϊ��arduino���Ծ������������ɵĴ��ڵ�ԭ�����Է���arduino��ר��stm15w���������Ҵ��͵�git���棬




************************************/
#include "main.h"
/*****************************************/

/*****************************************/
uchar ssid[]={"HR"};
uchar passwd[]={"1234567890"};
uchar IPAddress[]={"192,168,1,100"};
uint Port=5000;
uchar RemoteIP[4]={0};
char GetCmd[]={"GET /test HTTP/1.1\r\nHost: 192.168.1.100\r\nUser-Agent: abc\r\n\r\n"};//Connection: close
char PostCmd[]={"POST / HTTP/1.1\r\nHost: 192.168.1.100\r\nUser-Agent: abc\r\nContent-type: text/plain\r\nContent-Length:10\r\n\r\nabcdefghij\r\n"};
char buffer[1024];
char datelen=0;

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
void sendDate(uint cmd,uchar *str)
{
	str = str;
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
	DelayMS(3000);							                  // ��ʱһ��ʱ�䣬��ESP8266����

  	U1SendString(Rec_Buf);                        // ��ESP8266������Ϣͨ������1��ӡ��    	
  	U1SendString("Welcome to LSE STUDIO,\r\n");	
	CLR_Buf();                                 		//�����������	              
/*****************************/	
	setWifi(CWMODE,"OK");  	
	while(!setWifi(CWJAP,"OK"))
	{
		delay(10000);
		U1SendString("the wifi connecting\r\n");
	}
	LED1 = 0;			//������һ����,��ʾwifi��������
	setWifi(CIPMODE,"OK");
	while(!setWifi(CIPSTART,"Linked"))
	{
		 U1SendString("�������һ�����⣬���Ӳ�����\r\n");
		 delay(10000);			//��ʱ���Ӳ��ϣ����������� 
	}
	LED2 = 0;
/****************************/
//	setWifi(CIPSTART,"Linked");
//	DelayMS(1000);	
//	while(!Hand("Unlink"))     									//�ж��Ƿ������ݳɹ����粻�ɹ�����ʱ���ٴη���
//	{
//		CLR_Buf();		
//		sendCmd(CIPSEND); //��������	
//		DelayMS(500);
//		sendDate(1,""); //��������	
//		DelayMS(500);
//		U1SendString(Rec_Buf);
//	}
	CLR_Buf();                
  	U1SendString("Congratulations, You can send commands through TCP sever now\r\n");							
}
/**************************
�������ƣ�setWifi 
�������ã�����wifi��һЩָ��
����������cmd���͵Ĳ��� str��Ҫƥ�������
������ע��
***************************/
bit setWifi(int cmd,unsigned char *str)
{
	int i=0;
	while(!Hand(str))
	{
		CLR_Buf();	
		sendCmd(cmd);
		DelayMS(5000);
		U1SendString(Rec_Buf);
		if(Hand("no change")|Hand("CONNECT")) 		//�����no change ������Linked��ֱ�ӷ���
		{
			break;			
		}
		if(++i == 5)
		{
			goto out;
		}
	}			
	CLR_Buf();
	return true;
out:
	CLR_Buf();
	return false;	
}
/**************************
�������ƣ�sendHTTP
�������ã�����http����
�����������Ƿ��ͻ��ǽ��գ����͵�������ʲô��
������ע��
***************************/
void sendByte(int len)
{
	char reg[4];
	char buff[20]={"AT+CIPSNED="};
	sprintf(reg,"%d",len+2);
	strcat(buff,reg);
	WifiSendString(buff);	
}
bit sendHTTP(int cmd,unsigned char *str)
{
	int i=0,len=0;
	len = strlen(str);
	datelen = len;
	while(!setWifi(CIPSTART,"Linked"))
	{
		 U1SendString("�������һ�����⣬���Ӳ�����\r\n");
		 delay(10000);			//��ʱ���Ӳ��ϣ����������� 
	}
	LED2 = 0;

//	LED2 = 0;		//��ʾ������tcp�ˡ�
	LED3 = 1;
	setWifi(CIPSEND,">");
	delay(1000);
	switch(cmd)
	{
	case 1:
	//	sendByte(strlen(GetCmd));
		delay(1000);	
		sendDate(1,NULL);
		break;
	case 2:
	//	sendByte(strlen(PostCmd));
		delay(1000);
		sendDate(2,str);
		break;
	default :
		sendDate(0,NULL); //����wifi  һ�㲻��
		break;
	}
	LED3 = 0;	//��ʾ�˳�TCP����
	delay(2000);
	U1SendString(Rec_Buf);
	delay(5000);
	
	return true;
}

int main()
{
	int i=0;
	Init();
	init_port();
	
	while(1)
	{
		sendHTTP(GET,NULL);
		delay(1000);
		sendHTTP(POST,"");
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
