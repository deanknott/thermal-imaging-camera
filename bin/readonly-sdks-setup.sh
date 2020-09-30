#!/bin/bash
# readonly-sdks-setup.sh
#
# script to setup an unphone tree with sdks workable readonly
#
# thanks to https://github.com/dobairoland for help via
# https://github.com/espressif/esp-idf/issues/4034

# pull down the unphone tree
git clone https://gitlab.com/hamishcunningham/unphone.git
cd unphone

# setup IDF; do the pip installs system-wide
make esp-support
make esp-pip-installs-system 

# set up the Arduino IDE
make arduino-libs
make setup-arduino-ide 
# you can test the IDE by: make arduino-ide

# make sure the IDF is working and config kit compiled
( cd sdks/esp-idf/tools/kconfig && make )
cd sdks/esp-idf/examples/get-started/hello_world/
make menuconfig
make

# now copy to your installation dir, e.g.:
# cd /opt
# sudo cp -a ~/unphone .
