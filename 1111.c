/************************************************************************************
Copyright,  LCE STUDIO   
�������ƣ�ESP8266 WiFi����ģ��ͨ�ţ�18.432MHz�� 
����˵��: �ó���ESP8266����ΪSTATION����ģʽ����ͨ��wifi�ȵ�����֪��TCP sevsr�������ӣ�
					��TCP sever����������Ϣ����������ִ��TCP sever��ָ������յ���openled1���󣬽�����LED1 
					���򴮿�1�ʹ���2����һ��buffer,���յ�����1�ġ�openled1����Ҳ��ִ������ָ�
					����1�ʹ���2�����ʾ�Ϊ9600
����汾��1.0 ��2014/10��
����������rob1983
�Ա����̣�http://shop67639654.taobao.com 
************************************************************************************/


#include "stc15f2k60s2.h"	    // ��Ƭ��STC15F2K60S2ͷ�ļ�,���Բ��ټ���reg51.h
#include <intrins.h>					// �����ͷ�ļ���,��ʹ��_nop_�⺯��
#include "delay.h"		        // ��ʱ����ͷ�ļ�
#include "uart.h"		        	// ����ͨ�ź���ͷ�ļ�
#include <string.h>           // �����ͷ�ļ���,��ʹ��strstr�⺯��

sbit RST = P2^6;

#define Buf_Max 50

unsigned char xdata Rec_Buf[Buf_Max];
unsigned char i = 0;   
void CLR_Buf(void);				   
bit  Hand(unsigned char *a); 	


		 
sbit LED1 = P5^0;		    // ����LED1ΪP5.1 
sbit LED2 = P5^1;				// ����LED2ΪP5.2
sbit LED3 = P5^2;				// ����LED3ΪP5.3

char code str1[]="AT\r\n";                                    		    //  ����ָ�����"OK"
char code str2[]="AT+CWMODE=1\r\n";                         		      //  ����ESP8266�Ĺ���ģʽ������"OK"����"no change"
char code str3[]="AT+CWJAP=\"lces\",\"88518851\"\r\n";     			      //  ���ӵ�WiFi�ȵ㣬lcesΪ�ȵ����ƣ�88518851Ϊ���룻���ӳɹ����ء�OK��     
char code str4[]="AT+CIFSR\r\n";                                   	  // 	����IP��ַ��ѯָ��
char code str5[]="AT+CIPSTART=\"TCP\",\"192.168.191.1\",8234\r\n";    //  ���ӵ�TCP�����������ء�Linked��
char code str6[]="AT+CIPSEND=6\r\n";   															  //  ��������ָ��
char code str7[]="hello!\r\n";   																			//  ��������

void main()		                                  // ������     
{	

	RST = 1;                                      // ESP8266��λ���ܽţ����ͻὫESP8266��λ
	UartInit();								                		// ��ʼ������
	ES = 1;                                       // ����1�жϴ�
	IE2 = 0x01;                                   // ����2�жϴ�
	EA = 1;                                       // ���жϴ�
	DelayMS(1000);							                  // ��ʱһ��ʱ�䣬��ESP8266����
	DelayUS(100);
  U1SendString(Rec_Buf);                        // ��ESP8266������Ϣͨ������1��ӡ��  
  U1SendString("\r\n");	
  U1SendString("Welcome to LSE STUDIO, Please wait while we are getting the device ready\r\n");	
	CLR_Buf();                                 		//�����������	

	
	while(!Hand("OK"))                            //�ж��Ƿ����ֳɹ�,������ɹ���ʱһ��,�ٷ���AT����ָ��
	{
		U2SendString(str1);                         //��������ָ�� 
		DelayMS(500);
	}
	CLR_Buf();                                    //�����������
  	U1SendString("OK,Succeed Establish connection with ESP8266\r\n");		
	LED1 = 0;

	
	while(!(Hand("OK")|Hand("no change")))        //�ж��Ƿ����óɹ����粻�ɹ�����ʱ���ٴη���
	{		
		U2SendString(str2);                         //��������ESP8266����ģʽָ��	
		DelayMS(500);		
	}
	CLR_Buf();                 
  U1SendString("OK,ESP8266 has been set as Station Mode\r\n");	

	
	while(!Hand("OK"))    										 		//�ж��Ƿ�����WiFi·�������粻�ɹ�����ʱ���ٴη���
	{		
		U2SendString(str3); 	
		DelayMS(2000);		
	}
	LED2 = 0;
	CLR_Buf();              
  U1SendString("OK,Succeed establish connection with WiFi AP\r\n");	

	
	
	while(!Hand("Linked"))                        //�ж��Ƿ�����TCP sever���粻�ɹ�����ʱ���ٴη���
	{
		U2SendString(str5);  
		DelayMS(3000);
		
	}
	CLR_Buf();                
  U1SendString("OK,Succeed establish connection with TCP sever\r\n");			
	LED1 = 1;
	LED2 = 1;
	
	
	while(!Hand("SEND OK"))     									//�ж��Ƿ������ݳɹ����粻�ɹ�����ʱ���ٴη���
	{

		U2SendString(str6); //���ݷ���ָ�� 
		DelayMS(100);		
		U2SendString(str7); //��������	
		DelayMS(500);
	}
	CLR_Buf();                
  U1SendString("Congratulations, You can send commands through TCP sever now\r\n");				

	
  while (1)		                                        // ��ѭ��  
    {
			if(Hand("openled1"))                            //  �յ���openled1����ָ��
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
			else if(Hand("openled2"))                         //  �յ���openled2����ָ��
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
			else if(Hand("openled3"))                         //  �յ���openled3����ָ��
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