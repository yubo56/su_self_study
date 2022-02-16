# NB: json is no longer necessary maybe?
KEEBDIR=~/qmk_firmware/keyboards/preonic
CURRDIR=$(shell pwd)/

main:
	rm -f $(KEEBDIR)/config.h $(KEEBDIR)/rev3/rules.mk
	ln -s $$(pwd)/yubo_preonic_config.h $(KEEBDIR)/config.h
	ln -s $$(pwd)/yubo_preonic_rules.mk $(KEEBDIR)/rev3/rules.mk

flash:
	qmk flash yubo_preonic_qmk.json
