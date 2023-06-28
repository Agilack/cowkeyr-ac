/**
 * @file  spi.c
 * @brief This file contains a driver for STM32H5 SPI controller
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
#include "types.h"
#include "spi.h"

/*
SPI6:
 - PE11 : SPI4_NSS  -> CN10 ( 6) "D5"
 - PE12 : SPI4_SCK  -> CN10 (26) "PWM"
 - PE13 : SPI4_MISO -> CN10 (10) "D3"
 - PE14 : SPI4_MOSI -> CN10 ( 8) "D4"
*/

/**
 * @brief Initialize the SPI driver
 *
 */
void spi_init(void)
{
	u32 v;

	// Activate SPI4
	reg_set(RCC_APB2ENR_NS, (1 << 19));

	// Configure format
	v  = (7 << 28); // Set master clock /256
	v |= (7 << 16); // CRC size 8bits (must be set also with CRC disabled)
	v |= (7 <<  0); // Data size 8 bits
	reg_wr(SPI_CFG1(SPI4_NS), v);
	v  = (1 << 22); // Master mode
	v |= (1 << 29); // SSOE : SS output enable
	//v |= (1 << 30); // SSOM : SS output is managed in master mode
	v |= (1 <<  0); // MSSI = 1 clock
	reg_wr(SPI_CFG2(SPI4_NS), v);
	// Set TSIZE to 0 (endless transaction)
	reg_wr(SPI_CR2(SPI4_NS), 0);

	// Enable SPI (set SPE=1)
	reg_set(SPI_CR1(SPI4_NS), (1 << 0));
}
/* EOF */
