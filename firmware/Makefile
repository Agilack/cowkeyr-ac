##
 # @file  Makefile
 # @brief Script to compile sources and create flat binary using "make" program
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
TARGET   ?= fw_hello
CROSS    ?= arm-none-eabi-
BUILDDIR ?= build

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
CFLAGS += -DRUN_SEC -DTEST_UNSAFE -g
LDFLAGS  = -Wl,-Map=$(TARGET).map,--cref,--gc-sections
LDFLAGS += -nostartfiles -Tsrc/linker.ld -static

COBJ = $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC))
AOBJ = $(patsubst %.s, $(BUILDDIR)/%.o, $(ASRC))

all: fw_s fw_ns
fw_s: $(BUILDDIR) $(AOBJ) $(COBJ)
	@echo "  [LD] $(TARGET)"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET).elf $(AOBJ) $(COBJ)
	@echo "  [OC] $(TARGET).bin"
	@$(OC) -S $(TARGET).elf -O binary $(TARGET).bin
	@echo "  [OD] $(TARGET).dis"
	@$(OD) -D $(TARGET).elf > $(TARGET).dis

fw_ns:
	@echo " ----===={ NonSecure }====----"
	mkdir -p build_ns
	@echo "  [CC] src_ns/startup.s"
	@$(CC) $(CFLAGS) -c src_ns/startup.s -o build_ns/startup.o
	@echo "  [CC] src_ns/main.c"
	@$(CC) $(CFLAGS) -c src_ns/main.c -o build_ns/main.o
	@echo "  [LD] fw_ns.elf"
	@$(CC) $(CFLAGS) -Wl,-Map=$(TARGET).map,--cref,--gc-sections -nostartfiles -Tsrc_ns/linker.ld -static -o fw_ns.elf build_ns/startup.o build_ns/main.o
	@echo "  [OC] fw_ns.bin"
	@$(OC) -S fw_ns.elf -O binary fw_ns.bin
	@echo "  [OD] fw_ns.dis"
	@$(OD) -D fw_ns.elf > fw_ns.dis

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
	$(GDB) -q --command=scripts/gdb.cfg $(TARGET).elf
tz_en:
	@echo ""
	@echo "This command will enable TrustZone (TZEN) ! You have 5 second to abort (ctrl-c)"
	@sleep 5
	$(GDB) -q --command=scripts/mcu_config.gdb -ex "set_tz 1" -ex "quit"
tz_dis:
	@echo ""
	@echo "This command will disable TrustZone (TZEN) ! You have 5 second to abort (ctrl-c)"
	@sleep 5
	$(GDB) -q --command=scripts/mcu_config.gdb -ex "set_tz 0" -ex "quit"
