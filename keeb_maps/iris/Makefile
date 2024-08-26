IRISDIR=~/qmk_firmware/keyboards/keebio/iris
CURRDIR=$(shell pwd)/

default:
	@echo "None"

flash:
	qmk flash -kb keebio/iris/rev6 -km yubo

fresh_setup:
	mkdir -p $(IRISDIR)/keymaps/yubo
	rm -f $(IRISDIR)/rev6/config.h $(IRISDIR)/keymaps/yubo/keymap.c $(IRISDIR)/rev6/rules.mk
	cp $(IRISDIR)/keymaps/default/keymap.c $(IRISDIR)/keymaps/yubo/keymap.c
	ln -s $$(pwd)/yubo_iris6_qmk.c $(IRISDIR)/keymaps/yubo/keymap.c
	ln -s $$(pwd)/yubo_iris6_config.h $(IRISDIR)/rev6/config.h
	ln -s $$(pwd)/yubo_iris6_rules.mk $(IRISDIR)/rev6/rules.mk
