/***********************************
8-5号
因为用arduino电脑经常蓝屏，怀疑的串口的原因，所以放弃arduino，专用stm15w开发，并且传送到git上面，




************************************/
#include "main.h"
/*****************************************/

/*****************************************/
uchar ssid[]={"ih"};
uchar passwd[]={"yixing123456"};
uchar IPAddress[]={"192,168,1,104"};
uint Port=5000;
uchar RemoteIP[4]={0};
char GetCmd[]={"GET / HTTP/1.1\r\nHost: 192.168.1.104\r\nUser-Agent: abc\r\nConnection: close\r\n"};
char PostCmd[]={"POST / HTTP/1.1\r\nHost: 192.168.1.104\r\nUser-Agent: abc\r\nContent-type: text/plain\r\nContent-Length:10\r\n\r\nabcdefghij\r\nConnection: close\r\n"};
char buffer[1024];


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
函数名称：
函数作用：
函数参数：
函数备注：
***************************/
char isOK()
{
	int i=0;
	char OK[]={"ok"};
	char err[]={"err"};
	int t=0;
	t= strlen(buffer);
	if(buffer[t-2]=='o'&&buffer[t-1]=='k')
	{
		return 1;
	}
	return '0';	
}
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
}
/**************************
函数名称:sendDate
函数作用：发送wifi数据
函数参数：cmd
函数备注：
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
void Init()
{		
	RST = 1;                                      // ESP8266复位功能脚，拉低会将ESP8266复位
	UartInit();								      // 初始化串口
	ES = 1;                                       // 串口1中断打开
	IE2 = 0x01;                                   // 串口2中断打开
	EA = 1;                                       // 总中断打开
	DelayMS(1000);							                  // 延时一段时间，让ESP8266启动

  	U1SendString("123456789");                        // 将ESP8266启动信息通过串口1打印出    	
  	U1SendString("Welcome to LSE STUDIO,\r\n");	
	U1SendString("aaa");
	while(!Hand("OK"))                            //判断是否握手成功,如果不成功延时一会,再发送AT握手指令
	{
		CLR_Buf();                                 		//清除缓存内容	
		WifiSendString("AT");                         //发送联机指令 
		DelayMS(500);
		U1SendString("AT\n");
	}
	CLR_Buf();                                    //清除缓存内容
  	U1SendString("OK,Succeed Establish connection with ESP8266\r\n");		
	LED1 = 0;

	
	while(!(Hand("OK")))        //判断是否设置成功，如不成功，延时后再次发送
	{		
		sendCmd(CWMODE);                         //发送设置ESP8266工作模式指令	
		DelayMS(500);		
	}
	CLR_Buf();                 
  	U1SendString("OK,ESP8266 has been set as Station Mode\r\n");	

	
	while(!Hand("OK")|Hand("no change"))    										 		//判断是否连接WiFi路由器，如不成功，延时后再次发送
	{		
		sendCmd(CWJAP); 	
		DelayMS(2000);		
	}
	LED2 = 0;
	CLR_Buf();              
  	U1SendString("OK,Succeed establish connection with WiFi AP\r\n");			
	while(!Hand("Linked"))                        //判断是否连接TCP sever，如不成功，延时后再次发送
	{
		sendCmd(CIPSTART);  
		DelayMS(3000);
		
	}
	CLR_Buf();                
  	U1SendString("OK,Succeed establish connection with TCP sever\r\n");			
	LED1 = 1;
	LED2 = 1;		
	while(!Hand("SEND OK"))     									//判断是否发送数据成功，如不成功，延时后再次发送
	{

		sendCmd(CIPMODE); //数据发送指令 
		DelayMS(100);		
		sendCmd(CIPSEND); //数据内容	
		DelayMS(500);
		sendDate(1); //数据内容	
		DelayMS(500);
	}
	CLR_Buf();                
  	U1SendString("Congratulations, You can send commands through TCP sever now\r\n");							
}
int main()
{
	int i=0;
	U1SendString("wocaonima\n");
	Init();
	U1SendString("tooo\n");
	while(1)
	{
		U1SendString("wangdaye\r\n");	
	}
	return 0;
}


bit Hand(unsigned char *a)
{ 
    if(strstr(Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}

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



void Uart2() interrupt 8 using 1
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

