#include "wifi.h"

void WifiSend(char *cmd)
{
//	USART2_SEND(cmd);
	UART1_Send_String(cmd);
}
void USART2_byte(char a)
{
		
}
void USART2_SEND(char *cmd)
{
	int i=0,j=0;
	i=strlen(cmd);
	for(j=0;j<i;j++)
	{
		USART2_byte(*cmd);
		cmd++;	
	}
	USART2_byte('\n');
}