##
 # @file  Makefile
 # @brief Script to compile the "secure" part of the H5 firmware
 #
 # @author Saint-Genest Gwenael <gwen@cowlab.fr>
 # @copyright Agilack (c) 2023
 #
 # @page License
 # Cowkeyr-ac firmware is free software: you can redistribute it and/or
 # modify it under the terms of the GNU Lesser General Public License
 # version 3 as published by the Free Software Foundation. You should have
 # received a copy of the GNU Lesser General Public License along with this
 # program, see LICENSE.md file for more details.
 # This program is distributed WITHOUT ANY WARRANTY.
##
TARGET   ?= fw_secure
CROSS    ?= arm-none-eabi-
BUILDDIR ?= build
USE_SEC  ?= y

SRC  = hardware.c main.c
SRC += driver/spi.c driver/uart.c
SRC += log.c
ASRC = startup.s

CC = $(CROSS)gcc
OC = $(CROSS)objcopy
OD = $(CROSS)objdump
GDB = $(CROSS)gdb

CFLAGS  = -mcpu=cortex-m33 -mthumb -mcmse
CFLAGS += -Os -nostdlib -ffunction-sections
CFLAGS += -Wall -Wextra -Wconversion -pedantic
CFLAGS += -Isrc
CFLAGS += -g
LDFLAGS  = -Wl,-Map=$(TARGET).map,--cref,--gc-sections
LDFLAGS += -nostartfiles -static
#CFLAGS += -DPROTECT_CONSOLE

ifeq ($(USE_SEC), y)
	CFLAGS += -DRUN_SEC
	LDFLAGS += -Tsrc/linker_s.ld
else
	LDFLAGS += -Tsrc/linker_ns.ld
endif

COBJ = $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC))
AOBJ = $(patsubst %.s, $(BUILDDIR)/%.o, $(ASRC))

all: fw_s
fw_s: $(BUILDDIR) $(AOBJ) $(COBJ)
	@echo "  [LD] $(TARGET)"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET).elf $(AOBJ) $(COBJ)
	@echo "  [OC] $(TARGET).bin"
	@$(OC) -S $(TARGET).elf -O binary $(TARGET).bin
	@echo "  [OD] $(TARGET).dis"
	@$(OD) -D $(TARGET).elf > $(TARGET).dis
ifeq ($(USE_SEC), y)
	@echo "Build for TrustZone ENABLED"
else
	@echo "Build for TrustZone DISABLED"
endif

clean:
	@echo "  [RM] $(TARGET).*"
	@rm -f $(TARGET).elf $(TARGET).map $(TARGET).bin $(TARGET).dis
	@echo "  [RM] Temporary object (*.o)"
	@rm -f $(BUILDDIR)/driver/*.o
	@rm -f $(BUILDDIR)/*.o
	@echo "  [RM] Clean editor temporary files (*~) "
	@find -name "*~" -exec rm -f {} \;

$(BUILDDIR):
	@echo "  [MKDIR] $@"
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/driver

$(AOBJ) : $(BUILDDIR)/%.o : src/%.s
	@echo "  [AS] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(COBJ) : $(BUILDDIR)/%.o: src/%.c
	@echo "  [CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

debug:
	$(GDB) -q -ex "target extended-remote localhost:3333" $(TARGET).elf

load:
	$(GDB) -q -ex "set confirm off" \
	          -ex "target extended-remote localhost:3333" \
	          -ex "monitor reset halt" \
	          -ex "load" \
	          -ex "monitor reset run" \
	          -ex "quit" $(TARGET).elf
