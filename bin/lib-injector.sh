#!/bin/bash
# lib-injector.sh

# standard locals
alias cd='builtin cd'
P="$0"
# the following should have && pwd but for some reason it echos the dir anyhow
BASEDIR="$(builtin cd `dirname ${P}`/.. && pwd)"

USAGE="`basename ${P}` [-h(elp)] [-d(ebug)] [-u(pdate libs)]
create sdks/Arduino with libraries adjusted for unPhone"
DBG=:
UPD=n
OPTIONSTRING=hdu
R='\033[0;31m' # red (use with echo -e)
G='\033[0;32m' # green
B='\033[1;34m' # blue
N='\033[0m'    # no color

# specific locals
SDKS_DIR=${BASEDIR}/sdks
LIB_DIR=${SDKS_DIR}/Arduino/libraries
CANONICAL=${BASEDIR}/lib
INCOMING=${BASEDIR}/incoming-libs
INJECT_CODE="${BASEDIR}/bin/lib-injector.cpp"
LIBS_TO_PATCH=
UNPH_LIB=unPhone_Library
LIBS_LIST_FILE=${BASEDIR}/bin/lib-repos.sh

# message & exit if exit num present
e() { (
  C=""; case $1 in R|G|B|N) C=${!1}; shift; ;; esac; echo -e "${C}$*${N}";
) }
usage() { e G "Usage: $USAGE"; [ ! -z "$1" ] && exit $1; }

# process options
while getopts $OPTIONSTRING OPTION
do
  case $OPTION in
    h)	usage 0 ;;
    d)	DBG=echo ;;
    u)	UPD=y ;;
    *)	usage 1 ;;
  esac
done 
shift `expr $OPTIND - 1`

# if we're updating create incoming-libs and update unPhone_Library from src
if [ x$UPD == xy ]
then
  [ ! -d $INCOMING ] && mkdir $INCOMING
  LIB_DIR=$INCOMING
  cp -a src $INCOMING/$UNPH_LIB
fi

# update the libraries (copying in those in the ESP32 core)
cd $LIB_DIR
e G copying src...
[ -d $UNPH_LIB ] || mkdir -p $UNPH_LIB  # unPhone's own...
cp -a $BASEDIR/src/* $UNPH_LIB          # ...lib code
OIFS="$IFS"; IFS="
"
for LIB_URL in `grep -v '^[ ]*#' $LIBS_LIST_FILE |sed 's,#.*,,'`
do
  IFS="$OIFS"
  REPO=`echo $LIB_URL |sed -e 's,.*\/,,' -e 's,\.git,,' -e 's,[ ]*patch,,'`;

  # only inject those flagged "patch" in lib-repos.sh
  set $LIB_URL
  LIB_URL=$1 # throw away "patch" flags
  [ x$2 == xpatch ] && LIBS_TO_PATCH="${LIBS_TO_PATCH} $REPO"

  e B "cloning/copying or updating repo $REPO at $LIB_URL..."
  if [ x$UPD == xn ]
  then
    # done in Makefile now cp -a $CANONICAL/$REPO $LIB_DIR
    continue
  elif [ ! -d $REPO ]
  then
    if [[ $LIB_URL == https* ]] # a lib in a git repo, needs clone
    then 
      git clone $LIB_URL
    else                        # an ESP32 core lib, or cache, needs copy
      cp -a $CANONICAL/$LIB_URL .
    fi
  elif [[ $LIB_URL == https* ]] # a lib in a git repo, existing
  then
    (builtin cd $REPO && git pull)
  else
    e B "keeping existing version of $LIB_URL, delete and re-run to update"
  fi
done
cd $BASEDIR

# if we're not updating we're done
if [ x$UPD == xn ]
then
  e G "done"
  exit 0
fi

# find and inject source files that use digitalWrite/Read or pinMode
cd $LIB_DIR
e R doing injection on $LIBS_TO_PATCH ...
for d in $LIBS_TO_PATCH
do
  (
    cd $d
    for f in `find . -type f |grep -v '\/examples\/' |egrep '\.cpp'`
    do
      if egrep -q 'digitalWrite|digitalRead|pinMode' $f
      then
        # this library file twiddles pins so we need to inject IOExpander stuff
        e G injecting unPhone library into $B $d/$f
        if grep -q IOExpander $f
        then
          # the expander's already injected, don't do owt
          e B $f $G already contains IOExpander
        else
          # prepend the IOExpander stuff
          ( echo "// `basename ${f}`"; cat ${INJECT_CODE}; cat ${f}; ) >$$.tmp
#         # inject the IOExpander stuff after the last #include in the lib file
#         ( tac $f |sed -n '/#include/,$p' |tac; cat "${INJECT_CODE}"; \
#           tac $f |sed -n '1,/#include/p' |tac |tail -n +2; ) >$$.tmp
          mv $$.tmp $f
          e G "done $B $f"
        fi
      fi
    done
  )
done


# make dirty hacks to certain files while no one is looking...

# several HX8357 hacks...
e B munging the LCD library...
LCD_LIB=Adafruit_HX8357_Library
#
# the LCD is reversed by default; this hack fixes:
CHANGES=0
for p in `egrep -n '^[ ]*0xC0,$' \
  ${LCD_LIB}/Adafruit_HX8357.cpp |sed 's,:.*$,,g'`
do
  # change the library file; old line numbers were: 149|204
  case $p in
    226|171) CHANGES=$((CHANGES + 1)); sed -i \
      "${p}s/0xC0,/0x88, \/\/ modified (from 0xC0) for unPhone spin 4+/" \
      ${LCD_LIB}/Adafruit_HX8357.cpp ;;
  esac
done
if [ $CHANGES == 2 ]
then
  e B 'rewrote two MADCTL 0xC0 lines'
else
  e B "found only $CHANGES 0xC0 lines... injected already, or lib changed...?"
fi

# note need to ditch the .git dirs
cd $LIB_DIR && GITS=`find . -name '.git'`
e R "you need to delete\n$GITS\nin `pwd`..."
e G "all done"
