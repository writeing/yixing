#include	<config.h>

/***********************************函数声明***********************************/
void UART1_Send_Byte(unsigned char ddata) 
{	    
	SBUF = ddata;  //写入要发送的字符
	while(!TI);    //等待发送完毕
	TI = 0;        //清发送标志   
}

void UART1_Send_String(unsigned char *str)
{
	while(*str!='\0')
	{
		UART1_Send_Byte(*str++);	
	}
} 

void UartInit(void)		//9600bps@11.0592MHz
{
	PCON=0x00;
	SCON = 0x50;		//8位数据,可变波特率
	TMOD = 0x21;			//设定定时器1为16位自动重装方式
	TL1 = 0xfa;			//设定定时初值
	TH1 = 0xfa;			//设定定时初值
	TR1 = 1;		//启动定时器1
	EA=1;			//开总中断

}