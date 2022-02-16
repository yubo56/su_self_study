# NB: json is no longer necessary maybe?
IRISDIR=~/qmk_firmware/keyboards/keebio/iris/
CURRDIR=$(shell pwd)/

main:
	mkdir -p $(IRISDIR)/keymaps/yubo
	rm -f $(IRISDIR)/rev6/config.h $(IRISDIR)/keymaps/yubo/keymap.c $(IRISDIR)/rev6/rules.mk
	ln -s $$(pwd)/yubo_iris6_qmk.c $(IRISDIR)/keymaps/yubo/keymap.c
	ln -s $$(pwd)/yubo_iris6_config.h $(IRISDIR)/rev6/config.h
	ln -s $$(pwd)/yubo_iris6_rules.mk $(IRISDIR)/rev6/rules.mk

flash:
	qmk flash -kb keebio/iris/rev6 -km yubo

reset:
	cp $(IRISDIR)/keymaps/default/keymap.c $(IRISDIR)/keymaps/yubo/keymap.c
