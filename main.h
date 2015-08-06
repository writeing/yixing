#include <STC15F2K60S2.H>
#include "wifi.h"
#include "delay.h"		        // ��ʱ����ͷ�ļ�
#include "uart.h"		        	// ����ͨ�ź���ͷ�ļ�
#include <string.h>           // �����ͷ�ļ���,��ʹ��strstr�⺯��
#include <intrins.h>					// �����ͷ�ļ���,��ʹ��_nop_�⺯��
#include <stdlib.h>
#include <stdio.h>

#define CWJAP 1
#define CWMODE 2
#define CWQAP 3
#define CIPSTART 4
#define CIPSEND 5
#define CIPMODE 6
#define CIPCLOSE 7
#define Buf_Max 250

#define false 0
#define true 1

#define GET 1
#define POST 2
#define uchar unsigned char
#define uint unsigned int



void init_port(void);
char isOK(void);			  //�ж��Ƿ�ɹ�
void sendCmd(uint cmd);		  //��������
void CLR_Buf(void);			  //��ս��ܻ�����
void delay(uint time);		  //��ʱ
bit Hand(unsigned char *a);	  //�ж��Ƿ�ɹ�
void Init(void);			  //��ʼ��esp8266
void sendDate(uint cmd,uchar *str);	  //��������
bit setWifi(int cmd,unsigned char *str);
bit sendHTTP(int cmd,unsigned char *str);