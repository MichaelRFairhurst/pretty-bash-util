#!/bin/bash

NUMS=( )
NUMS[0]=' ###__
#   #_
#   #_
#   #_
 ###  '
NUMS[1]='  #___
###___
  #___
  #___
##### '
NUMS[2]=' ###__
#   #_
  ##__
 #____
##### '
NUMS[3]=' ###__
#   #_
  ##__
#   #_
 ###  '
NUMS[4]='  # #_
 #  #_
#####_
    #_
    #_'
NUMS[5]=' ####_
#_____
 ####_
     #
####  '
NUMS[6]=' ####_
#_____
#####_
#    #
 ###  '
NUMS[7]='#####_
   #__
  #___
 #____
#     '
NUMS[8]=' ###__
#   #_
 ###__
#   #_
 ###  '
NUMS[9]=' ###__
#   #_
 ####_
    #_
 ###  '

ESC='\\e'
CNUMS=( )
for i in {0..9}
do
CNUMS[$i]=$(echo "${NUMS[$i]}" | sed "s/[ _]/$ESC[1C/g" | tr '\n' "+" | sed "s/\+/$ESC[1B$ESC[6D/g" | sed "s/#/$ESC[31m0$ESC[0m/g")
done

echo > squareclock_nums.h
for i in {0..9}
do
echo '#define NUMS'$i '"'${CNUMS[$i]}'"' >> squareclock_nums.h
done
