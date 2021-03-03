#!/bin/sh

pid=`ps -aux | grep $PWD | grep gameworld | grep -v grep | awk '{print $2}'`

gdb<<GDBEOF
file ./gameworld
attach $pid
set logging file gdb.out
set logging on

source gdb.txt

set logging off
detach
q
GDBEOF
