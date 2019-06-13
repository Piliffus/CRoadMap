#!/bin/bash

INPUT=$@
FILE=$1

#arguments amount < 2
if [ $# -lt 2 ] 
then
    exit 1
fi

for ROUTE in "$@"
do
    #omit filename for obvious reasons
    if [ $ROUTE = $1 ] 
    then
        continue
    fi
    #check if proper road name
    #check if it is number
    if  ! echo $ROUTE | egrep -q '^[0-9]+$'; 
    then
        exit 1
    fi
    #check if it is correct number
    if [ $ROUTE -gt 1000 ] || [ $ROUTE -le 0 ] 
    then
        exit 1
    fi

    #cut off part with road name
    STRING=($(grep -m 1 "^$ROUTE" $FILE | sed 's/^[^;]*;//g'))
    LENGTH=0
    OIFS=$IFS
    IFS=';'
    #split string with IFS
    read -ra ADDR <<< "$STRING"

    #add elements to array
    for i in "${ADDR[@]}"
    do
        VALUES+=("$i")
    done

    #calculate length
    ARRAY_LENGTH=${#VALUES[@]}
    j=1
    while [ $j -lt $ARRAY_LENGTH ]
    do
        LENGTH=$(($LENGTH+${VALUES[$j]}))
        j=$(($j+3))
    done
    IFS=$OIFS
    echo "$ROUTE;$LENGTH"
done

exit 0