/***********************************
8-5号
因为用arduino电脑经常蓝屏，怀疑的串口的原因，所以放弃arduino，专用stm15w开发，并且传送到git上面，




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



sbit LED1 = P5^0;		    // 定义LED1为P5.1 
sbit LED2 = P5^1;				// 定义LED2为P5.2
sbit LED3 = P5^2;				// 定义LED3为P5.3

/**************************
函数名称：
函数作用：
函数参数：
函数备注：
***************************/

/**************************
函数名称:sendCmd
函数作用：发送wifi参数
函数参数：cmd
函数备注：
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
函数名称:sendDate
函数作用：发送wifi数据
函数参数：cmd
函数备注：
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
函数名称：delay()
函数作用：延时
函数参数：延时时间，单位 秒
函数备注：
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
函数名称：Init
函数作用：初始化一些设置
函数参数：
函数备注：
***************************/
void Init()
{		
	RST = 1;                                      // ESP8266复位功能脚，拉低会将ESP8266复位
	UartInit();								      // 初始化串口
	ES = 1;                                       // 串口1中断打开
	IE2 = 0x01;                                   // 串口2中断打开
	EA = 1;                                       // 总中断打开
	DelayMS(3000);							                  // 延时一段时间，让ESP8266启动

  	U1SendString(Rec_Buf);                        // 将ESP8266启动信息通过串口1打印出    	
  	U1SendString("Welcome to LSE STUDIO,\r\n");	
	CLR_Buf();                                 		//清除缓存内容	              
/*****************************/	
	setWifi(CWMODE,"OK");  	
	while(!setWifi(CWJAP,"OK"))
	{
		delay(10000);
		U1SendString("the wifi connecting\r\n");
	}
	LED1 = 0;			//点亮第一个灯,表示wifi连接上了
	setWifi(CIPMODE,"OK");
	while(!setWifi(CIPSTART,"Linked"))
	{
		 U1SendString("好像出了一点问题，连接不上了\r\n");
		 delay(10000);			//暂时连接不上，待会再连接 
	}
	LED2 = 0;
/****************************/
//	setWifi(CIPSTART,"Linked");
//	DelayMS(1000);	
//	while(!Hand("Unlink"))     									//判断是否发送数据成功，如不成功，延时后再次发送
//	{
//		CLR_Buf();		
//		sendCmd(CIPSEND); //数据内容	
//		DelayMS(500);
//		sendDate(1,""); //数据内容	
//		DelayMS(500);
//		U1SendString(Rec_Buf);
//	}
	CLR_Buf();                
  	U1SendString("Congratulations, You can send commands through TCP sever now\r\n");							
}
/**************************
函数名称：setWifi 
函数作用：发送wifi的一些指令
函数参数：cmd发送的参数 str需要匹配的数据
函数备注：
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
		if(Hand("no change")|Hand("CONNECT")) 		//如果是no change 或者是Linked就直接返回
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
函数名称：sendHTTP
函数作用：发送http数据
函数参数：是发送还是接收，发送的数据是什么，
函数备注：
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
		 U1SendString("好像出了一点问题，连接不上了\r\n");
		 delay(10000);			//暂时连接不上，待会再连接 
	}
	LED2 = 0;

//	LED2 = 0;		//表示连接上tcp了。
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
		sendDate(0,NULL); //重启wifi  一般不用
		break;
	}
	LED3 = 0;	//表示退出TCP连接
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
函数名称：Hand
函数作用：判断是否运行成功
函数参数：输入要判断的字符串
函数备注：
***************************/
bit Hand(unsigned char *a)
{ 
    if(strstr(Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}
/**************************
函数名称：CLR_Buf
函数作用：清空接收缓存
函数参数：
函数备注：
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
      RI = 0;                 //清除RI位
	  Rec_Buf[i] = SBUF; 
	  i++;               
	  if(i>Buf_Max)          
	  {
		i = 0;
	  } 
    }
    if (TI)
    {
        TI = 0;                 //清除TI位

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
函数名称：init_port
函数作用：初始化一些串口数据，让每个IO口都是通用IO
函数参数：
函数备注：
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
