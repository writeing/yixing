/************************************************************************************
Copyright,  LCE STUDIO   
程序名称：ESP8266 WiFi串口模块通信（18.432MHz） 
功能说明: 该程序将ESP8266设置为STATION工作模式，并通过wifi热点与已知的TCP sevsr建立连接，
					向TCP sever发送连机信息，并接收与执行TCP sever的指令。例如收到“openled1”后，将点亮LED1 
					（因串口1和串口2共用一个buffer,当收到串口1的“openled1“，也将执行上述指令）
					串口1和串口2波特率均为9600
程序版本：1.0 （2014/10）
阿里旺旺：rob1983
淘宝店铺：http://shop67639654.taobao.com 
************************************************************************************/


#include "stc15f2k60s2.h"	    // 单片机STC15F2K60S2头文件,可以不再加入reg51.h
#include <intrins.h>					// 加入此头文件后,可使用_nop_库函数
#include "delay.h"		        // 延时函数头文件
#include "uart.h"		        	// 串行通信函数头文件
#include <string.h>           // 加入此头文件后,可使用strstr库函数

sbit RST = P2^6;

#define Buf_Max 50

unsigned char xdata Rec_Buf[Buf_Max];
unsigned char i = 0;   
void CLR_Buf(void);				   
bit  Hand(unsigned char *a); 	


		 
sbit LED1 = P5^0;		    // 定义LED1为P5.1 
sbit LED2 = P5^1;				// 定义LED2为P5.2
sbit LED3 = P5^2;				// 定义LED3为P5.3

char code str1[]="AT\r\n";                                    		    //  联机指令，返回"OK"
char code str2[]="AT+CWMODE=1\r\n";                         		      //  设置ESP8266的工作模式，返回"OK"或者"no change"
char code str3[]="AT+CWJAP=\"lces\",\"88518851\"\r\n";     			      //  连接到WiFi热点，lces为热点名称，88518851为密码；连接成功返回“OK”     
char code str4[]="AT+CIFSR\r\n";                                   	  // 	本机IP地址查询指令
char code str5[]="AT+CIPSTART=\"TCP\",\"192.168.191.1\",8234\r\n";    //  连接到TCP服务器，返回“Linked”
char code str6[]="AT+CIPSEND=6\r\n";   															  //  发送数据指令
char code str7[]="hello!\r\n";   																			//  数据内容

void main()		                                  // 主函数     
{	

	RST = 1;                                      // ESP8266复位功能脚，拉低会将ESP8266复位
	UartInit();								                		// 初始化串口
	ES = 1;                                       // 串口1中断打开
	IE2 = 0x01;                                   // 串口2中断打开
	EA = 1;                                       // 总中断打开
	DelayMS(1000);							                  // 延时一段时间，让ESP8266启动
	DelayUS(100);
  U1SendString(Rec_Buf);                        // 将ESP8266启动信息通过串口1打印出  
  U1SendString("\r\n");	
  U1SendString("Welcome to LSE STUDIO, Please wait while we are getting the device ready\r\n");	
	CLR_Buf();                                 		//清除缓存内容	

	
	while(!Hand("OK"))                            //判断是否握手成功,如果不成功延时一会,再发送AT握手指令
	{
		U2SendString(str1);                         //发送联机指令 
		DelayMS(500);
	}
	CLR_Buf();                                    //清除缓存内容
  	U1SendString("OK,Succeed Establish connection with ESP8266\r\n");		
	LED1 = 0;

	
	while(!(Hand("OK")|Hand("no change")))        //判断是否设置成功，如不成功，延时后再次发送
	{		
		U2SendString(str2);                         //发送设置ESP8266工作模式指令	
		DelayMS(500);		
	}
	CLR_Buf();                 
  U1SendString("OK,ESP8266 has been set as Station Mode\r\n");	

	
	while(!Hand("OK"))    										 		//判断是否连接WiFi路由器，如不成功，延时后再次发送
	{		
		U2SendString(str3); 	
		DelayMS(2000);		
	}
	LED2 = 0;
	CLR_Buf();              
  U1SendString("OK,Succeed establish connection with WiFi AP\r\n");	

	
	
	while(!Hand("Linked"))                        //判断是否连接TCP sever，如不成功，延时后再次发送
	{
		U2SendString(str5);  
		DelayMS(3000);
		
	}
	CLR_Buf();                
  U1SendString("OK,Succeed establish connection with TCP sever\r\n");			
	LED1 = 1;
	LED2 = 1;
	
	
	while(!Hand("SEND OK"))     									//判断是否发送数据成功，如不成功，延时后再次发送
	{

		U2SendString(str6); //数据发送指令 
		DelayMS(100);		
		U2SendString(str7); //数据内容	
		DelayMS(500);
	}
	CLR_Buf();                
  U1SendString("Congratulations, You can send commands through TCP sever now\r\n");				

	
  while (1)		                                        // 主循环  
    {
			if(Hand("openled1"))                            //  收到“openled1”的指令
				{
					ES = 0; 
					IE2 = 0x00;					
					LED1 = 0;
					DelayMS(1000);	
					DelayMS(1000);
					LED1 = 1;				
					CLR_Buf();
					U1SendString("Command: openled1, Executed!\r\n");						
					ES = 1;		
					IE2 = 0x01;						
				}      
			else if(Hand("openled2"))                         //  收到“openled2”的指令
				{
					ES = 0; 
					IE2 = 0x00;						
					LED2 = 0;
					DelayMS(1000);	
					DelayMS(1000);
					LED2 = 1;				
					CLR_Buf();
					U1SendString("Command: openled2, Executed!\r\n");						
					ES = 1;					
					IE2 = 0x01;						
				}	
			else if(Hand("openled3"))                         //  收到“openled3”的指令
				{
					ES = 0; 
					IE2 = 0x00;						
					LED3 = 0;
					DelayMS(1000);	
					DelayMS(1000);
					LED3 = 1;				
					CLR_Buf();
					U1SendString("Command: openled2, Executed!\r\n");						
					ES = 1;					
					IE2 = 0x01;						
				}					
    }
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