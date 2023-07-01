##
 # @file  Makefile
 # @brief Script to compile the "application" part of the firmware
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
TARGET   ?= fw_app
CROSS    ?= arm-none-eabi-
BUILDDIR ?= build

SRC  = main.c
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
LDFLAGS += -nostartfiles -static -Tsrc/linker.ld

COBJ = $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC))
AOBJ = $(patsubst %.s, $(BUILDDIR)/%.o, $(ASRC))

all: $(TARGET)

$(TARGET): $(BUILDDIR) $(AOBJ) $(COBJ)
	@echo "  [LD] $(TARGET)"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET).elf $(AOBJ) $(COBJ)
	@echo "  [OC] $(TARGET).bin"
	@$(OC) -S $(TARGET).elf -O binary $(TARGET).bin
	@echo "  [OD] $(TARGET).dis"
	@$(OD) -D $(TARGET).elf > $(TARGET).dis

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
