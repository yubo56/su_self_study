# SETUP

NB: needs avr-gcc 11 or lower to work apparently

Note: use `qmk setup zsa/qmk_firmware -b firmware22` to get the zsa firmware
(`https://github.com/zsa/qmk_firmware/`), and build against that using the usual
`qmk flash -kb ergodox_ez -km yubo`
- copy `keymap.c` into `~/qmk_firmware/keyboards/ergodox_ez/keymaps/yubo`
