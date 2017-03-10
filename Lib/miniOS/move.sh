#!/bin/bash

while [ "$#" -ge "1" ]; do
out=$(grep "Texas" $1)
if [ -n "$out" ]; then
    #echo $1
    mv $1 ./newlib_include
fi
shift
done


