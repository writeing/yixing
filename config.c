#include	<config.h>

/***********************************��������***********************************/
void UART1_Send_Byte(unsigned char ddata) 
{	    
	SBUF = ddata;  //д��Ҫ���͵��ַ�
	while(!TI);    //�ȴ��������
	TI = 0;        //�巢�ͱ�־   
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
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD = 0x21;			//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xfa;			//�趨��ʱ��ֵ
	TH1 = 0xfa;			//�趨��ʱ��ֵ
	TR1 = 1;		//������ʱ��1
	EA=1;			//�����ж�

}