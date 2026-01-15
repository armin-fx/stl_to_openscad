#!/usr/bin/env bash

# This script is to create executables for STL to OpenSCAD converter
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
# -upx        - compress executable files with upx (default)
# -upx-good   - compress executable files with upx with better compression
# -upx-max    - compress executable files with upx with maximum compression, very slow
# -upx-no     - disable compress executables
#
# -d target   - target directory for executables
#
# -m          - switch multithreading on, create all executables parallel at the same time.
#               By default it will compile at first only one executables to
#               get syntax errors first, then the other one.


#compile_options='-std=c++11 -O3 -s -fno-rtti'
compile_options='-std=c++11 -O3 -s'
compile_options_win='-static'
compile_options_linux='-Wall -pedantic'

switch_upx=1
switch_linux=0
switch_windows32=0
switch_windows64=0
multithreading=0
target_dir='./bin'
source=./src/stl_to_openscad.cpp

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
	[ "$1" == "-m" ]        && multithreading=1 && shift && shifted=1
	[ $shifted -eq 0 ] && echo "make.sh: ignore unknown argument \"$1\"" && shift
done

# test for standard setting
[ $switch_linux -eq 0 ] && [ $switch_windows32 -eq 0 ] && [ $switch_windows64 -eq 0 ] && switch_os_complete

# test for installed programs
if ! command -v g++ > /dev/null; then
	echo Please install programm g++
	exit
fi
if ! command -v i686-w64-mingw32-g++ > /dev/null && [ $switch_windows32 -ne 0 ]; then
	echo i686-w64-mingw32-g++ is not installed. Can not compile Windows 32bit program.
	switch_windows32=0
fi
if ! command -v x86_64-w64-mingw32-g++ > /dev/null && [ $switch_windows64 -ne 0 ]; then
	echo x86_64-w64-mingw32-g++ is not installed. Can not compile Windows 64bit program.
	switch_windows64=0
fi
if ! command -v upx > /dev/null && [ $switch_upx -ne 0 ]; then
	echo upx is not installed. The target program will not be compressed.
	switch_upx=0
fi

if [ ! -d "$target_dir" ]; then
	mkdir "$target_dir"
fi
if [ ! -d "$target_dir" ]; then
	echo "make.sh: could not create target directory \"$target_dir\""
	exit
fi


run_upx() {
	if   [ $switch_upx -eq 1 ]; then
		upx $1 | grep "stl_to_openscad" -
	elif [ $switch_upx -eq 2 ]; then
		upx --best $1 | grep "stl_to_openscad" -
	elif [ $switch_upx -eq 3 ]; then
		upx --ultra-brute $1 | grep "stl_to_openscad" -
	else
		ls -al $1 | awk '{printf("%*s    %s\n", 10, $5, $9)}'
	fi
}

# compile first one to get syntax errors first
if [ $multithreading -eq 0 ]; then
	if [ $switch_linux -eq 1 ]; then
		switch_linux=2
		g++                    $compile_options $compile_options_linux $source -o "$target_dir"/stl_to_openscad
	elif [ $switch_windows32 -eq 1 ]; then
		switch_windows32=2
		i686-w64-mingw32-g++   $compile_options $compile_options_win   $source -o "$target_dir"/stl_to_openscad32.exe
	elif [ $switch_windows32 -eq 1 ]; then
		switch_windows64=2
		x86_64-w64-mingw32-g++ $compile_options $compile_options_win   $source -o "$target_dir"/stl_to_openscad64.exe
	fi
fi

# compile and compress only if no error occurs
if [ $? -eq 0 ]; then
	if   [ $switch_upx -ne 0 ]; then
		echo '        File size         Ratio      Format      Name'
		echo '   --------------------   ------   -----------   -----------'
	else
		echo '   File size  Name'
		echo '   ---------  -----------'
	fi
	[ $switch_linux     -eq 1 ] && g++                    $compile_options $compile_options_linux $source -o "$target_dir"/stl_to_openscad       && run_upx "$target_dir"/stl_to_openscad &
	[ $switch_linux     -eq 2 ] && run_upx "$target_dir"/stl_to_openscad &
	[ $switch_windows32 -eq 1 ] && i686-w64-mingw32-g++   $compile_options $compile_options_win   $source -o "$target_dir"/stl_to_openscad32.exe && run_upx "$target_dir"/stl_to_openscad32.exe &
	[ $switch_windows32 -eq 2 ] && run_upx "$target_dir"/stl_to_openscad32.exe &
	[ $switch_windows64 -eq 1 ] && x86_64-w64-mingw32-g++ $compile_options $compile_options_win   $source -o "$target_dir"/stl_to_openscad64.exe && run_upx "$target_dir"/stl_to_openscad64.exe &
	[ $switch_windows64 -eq 2 ] && run_upx "$target_dir"/stl_to_openscad64.exe &
fi

wait
