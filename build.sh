#!/usr/bin/env bash

# MIT License
# 
# Copyright (c) 2017-2018 Cody Tilkins
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


# --------------------------------------------------------------------


debug=0
debug_coverage=0


# --------------------------------------------------------------------


# I am actually guessing here what it is on linux \_(:/)_/ perhaps even the headers are wrong
libs=dll/OpenAL32.so


# --------------------------------------------------------------------


if [[ $debug -eq 0 ]]; then
	attrib="-std=gnu11 -s -Wall -O2"
	root=bin/Release
else
	attrib="-std=gnu11 -Wall -g -O0"
	root=bin/Debug
	if [ $debug_coverage -eq 1 ]; then
		attrib=$attrib -coverage
	fi
fi


objdir=obj
libdir=lib
resdir=res
rootdir=root
dlldir=dll
srcdir=src
incdir=include

dirs="-L$srcdir -L$libdir -L$dlldir -I$srcdir -I$incdir"


# Ensure bin && bin/res exists
if [ -d $root ]; then rm -r --one-file-system -d $root; fi
mkdir -p $root/res


# --------------------------------------------------------------------
# Not tested. Have fun.                                              #
# --------------------------------------------------------------------

echo Compiling...
gcc $attrib $dirs -fPIC -c $srcdir/*.c
if [ $? -ne 0 ]; then 
	mv *.o		$objdir		1>/dev/null	2>/dev/null
	exit 1;
fi

echo Linking...
gcc $attrib $dirs -fPIC -shared -Wl,-E -o libopenlual.so *.o $dlldir/*.so
if [ $? -ne 0 ]; then
	mv *.so		$root		1>/dev/null	2>/dev/null
	mv *.o		$objdir		1>/dev/null	2>/dev/null
	exit 1;
fi


# --------------------------------------------------------------------


# Migrate binaries
mv *.so		$root		1>/dev/null	2>/dev/null
mv *.o		$objdir		1>/dev/null	2>/dev/null
mv *.a		$objdir		1>/dev/null	2>/dev/null
cp -r		$resdir/*	$root/res	1>/dev/null	2>/dev/null
cp -r		$dlldir/*	$root		1>/dev/null	2>/dev/null
cp -r		$rootdir/*	$root		1>/dev/null	2>/dev/null


echo Done.

