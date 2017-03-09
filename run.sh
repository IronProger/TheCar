#!/usr/bin/bash

# $1 — executable
# $2 — logfile

if [ $1 = help ] || [ $1 = --help ] || [ -z $1 ] || [ -z $2 ] ; then
    echo "usage: $1 path_to_binary… path_to_logfile_what_you_wanna_show…"
    exit
fi

command $1 &
tail -f $2 -n 0 --pid=$!