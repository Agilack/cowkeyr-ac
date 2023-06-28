/**
 * @file  hardware.c
 * @brief Low-level cowstick-ums hardware configuration
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

static inline void _init_led(void);
static inline void _init_spi(void);
static inline void _init_uart(void);

/**
 * @brief Initialize processor, clocks and some peripherals
 *
 * This function should be called on startup for clocks and IOs configuration.
 */
void hw_init(void)
{
	int i;

	// Enable GPIO ports
	reg_set(RCC_AHB2ENR_NS, (1 << 1) | /* GPIO-B */
	                        (1 << 3) | /* GPIO-D */
	                        (1 << 4)); /* GPIO-E */
	// RCC : Reset GPIOB and GPIOD
	reg_wr(RCC_AHB2RST_NS, (1 << 1) | (1 << 3));
	for (i = 0; i < 16; i++)
		asm volatile("nop");
	reg_wr(RCC_AHB2RST_NS, 0);

	_init_led();
	_init_uart();
	_init_spi();	
}

/**
 * @brief Initialize IOs of the LEDs
 *
 */
static inline void _init_led(void)
{
	u32 v;

	// Configure PB0 as output
	v = reg_rd(GPIO_MODER(GPIOB_NS));
	v &= ~(u32)(3 << 0); // Clear curernt mode
	v |=  (u32)(1 << 0); // Set general purpose output mode
	reg_wr(GPIO_MODER(GPIOB_NS), v);

	// Set PB0 to '1'
	reg_wr(GPIO_BSRR(GPIOB_NS), 1);
}

/**
 * @brief Initialize IOs of the SPI interface(s)
 *
 */
static inline void _init_spi(void)
{
	u32 v;

	/* Select appropriate alterna functions for PE/SPI4 */
	v = reg_rd(GPIO_AFRH(GPIOE_NS));
	v &= ~(u32)(0xF << 12); // Clear current PE11 config
	v &= ~(u32)(0xF << 16); // Clear current PE12 config
	v &= ~(u32)(0xF << 20); // Clear current PE13 config
	v &= ~(u32)(0xF << 24); // Clear current PE14 config
	v |=  (u32)(  5 << 12); // PE11 use AF5 (SPI4_NSS)
	v |=  (u32)(  5 << 16); // PE12 use AF5 (SPI4_SCK)
	v |=  (u32)(  5 << 20); // PE13 use AF5 (SPI4_MISO)
	v |=  (u32)(  5 << 24); // PE14 use AF5 (SPI4_MOSI)
	reg_wr(GPIO_AFRH(GPIOE_NS), v);
	/* Set ios mode to use alternate-function */
	v = reg_rd(GPIO_MODER(GPIOE_NS));
	v &= ~(u32)( (3 << 22) | (3 << 24) | (3 << 26) | (3 << 28) );
	v |=  (u32)( (2 << 22) | (2 << 24) | (2 << 26) | (2 << 28) );
	reg_wr(GPIO_MODER(GPIOE_NS), v);
}

/**
 * @brief Initialize IOs of the console (uart)
 *
 */
static inline void _init_uart(void)
{
	u32 v;

	/* Configure PD8 (UART-TX) and PD9 (UART-RX) to use AF7 (USART3) */
	v = reg_rd(GPIO_AFRH(GPIOD_NS));
	v &= ~(u32)(0xF << 0); // Clear current PD8 config
	v |=  (u32)(  7 << 0); // PD8 use AF7
	v &= ~(u32)(0xF << 4); // Clear current PD9 config
	v |=  (u32)(  7 << 4); // PD9 use AF7
	reg_wr(GPIO_AFRH(GPIOD_NS), v);

	/* Configure PD8 and PD9 mode to use alternate function */
	v = reg_rd(GPIO_MODER(GPIOD_NS));
	v &= ~(u32)( (3 << 16) | (3 << 18) );
	v |=  (u32)( (2 << 16) | (2 << 18) );
	reg_wr(GPIO_MODER(GPIOD_NS), v);
}
/* EOF */
