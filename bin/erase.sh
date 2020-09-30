#!/bin/bash
# unPhone firmware erase script

SHELL=/bin/bash
ECHO='echo -e "\033[0;32m"'
ECHON='echo -e "\033[0m"'
unset CDPATH # don't echo directories when cd'ing

ESPPORT=$([ ! -z "$ESPPORT" ] && echo $ESPPORT || \
  for p in ttyUSB0 ttyUSB1 cu.SLAB_USBtoUART; do \
  [ -r /dev/${p} ] && echo /dev/${p} && break; done)

SDKS_DIR=sdks
OPT_SDKS=/opt/unphone/sdks
ARD_LIB_DIR=$SDKS_DIR/Arduino/libraries
ARD_HW_DIR=$SDKS_DIR/Arduino/hardware/espressif

[ -d $OPT_SDKS ] || OPT_SDKS=/resources/hc/unphone/sdks
[ -d $OPT_SDKS ] || OPT_SDKS=~/unphone/sdks

# erase
python3 \
        $OPT_SDKS/esp-idf/components/esptool_py/esptool/esptool.py \
        --chip esp32 --port $ESPPORT erase_flash
