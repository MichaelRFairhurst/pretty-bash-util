#!/bin/bash

[ -z "$1" ] && echo You must provide a file to watch && exit 1

echo Will begin monitoring $@ for you

LAST=( )
CURRENT=( )
I=0
SEDCOLS=
NOUPDATESSINCE=0

while true
do
  sleep 1
  I=0
  NOUPDATESSINCE=$(( $NOUPDATESSINCE + 1 ))
  SEDCOLS=$((`tput cols` - 3))
  for file in $@
  do
    I=$(( $I + 1 ))
    CURRENT[$I]=$(tail "$file" -n 2)
    ATS=$(printf @%.s $(seq $(( $(tput cols) / 2 - 5 ))))
    if [ "${CURRENT[$I]}" != "${LAST[$I]}" ]
    then
      echo
      echo -e $ATS]"\033[1;34mWakingUp\033[0m"[$ATS
      echo -e "  \033[31m"WARNING: Log $file has changed'!!'"\033[0m"
	  echo "  "Tail now shows:
      echo "${CURRENT[$I]}" | sed 's/^\(.\{0,'$SEDCOLS'\}\).*$/ - \1/'
      echo -en $ATS]"\033[1;34mSleeping\033[0m"[$ATS
      NOUPDATESSINCE=0
    fi
    LAST[$I]="${CURRENT[$I]}"
  done
  if [ $(( $NOUPDATESSINCE % 60 )) -eq 59 ]
  then
    if [ $NOUPDATESSINCE -lt 60 ]
    then
	  echo
	  echo
	  echo '-- Interrupting your active logging for this important message:'
    fi
    SPACER=$(printf ' %.s' $(seq $(( $(tput cols) / 2 - 23 ))))
    echo -en "$SPACER\033[32m[ - No new updates in the last ~$(( $NOUPDATESSINCE / 60 )) minutes - ]\033[0m\r"
  fi
done
