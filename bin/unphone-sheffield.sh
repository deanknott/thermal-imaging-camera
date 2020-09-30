#!/bin/bash
# unphone-sheffield.sh
#
# install unphone (relative to a master copy in /opt) in student home dir
#
# /opt/unphone should be installed/setup as normal, but with the addition of:
# cd /opt/unphone/sdks/esp-idf/tools/kconfig && sudo make
#
# hamish, 15/9/2019

# pull down the master tree from gitlab (using clone to enable future updates;
# works ok without authorisation as this is a public repo)
cd
git clone https://gitlab.com/hamishcunningham/unphone.git

# curl-based alternative:
#curl -O https://gitlab.com/hamishcunningham/unphone/-/archive/master/unphone-master.tar.gz
#tar xzf unphone-master.tar.gz
#rm unphone-master.tar.gz
#mv unphone-master unphone

# set up an example private.h
[ -f private.h ] || ( \
  curl -s https://gitlab.com/hamishcunningham/unphone/raw/master/README.mkd \
  |sed -n '/^\/\/ private.h$/,/```/p' |head -n -1 > private.h )

# set up the paths, and add to bashrc if not already there
export IDF_PATH=`pwd`/unphone/sdks/esp-idf
export PATH=`pwd`/unphone/sdks/arduino-esp32/tools/xtensa-esp32-elf/bin:${PATH}
grep -sq '# unphone support' .bashrc || \
  (echo; echo '# unphone support') >>~/.bashrc
grep -sq IDF_PATH .bashrc || \
  echo "export IDF_PATH=`pwd`/unphone/sdks/esp-idf" >>~/.bashrc
grep -sq xtensa-esp32 .bashrc || \
  echo "export PATH=`pwd`/unphone/sdks/arduino-esp32/tools/xtensa-esp32-elf/bin:"'${PATH}' >>~/.bashrc

# configure the local unphone tree
cd unphone
make setup

# to run the IDE: make arduino-ide
