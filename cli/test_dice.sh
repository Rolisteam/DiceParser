#!/bin/sh

export LD_LIBRARY_PATH="/home/renaud/application/other/Qt/5.7/gcc_64/lib:$LD_LIBRARY_PATH"
for line in `cat cmds.txt`
do
  #echo $line;
	dice $line
done
