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
#include "driver/spi.h"
#include "driver/uart.h"
#include "log.h"

void spi_test(void);
uint wait_key(void);

/**
 * @brief Entry point of the C code
 *
 */
int main(void)
{
	// Board init
	hw_init();
	// Drivers init
	uart_init();
	spi_init();
	// Functional modules init
	log_init();

	log_print(0, "%{--{ CowKeyr-AC }==--%}\n", LOG_BBLU);

	spi_test();

	while(1)
	{
		unsigned char c;
		if (uart_getc(&c))
		{
			log_print(0, "RX %c\n", c);
		}
	}
}

/**
 * @brief Firsts tests of the SPI driver during firmware development
 *
 */
void spi_test(void)
{
	u32 v1, v2;

	log_print(0, " SPI registers\n");

	v1 = reg_rd( SPI_CR1(SPI4_NS) );
	v2 = reg_rd( SPI_CR2(SPI4_NS) );
	log_print(0, "   CR1 %32x  CR2 %16x\n", v1, v2);

	v1 = reg_rd( SPI_CFG1(SPI4_NS) );
	v2 = reg_rd( SPI_CFG2(SPI4_NS) );
	log_print(0, "  CFG1 %32x CFG2 %32x\n\n", v1, v2);

	reg_wr( SPI_CR2(SPI4_NS),  0); // Endless transaction

	// Push one byte into TC fifo
	log_print(0, "Send byte (wait key) ... ");
	wait_key();
	reg8_wr( SPI_TXDR(SPI4_NS), 0xAA);
	log_print(0, "0xAA sent\n");

	// Set CSTART to start SPI transfer
	log_print(0, "Set CSTART (wait key) ... ");
	wait_key();
	reg_set(SPI_CR1(SPI4_NS), (1 << 9));
	log_print(0, "done.\n");

	// Set CSUSP to finish endless transfer
	log_print(0, "Set SUSP to finish transaction ... ");
	wait_key();
	reg_set(SPI_CR1(SPI4_NS), (1 << 10));
	log_print(0, "done\n");

	log_print(0, "Transaction complete\n\n");

	wait_key();
	reg_clr(SPI_CR1(SPI4_NS), (1 << 10));
	log_print(0, "Ready for second transaction\n");

	wait_key();
	reg8_wr( SPI_TXDR(SPI4_NS), 0xAA);
	reg_set(SPI_CR1(SPI4_NS), (1 << 9));
	while ((reg_rd(SPI_SR(SPI4_NS)) & 1) == 0)
		;
	reg_set(SPI_CR1(SPI4_NS), (1 << 10));
	log_print(0, "Sencond transaction complete\n");

	wait_key();
	reg_wr( SPI_CR2(SPI4_NS),  1);
	reg8_wr( SPI_TXDR(SPI4_NS), 0xAA);
	wait_key();
	reg_set(SPI_CR1(SPI4_NS), (1 << 9));
	wait_key();
	reg_set(SPI_CR1(SPI4_NS), (1 << 10));
}

uint wait_key(void)
{
	while(1)
	{
		unsigned char c;
		if (uart_getc(&c))
			return(c);
	}
}
/* EOF */
