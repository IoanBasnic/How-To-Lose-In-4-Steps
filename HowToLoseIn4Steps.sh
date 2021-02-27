#!/bin/bash

gcc -Wall -o server Server/TCPserver.c
gcc -Wall -o client Client/TCPclient.c

lxterminal -e ./server &

lxterminal -e ./client &
