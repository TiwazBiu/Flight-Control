#ifndef __CORE_UART_H__
#define __CORE_UART_H__

#include "common.h"
#include "chiplevel_includes.h"
#include "core_main.h"
#include <includes.h>
#include <core_uartstdio.h>

#define UART1_REC_LEN  			200

extern uint16 UART1_RX_STA;
extern uint8 UART1_RX_BUF[UART1_REC_LEN];

void UART1_STInit(uint32 Baud_rate);
void UART1SendString(uint8* send);
void UART1_IRQHandler(void);
void GPIO_PINB7init(void);


#endif  //__CORE_UART_H__
