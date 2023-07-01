/**
 * @file  uart.c
 * @brief This file contains UART driver for STM32G0 USARTs
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
#include "driver/uart.h"
#include "hardware.h"
#include "types.h"

void uart_init(void)
{
	/* Activate USART3 */
	reg_set(RCC_APB1LENR(RCC), (1 << 18));

	/* Configure UART3 */
	reg_wr(USART_BRR(USART3),  278); // 115200 @ 32MHz
	reg_wr(USART_CR1(USART3), 0x0C); // Set TE & RE
	reg_wr(USART_CR1(USART3), 0x0D); // Set USART enable bit
}

/**
 * @brief Read one byte received on UART
 *
 * @param c Pointer to a byte variable where to store recived data
 * @return True if a byte has been received, False if no data available
 */
int uart_getc(unsigned char *c)
{
	u32 rx;

	if (reg_rd(USART_ISR(USART3)) & (1 << 5))
	{
		/* Get the received byte from RX fifo */
		rx = reg_rd(USART_RDR(USART3));
		/* If a data pointer has been defined, copy received byte */
		if (c)
			*c = (rx & 0xFF);
		return(1);
	}
	return (0);
}


/**
 * @brief Send a single byte to UART
 *
 * @param c Byte to send
 */
void uart_putc(u8 c)
{
	while ((reg_rd(USART_ISR(USART3)) & (1 << 7)) == 0)
		;
	reg_wr(USART_TDR(USART3), c);
}

/**
 * @brief Send a text string to UART
 *
 * @param s Pointer to the string to send
 */
void uart_puts (char *s)
{
	while (*s)
	{
		uart_putc(*s);
		s++;
	}
}
/* EOF */
