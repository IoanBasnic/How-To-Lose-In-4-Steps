#!/bin/bash

#To run this Shell script you need to have the lxterminal installed
#run the 'sudo apt-get install -y lxterminal' to install it on Ubuntu

gcc -Wall -o server Server/TCPserver.c
gcc -Wall -o client Client/TCPclient.c

lxterminal -e ./server &

lxterminal -e ./client &
