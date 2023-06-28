/**
 * @file  log.c
 * @brief This module is used to handle and dispach log messages (console)
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
#include "log.h"
#include "types.h"

static uint log_level;

/**
 * @brief Initialize log module
 *
 */
void log_init(void)
{
	log_level = 5;
}

/**
 * @brief Write a VT100 escape sequence to change font color
 *
 * @param code Color to set (0 for default) (see standard ansi colors)
 */
void log_color(int code)
{
	char *str = 0;
	switch(code)
	{
		case 0: str="\x1B[0m";  break;
		case LOG_RED: str="\x1B[31m"; break; // Red
		case LOG_GRN: str="\x1B[32m"; break; // Green
		case LOG_YLW: str="\x1B[33m"; break; // Yellow
		case LOG_BLU: str="\x1B[34m"; break; // Blue
		case LOG_MTA: str="\x1B[35m"; break; // Magenta
		case LOG_CYN: str="\x1B[36m"; break; // Cyan
		case LOG_WTE: str="\x1B[37m"; break; // White

		case LOG_BGRY: str="\x1B[1;30m"; break; // Grey (bright dark)
		case LOG_BRED: str="\x1B[1;31m"; break; // Bright Red
		case LOG_BGRN: str="\x1B[1;32m"; break; // Bright Green
		case LOG_BYLW: str="\x1B[1;33m"; break; // Bright Yellow
		case LOG_BBLU: str="\x1B[1;34m"; break; // Bright Blue
		case LOG_BMTA: str="\x1B[1;35m"; break; // Bright Magenta
		case LOG_BCYN: str="\x1B[1;36m"; break; // Bright Cyan
		case LOG_BWTE: str="\x1B[1;37m"; break; // Bright White
	}
	if (str == 0)
		return;

	while(*str)
		log_putc(*str++);
}

/**
 * @brief Dump the content of a buffer
 *
 * @param data  Pointer to the buffer to dump
 * @param count Number of bytes to dump
 * @param flags Formating options, 1: absolute address, 2: relative address
 */
void log_dump(const u8 *data, uint count, uint flags)
{
	const u8 *origin = data;
	int i;

	while (count)
	{
		/* Print line header with absolute address */
		if (flags & 1)
			log_print(0, "%32x ", (u32)data);
		/* Print line header with relative address */
		else if (flags & 2)
			log_print(0, "%32x ", (u32)(data - origin));

		for (i = 0; i < 16; i++)
		{
			log_puthex(*data, 8);
			count--;
			if (count == 0)
				break;
			data ++;
			log_putc(' ');
		}
		log_putc('\n');
	}
}

/**
 * @brief Log a formated string with arguments
 *
 * This function log a message with optional formating arguments (like or
 * inspired by printf command). The standard %%, %d, %s, %x and also some
 * non-standard argument for terminal escape sequences (colors) %{ and %}.
 *
 * @param level Level of importance of the message to log
 * @param fmt   String to log with optional formating
 * @param ...   According to formating, optional arguments
 */
void log_print(uint level, const char *fmt, ...)
{
	int modifier;
#ifdef __GNUC__
	__builtin_va_list args;
#endif

	/* This message should be log according to current log level */
	if (level > log_level)
		return;

#ifdef __GNUC__
	__builtin_va_start(args, fmt);
#endif

	while(*fmt)
	{
		/* Reencode newline with carriage return */
		if (*fmt == '\n')
		{
			log_putc('\r');
			log_putc('\n');
		}
		else if (*fmt != '%')
			log_putc(*fmt);
		else
		{
			fmt++;
			/* Extract format modifier (if any) */
			modifier = 0;
			while( (*fmt >= '0') && (*fmt <= '9'))
			{
				modifier = (modifier * 10);
				modifier += (*fmt - '0');
				fmt++;
			}
			switch(*fmt)
			{
				/* Insert a percent character */
				case '%':
					log_putc('%');
					break;
#ifdef __GNUC__
				/* Insert a single character */
				case 'c':
				{
					uint c;
					c = __builtin_va_arg(args, uint);
					log_putc((uint)(c & 0xFF));
					break;
				}
				/* Insert a decimal integer */
				case 'd':
				{
					int value;
					value = __builtin_va_arg(args, int);
					log_putdec((u32)value, 1, modifier);
					break;
				}
				/* Insert a text string */
				case 's':
				{
					char *str;
					str = __builtin_va_arg(args, char*);
					uart_puts(str);	
					break;
				}
				/* Insert an unsigned decimal integer */
				case 'u':
				{
					uint value;
					value = __builtin_va_arg(args, uint);
					log_putdec(value, 0, modifier);
					break;
				}
				/* Insert an hexadecimal value */
				case 'x':
				{
					uint value;
					value = __builtin_va_arg(args, uint);
					log_puthex(value, modifier);
					break;
				}

				/* Uncommon: Color change */
				case '{':
				{
					int code;
					code = __builtin_va_arg(args, int);
					log_color(code);
					break;
				}
#else
				case '{':
					break;
#endif
				/* Uncommon: Use default color */
				case '}':
					log_color(0);
					break;
				/* Unknown format, log it "as is" */
				default:
					log_putc('%');
					log_putc(*fmt);
					break;
			}
		}
		fmt++;
	}

#ifdef __GNUC__
	__builtin_va_end(args);
#endif
}

/**
 * @brief Log a single byte
 *
 * @param c Byte to send
 */
void log_putc(const char c)
{
	uart_putc(c);
}

/**
 * @brief Convert and write an integer value to his decimal representation
 *
 * @param n Interger value to convert
 * @param sign True if n is a signed integer, False to decode n as unsigned
 * @param pad Align output value to ad least "pad" digits
 */
void log_putdec(uint n, int sign, int pad)
{
	unsigned int decade = 1000000000;
	int count = 0;
	int i;

	if (sign && (n & (uint)(1 << 31)) )
	{
		/* Log minus sign */
		log_putc('-');
		/* Reencode to positive value */
		i = 0 - (int)n;
		n = (uint)i;
	}

	for (i = 0; i < 9; i++)
	{
		if ((n > (decade - 1)) || count || (pad >= (10-i)))
		{
			log_putc( (u8)(n / decade) + '0' );
			n -= ((n / decade) * decade);
			count++;
		}
		decade = (decade / 10);
	}
	log_putc( (u8)(n + '0') );
	count ++;
}

/**
 * @brief Send the hexadecimal representation of an integer
 *
 * @param c Binary word (32 bits) to show as hex
 * @param len Number of bits to decode
 */
void log_puthex(const u32 c, const int len)
{
	const u8 hex[16] = "0123456789ABCDEF";
	u8 code;
	int flag = 0;
	int i;

	for (i = 28; i >= 0; i -= 4)
	{
		/* Extract value of next 4-bits */
		code = ((c >> i) & 0xF);
		/* Test if the digit must be printed */
		if ((len > i) || (code > 0) || flag)
		{
			log_putc( hex[code] );
			flag = 1;
		}
	}
}

/**
 * @brief Log a text string
 *
 * @param level Level of importance of the message to log
 * @param s Pointer to the string to log
 */
void log_puts (uint level, const char *s)
{
	/* This message should be log according to current log level */
	if (level > log_level)
		return;

	while (*s)
	{
		if (*s == '\n')
		{
			log_putc('\r');
			log_putc('\n');
		}
		else
			log_putc(*s);
		s++;
	}
}

/* EOF */
