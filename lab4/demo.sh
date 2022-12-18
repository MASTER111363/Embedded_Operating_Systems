#!/bin/sh

set -x
# set -e

rmmod -f mydev
insmod mydev.ko

./writer MARCO &
./reader 192.168.0.48 22 /dev/mydev
