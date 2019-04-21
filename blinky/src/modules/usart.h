#ifndef __USART_H__
#define __USART_H__

#include <stdint.h>

void MX_USART3_UART_Init(void);
void UART_Puts(const char * str);
void print(const char * str, ...);

#endif
