/************************************************************************************
Copyright,  LCE STUDIO  
程序名称：串口通信（晶振频率18.432MHz）  
功能说明: 串口1和串口2初始化,字符发送与字符串发送函数,波特率9600bps
程序版本：1.0 （2014/06）
阿里旺旺：rob1983
淘宝店铺：http://shop67639654.taobao.com 
************************************************************************************/ 

#include "stc15f2k60s2.h"
#include "intrins.h"
#include "uart.h"
#include "delay.h"

void SendData(unsigned char ch);
void SendString(char *s);
void UartInit(void);
void WifiSendData(unsigned char ch);
void WifiSendString(char *s);


/*----------------------------
串口初始化
----------------------------*/

void UartInit(void)		//115200bps@18.432MHz
{
	PCON &= 0x7F;		
	SCON = 0x50;		//8位数据，可变波特率
	AUXR |= 0x40;		//定时器1时钟1为FOSC,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;	    	//设定定时初值
	TH1 = 0xFD;			//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1

	S2CON = 0x50;		//8位数据，可变波特率
	AUXR |= 0x04;		//定时器2时钟1为FOSC,即1T
	IE2=0x01;
//  以下为单片机串口2的波特率初始化值，因ESP8266模块串口波特率可能为9600和115200，使用过程中如无法通信，可通过以下串口初始化时进行选择。
	T2L = 0xE6;		//波特率9600设定初始值
	T2H = 0xFE;		//波特率9600设定初始值
//	T2L = 0xD8;		//波特率115200设定初始值
//	T2H = 0xFF;		//波特率115200设定初始值	
	
	AUXR |= 0x10;		//启动定时器2
}


/*----------------------------
通过串口1发送串口数据
----------------------------*/
void U1SendData(unsigned char ch)
{
    SBUF = ch;                 //写数据到UART数据寄存器
	while(TI == 0);
	TI = 0;
}

/*----------------------------
通过串口1发送字符串
----------------------------*/
void U1SendString(char *s)
{
    while ((*s)!='\0')                  //检测字符串结束标志
    {
        U1SendData(*s++);         //发送当前字符
    }
}

/*----------------------------
通过串口2发送串口数据
----------------------------*/
void WifiSendData(unsigned char ch)
{
    S2BUF = ch;                 //写数据到UART数据寄存器
	while(!(S2CON&S2TI));    
    S2CON&=~S2TI;
}

/*----------------------------
通过串口2发送字符串
----------------------------*/
void WifiSendString(char *s)
{
 	IE2 = 0x00; 
	while ((*s)!='\0')                  //检测字符串结束标志
    {
        WifiSendData(*s++);         //发送当前字符
    }
	WifiSendData('\r');
	WifiSendData('\n');
 	IE2 = 0x01;
}
