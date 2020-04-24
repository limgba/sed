#!/bin/bash

root=/root
my_file='file'

function checkout()
{
	cd $root
	rm -rf $1
	mkdir $1
	cd $1
	svn co svn://localhost/$1 .
}

function modify()
{
	path=$1
	file=$2
	checkout $path
	sed -i 's/f/g/g' $file
	svn cl change $2
	svn commit -m 'auto commit' --cl change
}

modify svntest $my_file
