/**
 * @file  spi.h
 * @brief Headers and definitions for STM32H5 SPI driver
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
#ifndef SPI_H
#define SPI_H

// SPI registers
#define SPI_CR1(x)     (x + 0x00)
#define SPI_CR2(x)     (x + 0x04)
#define SPI_CFG1(x)    (x + 0x08)
#define SPI_CFG2(x)    (x + 0x0C)
#define SPI_IER(x)     (x + 0x10)
#define SPI_SR(x)      (x + 0x14)
#define SPI_IFCR(x)    (x + 0x18)
#define SPI_TXDR(x)    (x + 0x20)
#define SPI_RXDR(x)    (x + 0x30)
#define SPI_CRCPOLY(x) (x + 0x40)
#define SPI_TXCRC(x)   (x + 0x44)
#define SPI_RXCRC(x)   (x + 0x48)
#define SPI_UDRDR(x)   (x + 0x4C)
#define SPI_I2SCFGR(x) (x + 0x50)

void spi_init(void);

#endif
