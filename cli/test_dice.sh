#!/bin/sh


DICE="/home/renaud/application/mine/build-DiceParser-Qt5_7-debug-dice/cli/bin/dice"

export LD_LIBRARY_PATH="/home/renaud/application/other/Qt/5.7/gcc_64/lib:$LD_LIBRARY_PATH"
for line in `cat cmds.txt`
do
  #echo $line;
	$DICE $line
done
