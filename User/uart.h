#ifndef __UART_H_
#define __UART_H_

#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1
#define S2RB8 0x04              //S2CON.2
#define S2TB8 0x08              //S2CON.3

void U1SendData(unsigned char ch);
void U1SendString(char *s);
void UartInit(void);
void U2SendData(unsigned char ch);
void U2SendString(char *s);


#endif
