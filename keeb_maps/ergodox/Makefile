# NB: json is no longer necessary maybe?
DIR=~/qmk_firmware/keyboards/ergodox_ez

setup:
	rm -f $(DIR)/config.h $(DIR)/rules.mk
	ln -s $$(pwd)/yubo_ergodox_config.h $(DIR)/config.h
	ln -s $$(pwd)/yubo_ergodox_rules.mk $(DIR)/rules.mk

flash:
	qmk compile yubo_ergodox.json
	teensy-loader-cli --mcu=atmega32u4 -w ~/qmk_firmware/ergodox_ez_yubo_ergodox.hex
