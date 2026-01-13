#!/usr/bin/env bash

# This script is to create executables for STL to OpenScad Konverter
# This script run under Linux
# and should run under Mac OS X, as well as Windows with Cygwin.

# Arguments:
#
# -all        - create executables for all operating systems 
#               default, if no other system option is set
# -linux      - create executable for linux
# -windows    - create executable for windows 32bit and 64bit
# -win32      - create executable for windows 32bit
# -win64      - create executable for windows 64bit
#
# -upx        - compress executable files with upx (standard)
# -upx-good   - compress executable files with upx with better compression
# -upx-max    - compress executable files with upx with maximum compression, very slow
# -upx-no     - disable compress executables
#
# -d target   - target directory for executables


#compile_options='-std=c++11 -O3 -s -fno-rtti'
compile_options='-std=c++11 -O3 -s'
compile_options_win='-static'
compile_options_linux='-Wall -pedantic'

switch_upx=1
switch_linux=0
switch_windows32=0
switch_windows64=0
target_dir='./bin'
source=./src/STL_to_OpenScad.cpp

switch_os_complete() {
	switch_linux=1
	switch_windows32=1
	switch_windows64=1
}

# test for switches
while [ "$1" != '' ]
do
	shifted=0
	[ "$1" == "-upx-no" ]   && switch_upx=0 && shift && shifted=1
	[ "$1" == "-upx-off" ]  && switch_upx=0 && shift && shifted=1
	[ "$1" == "-upx" ]      && switch_upx=1 && shift && shifted=1
	[ "$1" == "-upx-good" ] && switch_upx=2 && shift && shifted=1
	[ "$1" == "-upx-max" ]  && switch_upx=3 && shift && shifted=1
	[ "$1" == "-all" ]      && switch_os_complete && shift && shifted=1
	[ "$1" == "-linux" ]    && switch_linux=1 && shift && shifted=1
	[ "$1" == "-win32" ]    && switch_windows32=1 && shift && shifted=1
	[ "$1" == "-win64" ]    && switch_windows64=1 && shift && shifted=1
	[ "$1" == "-windows" ]  && switch_windows32=1 && switch_windows64=1 && shift && shifted=1
	[ "$1" == "-d" ]        && shift && target_dir=$1 && shift && shifted=1
	[ $shifted -eq 0 ] && echo "make.sh: ignore unknown argument \"$1\"" && shift
done

# test for standard setting
[ $switch_linux -eq 0 ] && [ $switch_windows32 -eq 0 ] && [ $switch_windows64 -eq 0 ] && switch_os_complete

if [ ! -d "$target_dir" ]; then
	mkdir "$target_dir"
fi
if [ ! -d "$target_dir" ]; then
	echo "make.sh: could not create target directory \"$target_dir\""
	exit
fi


run_upx() {
	if   [ $switch_upx -eq 1 ]; then
		upx $1 | grep "STL_to_OpenScad" -
	elif [ $switch_upx -eq 2 ]; then
		upx --best $1 | grep "STL_to_OpenScad" -
	elif [ $switch_upx -eq 3 ]; then
		upx --ultra-brute $1 | grep "STL_to_OpenScad" -
	else
		ls -al $1 | awk '{printf("%*s    %s\n", 10, $5, $9)}'
	fi
}

if [ $switch_linux -eq 1 ]; then
	g++ $compile_options $compile_options_linux $source -o "$target_dir"/STL_to_OpenScad
else
	g++ $compile_options $compile_options_linux $source -o /dev/null
fi

if [ $? -eq 0 ]; then
	if   [ $switch_upx -ne 0 ]; then
		echo '        File size         Ratio      Format      Name'
		echo '   --------------------   ------   -----------   -----------'
	else
		echo '   File size  Name'
		echo '   ---------  -----------'
	fi
	[ $switch_linux     -eq 1 ] && run_upx "$target_dir"/STL_to_OpenScad &
	[ $switch_windows32 -eq 1 ] && i686-w64-mingw32-g++   $compile_options $compile_options_win $source -o "$target_dir"/STL_to_OpenScad32.exe && run_upx "$target_dir"/STL_to_OpenScad32.exe &
	[ $switch_windows64 -eq 1 ] && x86_64-w64-mingw32-g++ $compile_options $compile_options_win $source -o "$target_dir"/STL_to_OpenScad64.exe && run_upx "$target_dir"/STL_to_OpenScad64.exe &
fi

wait
