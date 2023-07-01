##
 # @file  scripts/mcu_config.gdb
 # @brief A GDB script to access or modify MCU state and option bytes
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

##
 # @brief Unlock FLASH_OPTCR before modifying *_PRG register
 #
 # This function use the unlock sequence described into RM0481
 #
define optcr_unlock
	set $optcr  = (*(unsigned long *)0x4002201C & 0x01)
	printf "  OPTCR is currently"
	if $optcr != 1
		printf "%c[01;32m Unlock %c[0m (good)\n", 0x1B, 0x1B
	end
	if $optcr == 1
		printf "%c[01;31m Locked %c[0m (try to unlock it)\n", 0x1B, 0x1B

		# First key is 0x08192A3B
		set *(unsigned long *)0x4002200C = 0x08192A3B
		# Second key is 0x4C5D6E7F
		set *(unsigned long *)0x4002200C = 0x4C5D6E7F
	end
end

##
 # @brief Modify the flash watermark configuration
 #
 # @param $arg0 Bank1 first secure sector number (0 > 127)
 # @param $arg1 Bank1 last secure sector number (0 > 127)
 #
define set_fl_wm
	set $bank1_wm = *(unsigned long *)0x400220E4
	set $bank2_wm = *(unsigned long *)0x400221E4
	printf "Current Flash wartermark configuration :\n"
	printf "  - Bank 1 start=%x end=%x\n", ($bank1_wm & 0xFF), (($bank1_wm >> 16) & 0xFF)
	printf "  - Bank 2 start=%x end=%x\n", ($bank2_wm & 0xFF), (($bank2_wm >> 16) & 0xFF)

	# If arguments are present: modify watermark configuration
	if ($argc == 2)
		optcr_unlock
		# Compute new value
		set $sect_start = $arg0
		set $sect_end   = $arg1
		set $new_wm = ($sect_end << 16) | ($sect_start << 0)
		# Set this new value back to register
		set *(unsigned long *)0x400220E4 = $new_wm
		# Set OPTSTRT to apply changes
		set *(unsigned long *)0x4002201C = 0x00000002
		# Wait end of operation
		while (*(unsigned long *)0x4002201C & 2)
		end
		set *(unsigned long *)0x4002201C = 0x00000001
		# Verify
		set $wm = *(unsigned long *)0x400220E4
		printf "  new value %c[01;33m%X%c[0m\n", 0x1B, $wm, 0x1B
	end
	if ($argc > 0) && ($argc != 2)
		printf "\nUsage: set_fl_wm <start_sect> <end_sect>\n"
	end
end

##
 # @brief Modify the PRODUCT_STATE register - DANGEROUS
 #
define set_st
	set $optsr = *(unsigned long *)0x40022054
	set $st_cur = (($optsr >> 8) & 0xFF)
	set $new_state = 0
	printf "  PRODUCT_STATE current value %c[01;33m%X%c[0m\n", 0x1B, $st_cur, 0x1B

	if ($argc > 0)
		if $arg0 == 1
			set $new_state = 0x17
			printf "    - Switch to PROVISIONING (0x17)\n"
		end
		if $arg0 == 2
			set $new_state = 0x2E
			printf "    - Switch to iRoT-Provisioned (0x2E)\n"
		end
		if $arg0 == 3
			set $new_state = 0xC6
			printf "    - Switch to TZ-Closed (0xC6)\n"
		end
		if $arg0 == 4
			set $new_state = 0x72
			printf "    - Switch to CLOSED (0x72)\n"
		end
	end
	if ($new_state > 0)
		# Unlock OPTCR before modifying *_PRG register
		optcr_unlock
		# Compute new value of SR with PRODUCT_STATE
		set $newsr = ($optsr & 0xFFFF00FF)
		set $newsr = ($newsr | ($new_state << 8))
		# Set this new value back to OPTSR
		set *(unsigned long *)0x40022054 = $newsr
		# Set OPTSTRT to apply changes
		set *(unsigned long *)0x4002201C = 0x00000002
		# Wait end of operation
		while (*(unsigned long *)0x4002201C & 2)
		end
		set *(unsigned long *)0x4002201C = 0x00000001
		# Verify
		set $st_cur = ((*(unsigned long *)0x40022054 >> 8) & 0xFF)
		printf "  PRODUCT_STATE new value %c[01;33m%X%c[0m\n", 0x1B, $st_cur, 0x1B
	end
end

##
 # @brief Enable or disable TrustZone (modify TZEN value)
 #
define set_tz
	set $optsr2 =  *(unsigned long *)0x40022070

	set $tzen = (($optsr2 >> 24) & 0xFF)
	printf "  TZEN current value %c[01;33m%X%c[0m\n", 0x1B, $tzen, 0x1B

	# If request is to ENABLE TrustZone
	if ($argc > 0) && ($arg0 == 1)
		if ($tzen == 0xB4)
			printf "%c[01;32mTrustZone is already active :)%c[0m\n\n", 0x1B, 0x1B
		end
		# If TrustZone is currently disable
		if ($tzen == 0xC3)
			# Unlock OPTCR before modifying *_PRG register
			optcr_unlock

			# Compute new value of SR2 with TZEN=B4
			set $newsr2 = ($optsr2 & 0x00FFFFFF)
			set $newsr2 = ($newsr2 | 0xB4000000)
			# Set this new value back to OPTSR2
			set *(unsigned long *)0x40022074 = $newsr2
			# Set OPTSTRT to apply changes
			set *(unsigned long *)0x4002201C = 0x00000002
			# Wait end of operation
			while (*(unsigned long *)0x4002201C & 2)
			end
			set *(unsigned long *)0x4002201C = 0x00000001
			# Verify
			set $tzen = ((*(unsigned long *)0x40022070 >> 24) & 0xFF)
			if ($tzen == 0xB4)
				printf "  TrustZone is now %c[01;32mENABLE%c[0m\n", 0x1B, 0x1B
			end
		end
		if ($tzen != 0xB4) && ($tzen != 0xC3)
			printf "Error: Current value of TZEN is not supported\n"
		end
	end

	# If request is to DISABLE TrustZone
	if ($argc > 0) && ($arg0 == 0)
		if ($tzen == 0xC3)
			printf "%c[01;32mTrustZone is already disabled :)%c[0m\n\n", 0x1B, 0x1B
		end
		# If TrustZone is currently enabled
		if ($tzen == 0xB4)
			# Unlock OPTCR before modifying *_PRG register
			optcr_unlock

			# Compute new value of SR2 with TZEN=C3
			set $newsr2 = ($optsr2 & 0x00FFFFFF)
			set $newsr2 = ($newsr2 | 0xC3000000)
			# Set this new value back to OPTSR2
			set *(unsigned long *)0x40022074 = $newsr2
			# Set OPTSTRT to apply changes
			set *(unsigned long *)0x4002201C = 0x00000002
			# Wait end of operation
			while (*(unsigned long *)0x4002201C & 2)
			end
			set *(unsigned long *)0x4002201C = 0x00000001
			# Verify
			set $tzen = ((*(unsigned long *)0x40022070 >> 24) & 0xFF)
			if ($tzen == 0xC3)
				printf "  TrustZone is now %c[01;31mDISABLED%c[0m\n", 0x1B, 0x1B
			end
		end
		if ($tzen != 0xB4) && ($tzen != 0xC3)
			printf "Error: Current value of TZEN is not supported\n"
		end
	end

	if $argc == 0
		printf "\nMissing argument ! Usage: set_tz <mode> (where mode=1 to enable and mode=0 to disable)\n\n"
	end
end

set confirm off
target extended-remote localhost:3333
