#! /bin/bash

$EXTRACT_TR_STRINGS `find . -name \*.cpp -o -name \*.h` -o $podir/rolisteam-diceparser_qt.pot

#previousText=""

#cat HelpMe.md | { cat ; echo ; } | while read line; 
#do 
#    if [ -z "$line" ];
#    then
#        if [ -n "$previousText" ];
#        then
#            echo "i18nd(\"HelpMe\", \"$previousText\");" >> rc.cpp 
#            previousText=""
#        fi
#    else
#        if [ -n "$previousText" ];
#        then
#            previousText="$previousText\n$line"
#        else
#            previousText="$line"
#        fi
#    fi
#done
#if [ -n "$previousText" ];
#then
#    echo "i18nd(\"HelpMe\", \"$previousText\");" >> rc.cpp 
#fi

#$XGETTEXT rc.cpp -o $podir/rolisteam-diceparser_documentation.pot
