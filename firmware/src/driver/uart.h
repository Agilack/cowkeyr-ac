/**
 * @file  uart.h
 * @brief Headers and definitions for STM32H5 USART driver
 *
 * @author Saint-Genest Gwenael <gwen@cowlab.fr>
 * @copyright Agilack (c) 2023
 *
 * @page License
 * Cowkeyr-AC firmware is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 3 as published by the Free Software Foundation. You should have
 * received a copy of the GNU Lesser General Public License along with this
 * program, see LICENSE.md file for more details.
 * This program is distributed WITHOUT ANY WARRANTY.
 */
#ifndef UART_H
#define UART_H
#include "types.h"

// USART registers
#define USART_CR1(x)   (x + 0x00)
#define USART_CR2(x)   (x + 0x04)
#define USART_CR3(x)   (x + 0x08)
#define USART_BRR(x)   (x + 0x0C)
#define USART_GTPR(x)  (x + 0x10)
#define USART_RTOR(x)  (x + 0x14)
#define USART_RQR(x)   (x + 0x18)
#define USART_ISR(x)   (x + 0x1C)
#define USART_ICR(x)   (x + 0x20)
#define USART_RDR(x)   (x + 0x24)
#define USART_TDR(x)   (x + 0x28)
#define USART_PRESC(x) (x + 0x2C)

void uart_init(void);
int  uart_getc(unsigned char *c);
void uart_putc(u8 c);
void uart_puts(char *s);

#endif
