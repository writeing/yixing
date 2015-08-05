#include <STC15F2K60S2.H>
#include "wifi.h"
#include "delay.h"		        // 延时函数头文件
#include "uart.h"		        	// 串行通信函数头文件
#include <string.h>           // 加入此头文件后,可使用strstr库函数
#include <intrins.h>					// 加入此头文件后,可使用_nop_库函数


#define CWJAP 1
#define CWMODE 2
#define CWQAP 3
#define CIPSTART 4
#define CIPSEND 5
#define CIPMODE 6
#define CIPCLOSE 7
#define Buf_Max 250


#define uchar unsigned char
#define uint unsigned int




char isOK(void);			  //判断是否成功
void sendCmd(uint cmd);		  //发送命令
void CLR_Buf(void);			  //清空接受缓存区
void delay(uint time);		  //延时
bit Hand(unsigned char *a);	  //判断是否成功
void Init(void);			  //初始化esp8266
void sendDate(uint cmd);	  //发送数据