#ifndef __UART_H__
#define __UART_H__

void UART_Init(void);
int  UART_Rx(char* ch);
void UART_Tx(char ch);

#endif
