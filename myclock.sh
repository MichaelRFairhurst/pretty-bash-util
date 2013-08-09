#!/bin/bash

SAVED=false

while true
do 
  echo -en '\e[45m\n\n\n\e[1A\t\t\t\t\e[44m\e[1;33m'`date "+%A %B %d %I:%M"`' '
  i=`date +%S`
  echo -en '\e[47m\e[1;35m -- | -- Seconds:[['
  while [ $i -lt 60 ]
  do 
    [ $i -gt 0 ] && $SAVED && echo -en '\e[u'
    echo -en '\e[1;37m\e[41m\e[s'$i'\e[47m\e[1;35m]] -- | -- \e[42m\e[1;31m '
    SAVED=true
    [ $(( $i % 2)) -eq 1 ] && echo -en $@ || echo -en '                                      '
    echo -en "\e[0m"
    i=$(( $i + 1 ))
    sleep 1
  done
  echo -en '\e[45m\n\e[1A'; 
done
