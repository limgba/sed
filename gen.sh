#!/bin/sh

xml=file
xml_path=/root/sed/${xml}.xml
rm -f config/*.xml
cp $xml_path ./config

if [ ! -d tmp ]; then
	mkdir tmp
fi
./debug.out ./config ./tmp 0
