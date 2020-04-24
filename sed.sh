#!/bin/sh

#write
sed -n 'w file' file.conf

#replace
sed -i '12s/a/b/g' file

#use regular expression
sed -i -E '/(?=abc)/,/b/{s/a/b/g}' file

#use var
num=`sed -n '/a/=' file | tail -n 1`
sed -i ''"$num"'s/a/b/g' file

#find first
sed -i '1,/b/s/a/b/g' file
num=1
sed -i ''"$num"',/b/s/a/b/g' file

#use grep
sed =n '/a/p' file | grep -o -E "a.c"

