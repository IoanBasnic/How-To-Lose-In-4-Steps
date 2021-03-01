#!/bin/bash

#To run this Shell script you need to have the lxterminal installed
#run the 'sudo apt-get install -y lxterminal' to install it on Ubuntu

lxterminal -e ./server &
sleep 1

#First client
lxterminal -e ./client &

#Second client
lxterminal -e ./client &
