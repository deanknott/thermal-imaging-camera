#!/bin/bash
# listen on ESPPORT

SHELL=/bin/bash
unset CDPATH # don't echo directories when cd'ing

ESPPORT=$([ ! -z "$ESPPORT" ] && echo $ESPPORT || \
  for p in ttyUSB0 ttyUSB1 cu.SLAB_USBtoUART; do \
  [ -r /dev/${p} ] && echo /dev/${p} && break; done)

[ -z "$ESPPORT" ] && {
  echo 'oops: no ESSPORT set, perhaps no device plugged in?'
  exit 0
}

echo "listening on port $ESPPORT; to quit try Cntrl+A "
sleep 2
screen $ESPPORT 115200
