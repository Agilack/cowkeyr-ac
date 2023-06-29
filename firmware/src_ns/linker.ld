/**
 * @file linker.ld
 * @brief Linker script for Cowkeyr-AC
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
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(SRAM2) + LENGTH(SRAM2); /* end of "RAM" Ram type memory */

_Min_Heap_Size = 0x4000;  /* required amount of heap */
_Min_Stack_Size = 0x4000; /* required amount of stack */

/* Memories definition */
MEMORY
{
	FLASH  (rx) : ORIGIN = 0x08100000, LENGTH = 128K /* 2048K */
	SRAM1 (xrw) : ORIGIN = 0x20000000, LENGTH = 256K
	SRAM2 (xrw) : ORIGIN = 0x20040000, LENGTH = 64K
	SRAM3 (xrw) : ORIGIN = 0x20050000, LENGTH = 320K
}

/* Sections */
SECTIONS
{
	.text :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))
		KEEP(*(.fw_version))
		*(.text)
		*(.text*)
		*(.glue_7)
		*(.glue_7t)
		*(.eh_frame)

		KEEP (*(.init))
		KEEP (*(.fini))

		. = ALIGN(4);
		_etext = .;
	} >FLASH


	.rodata :
	{
		. = ALIGN(4);
		*(.rodata)
		*(.rodata*)
		. = ALIGN(4);
	} >FLASH

	.preinit_array :
	{
		. = ALIGN(4);
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);
		. = ALIGN(4);
	} >FLASH

	.init_array :
	{
		. = ALIGN(4);
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
		. = ALIGN(4);
	} >FLASH

	.fini_array :
	{
		. = ALIGN(4);
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array*))
		PROVIDE_HIDDEN (__fini_array_end = .);
		. = ALIGN(4);
	} >FLASH

	.ARM.extab :
	{
		. = ALIGN(4);
		*(.ARM.extab* .gnu.linkonce.armextab.*)
		. = ALIGN(4);
	} >FLASH
	.ARM :
	{
		. = ALIGN(4);
		__exidx_start = .;
		*(.ARM.exidx*)
		__exidx_end = .;
		. = ALIGN(4);
	} >FLASH

	.ethernet_data :
	{
		PROVIDE_HIDDEN (__ethernet_data_start = .);
		KEEP (*(SORT(.ethernet_data.*)))
		KEEP (*(.ethernet_data*))
		PROVIDE_HIDDEN (__ethernet_data_end = .);
	} >SRAM2

	/* Used by the startup to initialize data */
	_sidata = LOADADDR(.data);

	/* Initialized data sections into "RAM" Ram type memory */
	.data :
	{
		. = ALIGN(4);
		__data_start__ = .;
		*(.data) /* .data sections */
		*(.data*) /* .data* sections */
		*(.RamFunc) /* .RamFunc sections */
		*(.RamFunc*) /* .RamFunc* sections */
		. = ALIGN(4);
		__data_end__ = .;
	} >SRAM2 AT> FLASH

	/* Uninitialized data section into "RAM" Ram type memory */
	. = ALIGN(4);
	.bss :
	{
		_sbss = .;
		__bss_start__ = _sbss;
		*(.bss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
		__bss_end__ = _ebss;
	} >SRAM2

	/* User_heap_stack section, used to check that there is enough "RAM" Ram type memory left */
	._user_heap_stack :
	{
		. = ALIGN(8);
		PROVIDE ( end = . );
		PROVIDE ( _end = . );
		. = . + _Min_Heap_Size;
		. = . + _Min_Stack_Size;
		. = ALIGN(8);
	} >SRAM2

	/* Remove information from the compiler libraries */
	/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}

	.ARM.attributes 0 :
	{
		*(.ARM.attributes)
	}
}
