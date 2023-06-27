/**
 * @file  main.c
 * @brief Entry point of the firmware and main application loop
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
#include "hardware.h"
#include "uart.h"

/**
 * @brief Entry point of the C code
 *
 */
int main(void)
{
	const char msg[] = "Hello World !\r\n";
	const char *s;

	// Enable GPIO-B
	reg_set(RCC_AHB2ENR_NS, (1 << 1));
	// Configure PB0 as output
	reg_wr(GPIOB_NS, 0x01);
	// Set PB0 to '1'
	reg_wr(GPIOB_NS+ 0X18, 1);

	// Enable USART-3
	reg_set(RCC_APB1LENR_NS, (1 << 18));
	// Configure USART-3
	reg_wr(USART_BRR(USART3_NS),  278); // 115200 @ 32MHz
	reg_wr(USART_CR1(USART3_NS), 0x0C); // Set TE & RE
	reg_wr(USART_CR1(USART3_NS), 0x0D); // Set USART enable bit

	// Enable GPIO-D
	reg_set(RCC_AHB2ENR_NS, (1 << 3));
	// Configure IOs to use UART alternate-function
	reg_wr (GPIOD_NS+0x24, (7 << 0)); // PD8 use AF7
	reg_set(GPIOD_NS+0x24, (7 << 4)); // PD9 use AF7
	// Configure TX pin as output
	reg_wr(GPIOD_NS+0x00, 0xFFFAFFFF);

	// Send message string to uart
	for (s = msg; *s != 0; s++)
	{
		// Wait for UART ready to transmit
		while ((reg_rd(USART_ISR(USART3_NS)) & (1 << 7)) == 0)
			;
		// Send a single test byte
		reg_wr(USART_TDR(USART3_NS), *s);
	}

	while(1);
}
/* EOF */
