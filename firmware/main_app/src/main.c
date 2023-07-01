/**
 * @file  main.c
 * @brief Entry point of the non-secure) application
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

#define USART3 0x40004800

typedef unsigned long  u32;
typedef unsigned char  u8;

void uart_putc(u8 c);
void uart_puts(char *s);
inline u32  reg_rd(u32 reg);
inline void reg_wr(u32 addr, u32 value);

int main(void)
{
	volatile unsigned long v;

	v = *(volatile unsigned long *)0xE000ED00;
	(void)v;
	v = *(volatile unsigned long *)0xE002ED00;
	(void)v;

	*(unsigned long *)(0x40004828) = '*';

	uart_puts("Hello World from non-secure app !\r\n");

	while(1);
	return(0);
}

/**
 * @brief Send a single byte to console UART
 *
 * @param c Byte to send
 */
void uart_putc(u8 c)
{
	while ((reg_rd(USART3 + 0x1C) & (1 << 7)) == 0)
		;
	reg_wr((USART3 + 0x28), c);
}

/**
 * @brief Send a text string to UART
 *
 * @param s Pointer to the string to send
 */
void uart_puts(char *s)
{
	while (*s)
	{
		uart_putc(*s);
		s++;
	}
}

/**
 * @brief Read the value of a 32 bits memory mapped register
 *
 * @param  reg Address of the register to read
 * @return u32 Value of the register (32 bits)
 */
inline u32 reg_rd(u32 reg)
{
	return( *(volatile u32 *)reg );
}

/**
 * @brief Write a 32 bits value to a memory mapped register
 *
 * @param reg   Address of the register to update
 * @param value New value to write into the register
 */
inline void reg_wr(u32 addr, u32 value)
{
	*(volatile u32 *)addr = value;
}

/* EOF */
