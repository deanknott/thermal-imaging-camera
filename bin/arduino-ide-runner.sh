#!/bin/bash
#
# arduino-ide-runner.sh -- run the ESP Arduino IDE

# standard locals
alias cd='builtin cd'
P="$0"
USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-e(rase flash)] [-S(taging)] [-H(EAD)] [-8 (1.8.9)] [-1 (1.8.1)] [-3 (1.8.3)] [-5 (1.8.5)] [-6 (1.8.6)] [-7 (1.8.7)] [-9 (1.9.x)]"
DBG=:
OPTIONSTRING=hdSHx:8135967
R='\033[0;31m' # red (use with echo -e)
G='\033[0;32m' # green
B='\033[1;34m' # blue
N='\033[0m'    # no color
e() { (
  C=""; case $1 in R|G|B|N) C=${!1}; shift; ;; esac; echo -e "${C}$*${N}";
) }

# specific locals
BASEDIR="$(cd `dirname ${P}`/.. && pwd)"
SDKS_DIR=${BASEDIR}/sdks
LIB_DIR=${SDKS_DIR}/Arduino

# specific locals
ERASE=
PORT='/dev/ttyUSB0'
#IDEHOME=~/esp-resources
IDEHOME=${SDKS_DIR}
IDEBASE=${IDEHOME}

# arduino IDE vars
PREFSDIR=~/.arduino15
SPREFSDIR=${IDEBASE}/dot-arduino15-staging
HPREFSDIR=${IDEBASE}/dot-arduino15-head
EIGHTPREFSDIR=${IDEBASE}/dot-arduino15-188
ONE8ONEPREFSDIR=${IDEBASE}/dot-arduino15-181
ONE83PREFSDIR=${IDEBASE}/dot-arduino15-183
ONE85PREFSDIR=${IDEBASE}/dot-arduino15-185
ONE86PREFSDIR=${IDEBASE}/dot-arduino15-186
ONE87PREFSDIR=${IDEBASE}/dot-arduino15-187
ONE9xPREFSDIR=${IDEBASE}/dot-arduino15-190-beta
X=
STAGING=
HEAD=
EIGHT=
ONE8ONE=
ONE83=
ONE85=
ONE86=
ONE87=
ONE9x=
STAGINGDIR=${IDEBASE}/arduino-1.6.5-r5
HEADDIR=${IDEBASE}/arduino-1.6.5-r5--local-esp
EIGHTDIR=${IDEBASE}/arduino-1.8.9
ONE8ONEDIR=${IDEBASE}/arduino-1.8.1
ONE83DIR=${IDEBASE}/arduino-1.8.3
ONE85DIR=${IDEBASE}/arduino-1.8.5
ONE86DIR=${IDEBASE}/arduino-1.8.6
ONE87DIR=${IDEBASE}/arduino-1.8.7
#ONE9xDIR=${IDEBASE}/arduino-PR-beta1.9-BUILD-107
ONE9xDIR=${IDEBASE}/arduino-PR-beta1.9-BUILD-*

# message & exit if exit num present
usage() { echo -e Usage: $USAGE; [ ! -z "$1" ] && exit $1; }

# process options
while getopts $OPTIONSTRING OPTION
do
  case $OPTION in
    h)	usage 0 ;;
    d)	DBG=echo ;;
    e)	ERASE=yes ;;
    S)	STAGING=yes ;;
    H)	HEAD=yes ;;
    8)	EIGHT=yes ;;
    1)	ONE8ONE=yes ;;
    3)	ONE83=yes ;;
    5)	ONE85=yes ;;
    6)	ONE86=yes ;;
    7)	ONE87=yes ;;
    9)	ONE9x=yes ;;
    x)	X="${OPTARG}" ;;
    *)	usage 1 ;;
  esac
done 
shift `expr $OPTIND - 1`

# main action branches
if [ x$ERASE == xyes ]
then
  [ -d Arduino ] || { echo "can't find Arduino dir from `pwd`"; exit 10; }
  echo running erase on $PORT ...
  echo Arduino/hardware/espressif/esp32/tools/esptool.py --port "${PORT}" erase_flash
  Arduino/hardware/espressif/esp32/tools/esptool.py --port "${PORT}" erase_flash
  exit 0
else
  # need to specify an IDE version to run
  if [ x$STAGING == x -a x$HEAD == x -a x$EIGHT == x -a x$ONE8ONE == x -a x$ONE83 == x -a x$ONE85 == x -a x$ONE86 == x -a x$ONE87 == x -a x$ONE9x == x ]
  then
    echo "you must choose staging or head or 8 or 1 or 3 or etc."
    usage 2
  fi
fi

# choose version
e G preparing to run Arduino IDE...
if [ x$STAGING == xyes ]
then
  USEPREFS=${SPREFSDIR}
  USEIDE=${STAGINGDIR}
elif [ x$HEAD == xyes ]
then
  USEPREFS=${HPREFSDIR}
  USEIDE=${HEADDIR}
elif [ x$EIGHT == xyes ]
then
  USEPREFS=${EIGHTPREFSDIR}
  USEIDE=${EIGHTDIR}
elif [ x$ONE8ONE == xyes ]
then
  USEPREFS=${ONE8ONEPREFSDIR}
  USEIDE=${ONE8ONEDIR}
elif [ x$ONE83 == xyes ]
then
  USEPREFS=${ONE83PREFSDIR}
  USEIDE=${ONE83DIR}
elif [ x$ONE85 == xyes ]
then
  USEPREFS=${ONE85PREFSDIR}
  USEIDE=${ONE85DIR}
elif [ x$ONE86 == xyes ]
then
  USEPREFS=${ONE86PREFSDIR}
  USEIDE=${ONE86DIR}
elif [ x$ONE87 == xyes ]
then
  USEPREFS=${ONE87PREFSDIR}
  USEIDE=${ONE87DIR}
elif [ x$ONE9x == xyes ]
then
  USEPREFS=${ONE9xPREFSDIR}
  USEIDE=${ONE9xDIR}
else
  echo 'erk!'
  usage 3
fi

# link the prefs dir if it doesn't exist
if [ -d $PREFSDIR ]
then
  mv $PREFSDIR $HOME/.saved-`basename $PREFSDIR`-$$
fi
if [ ! -e $PREFSDIR ]
then
  echo linking $USEPREFS to $PREFSDIR ...
  ( cd; ln -s $USEPREFS $PREFSDIR; )
fi

# the prefs dir should be a link to a version-specific dir
if [ ! -L $PREFSDIR ]
then
  echo $PREFSDIR should be a link
  ls -l $PREFSDIR
  usage 4
fi

# switch the link to the required version
rm $PREFSDIR
ln -s $USEPREFS $PREFSDIR
echo using $PREFSDIR: `ls -ld $PREFSDIR`

# run the IDE
cd $USEIDE
echo running arduino IDE from `pwd`
if [ -x ./arduino ] 
then
  #./arduino --pref sketchbook.path="${HOME}/iot/com3505/labs"
  echo running arduino from `pwd`...
  ./arduino
else 
  cd build
  echo doing ant run from `pwd`...
  ant run
fi
