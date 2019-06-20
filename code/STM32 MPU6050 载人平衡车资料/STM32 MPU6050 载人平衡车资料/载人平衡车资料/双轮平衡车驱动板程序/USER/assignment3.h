#ifndef  _assignment3_H_
#define  _assignment3_H_

#include "stm32f10x.h"
#include <stdio.h>

void USART2_Config(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);
void USART2_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
