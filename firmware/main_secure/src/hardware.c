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

static inline void _cfg_sec(void);
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

	_cfg_sec();

	// Enable GPIO ports
	reg_set(RCC_AHB2ENR(RCC), (1 << 1) | /* GPIO-B */
	                          (1 << 3) | /* GPIO-D */
	                          (1 << 4)); /* GPIO-E */
	// RCC : Reset GPIOB and GPIOD
	reg_wr(RCC_AHB2RST(RCC), (1 << 1) | (1 << 3));
	for (i = 0; i < 16; i++)
		asm volatile("nop");
	reg_wr(RCC_AHB2RST(RCC), 0);

	_init_led();
	_init_uart();
	_init_spi();	
}

/**
 * @brief Configure of the secure context (SAU and TZSC)
 *
 * This function do the minimal configuration of the cortex core to use
 * both secure and non-secure code.
 */
static inline void _cfg_sec(void)
{
	int i;

	// SAU_CTRL set ALLNS
	reg_wr(0xE000EDD0, (1 << 1));
	// SCB NSACR
	reg_wr(0xE000ED8C, 0xC00);

	// Enable TZSC for peripheral protection
	reg_set(RCC_AHB1ENR(RCC), (1 << 24));

	for (i = 0; i < 31; i++)
		reg_wr((u32)(GTZC1 + 0x1000 + 0x200+ (i*4)), (u32)0xFFFFFFFF);

#ifdef PROTECT_CONSOLE
	// Protect USART3
	reg_wr(GTZC1 + 0x10, (1 << 14));
	reg_wr(GTZC1 + 0x20, (1 << 14));
#endif
}

/**
 * @brief Initialize IOs of the LEDs
 *
 */
static inline void _init_led(void)
{
	u32 v;

	// Configure PB0 as output
	v = reg_rd(GPIO_MODER(GPIOB));
	v &= ~(u32)(3 << 0); // Clear curernt mode
	v |=  (u32)(1 << 0); // Set general purpose output mode
	reg_wr(GPIO_MODER(GPIOB), v);

	// Set PB0 to '1'
	reg_wr(GPIO_BSRR(GPIOB), 1);
}

/**
 * @brief Initialize IOs of the SPI interface(s)
 *
 */
static inline void _init_spi(void)
{
	u32 v;

	/* Select appropriate alterna functions for PE/SPI4 */
	v = reg_rd(GPIO_AFRH(GPIOE));
	v &= ~(u32)(0xF << 12); // Clear current PE11 config
	v &= ~(u32)(0xF << 16); // Clear current PE12 config
	v &= ~(u32)(0xF << 20); // Clear current PE13 config
	v &= ~(u32)(0xF << 24); // Clear current PE14 config
	v |=  (u32)(  5 << 12); // PE11 use AF5 (SPI4_NSS)
	v |=  (u32)(  5 << 16); // PE12 use AF5 (SPI4_SCK)
	v |=  (u32)(  5 << 20); // PE13 use AF5 (SPI4_MISO)
	v |=  (u32)(  5 << 24); // PE14 use AF5 (SPI4_MOSI)
	reg_wr(GPIO_AFRH(GPIOE), v);
	/* Set ios mode to use alternate-function */
	v = reg_rd(GPIO_MODER(GPIOE));
	v &= ~(u32)( (3 << 22) | (3 << 24) | (3 << 26) | (3 << 28) );
	v |=  (u32)( (2 << 22) | (2 << 24) | (2 << 26) | (2 << 28) );
	reg_wr(GPIO_MODER(GPIOE), v);
}

/**
 * @brief Initialize IOs of the console (uart)
 *
 */
static inline void _init_uart(void)
{
	u32 v;

	/* Configure PD8 (UART-TX) and PD9 (UART-RX) to use AF7 (USART3) */
	v = reg_rd(GPIO_AFRH(GPIOD));
	v &= ~(u32)(0xF << 0); // Clear current PD8 config
	v |=  (u32)(  7 << 0); // PD8 use AF7
	v &= ~(u32)(0xF << 4); // Clear current PD9 config
	v |=  (u32)(  7 << 4); // PD9 use AF7
	reg_wr(GPIO_AFRH(GPIOD), v);

	/* Configure PD8 and PD9 mode to use alternate function */
	v = reg_rd(GPIO_MODER(GPIOD));
	v &= ~(u32)( (3 << 16) | (3 << 18) );
	v |=  (u32)( (2 << 16) | (2 << 18) );
	reg_wr(GPIO_MODER(GPIOD), v);
}
/* EOF */
