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
CROSS    ?= arm-none-eabi-

GDB = $(CROSS)gdb

all: fw_secure fw_app

fw_secure:
	@echo ""
	@echo "  --=={ Compile secure component }==--"
	@echo ""
	make -C main_secure

fw_app:
	@echo ""
	@echo "  --=={ Compile application component }==--"
	@echo ""
	make -C main_app

clean:
	make -C main_secure clean
	make -C main_app clean

config:
	$(GDB) -q --command=scripts/mcu_config.gdb

debug:
	$(GDB) -q --command=scripts/gdb.cfg

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
st_prov:
	@echo ""
	$(GDB) -q --command=scripts/mcu_config.gdb -ex "set_st 1" -ex "quit"
st_closed:
	@echo ""
	$(GDB) -q --command=scripts/mcu_config.gdb -ex "set_st 4" -ex "quit"
