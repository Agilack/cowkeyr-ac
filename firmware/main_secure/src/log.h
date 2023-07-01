/**
 * @file  log.h
 * @brief Definitions and prototypes for log module
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
#ifndef LOG_H
#define LOG_H
#include "types.h"

/* Levels of logs */
#define LOG_ERR 1
#define LOG_WRN 2
#define LOG_INF 3
#define LOG_VIF 4
#define LOG_DBG 5
/* Colors */
typedef enum
{
	LOG_NOCOLOR,
	// Basic colors
	LOG_RED, LOG_GRN, LOG_YLW, LOG_BLU,
	LOG_MTA, LOG_CYN, LOG_WTE,
	// Bright colors
	LOG_BGRY,
	LOG_BRED, LOG_BGRN, LOG_BYLW, LOG_BBLU,
	LOG_BMTA, LOG_BCYN, LOG_BWTE
} log_color_t;

void log_init(void);
void log_putc(const char c);

/* Log data with atomic types */
void log_color (int code);
void log_putdec(uint n, int sign, int pad);
void log_puthex(const u32 c, const int len);
void log_puts  (uint level, const char *s);

/* Log structured contents */
void log_dump(const u8 *data, uint count, uint flags);
void log_print(uint level, const char *s, ...);

#endif