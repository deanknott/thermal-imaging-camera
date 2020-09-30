#!/bin/bash
# unphone-sheffield-test.sh
#
# xdo an IDE test, then an IDF test
#
# hamish, 18/9/2019, from john's unphone-test

# we redirect output from the IDE and grep it to identify compile end point
TMPFILE=/tmp/unphone-sheffield-test-$$.txt
STARTSECS=$SECONDS

# timings get stored here
RESULTS=~/test-results.txt
(echo; echo Testing unphone setup and compile at `date`; echo) >>$RESULTS

# WARNING!
cd
echo 'EEEK! About to DELETE ~/unphone in 5 secs... CNTRL^C quick to stop!'
sleep 6
rm -rf unphone

# clone and setup unphone tree
START=`date "+%Y-%m-%d-%T"`
/usr/local/bin/unphone-sheffield.sh
export IDF_PATH=`pwd`/unphone/sdks/esp-idf
export PATH=`pwd`/unphone/sdks/arduino-esp32/tools/xtensa-esp32-elf/bin:${PATH}
END=`date "+%Y-%m-%d-%T"`
echo "unphone setup started at           $START and ended at $END" |
  tee -a $RESULTS
sleep 2
cd unphone

# IDF build
START=`date "+%Y-%m-%d-%T"`
( cd examples/BigDemoIDF && time make app )
END=`date "+%Y-%m-%d-%T"`
echo "IDF compile started at             $START and ended at $END" |
  tee -a $RESULTS
sleep 5

# IDE build
make arduino-ide >$TMPFILE 2>&1 &
MAKE_IDE_PID=$!
sleep 22
xdotool windowactivate $(xdotool search --name Arduino) && sleep 5 && \
        xdotool key ctrl+o && sleep 5 && \
        xdotool type "~/unphone/examples/BigDemo/BigDemo.ino"
sleep 5
START=`date "+%Y-%m-%d-%T"`
xdotool windowactivate $(xdotool search --name BigDemo) key ctrl+r

# wait until the compile end result is reported, then echo the timing
while :
do
  sleep 2
  grep -sq \
    'Sketch uses [0-9][0-9]* bytes ([0-9][0-9]%) of program storage ' \
      $TMPFILE && \
    END=`date "+%Y-%m-%d-%T"` && \
    { echo "IDE compile started at             $START and ended at $END" | \
      tee -a $RESULTS; } && \
    break
done

# shut the IDE
sleep 2
IDE_PID=`ps x | grep -v grep | grep 'java .*arduino' |sed 's, pts.*,,'`
kill -9 $IDE_PID $MAKE_IDE_PID

# finally, a trivial (IDF) build
echo >> examples/BigDemo/BigDemo.ino

# trivial IDF build
START=`date "+%Y-%m-%d-%T"`
( cd examples/BigDemoIDF && time make app )
END=`date "+%Y-%m-%d-%T"`
echo "Trivial IDF compile started at     $START and ended at $END" |
  tee -a $RESULTS
sleep 5

# report total times
TOTALSECS=$(($SECONDS - $STARTSECS))
MINS=`date -d@$TOTALSECS -u +%M:%S`
echo "Total runtime was                  $MINS (or $TOTALSECS secs)" |
  tee -a $RESULTS

# clean up
git checkout examples/BigDemo/BigDemo.ino
echo >>$RESULTS
rm $TMPFILE
