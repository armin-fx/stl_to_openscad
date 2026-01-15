stl_to_openscad
===============

Convert a 3D model in a STL file to a *.scad script file for OpenSCAD.


Usage
-----
Run
```
stl_to_openscad 3d_model.stl 3d_model.scad
```
to generate `3d_model.scad` from `3d_model.stl`.

### For Thingiverse
Thingiverse Customizer doesn't support the `import` command,
as it requires that everything is in one .scad file.
This tool is useful to copy and paste the module into one file.  
[=> Thingiverse](https://www.thingiverse.com/)


Building stl_to_openscad
------------------------

To build this from source you have to compile file `src/stl_to_openscad.cpp`.
It is written in standard C++17 and should work on every system.

### Tested in Linux Mint:
You can run to create linux and windows versions of stl_to_openscad with:
```
./make.sh
```
This will build the executable files into directory `./bin`.

You have to install some programs:
```
# to create (linux) programs in C++
sudo apt install g++

# optional, to create Windows programs
sudo apt install mingw-w64 g++-mingw-w64

# optional, to compress program files
sudo apt install upx
```
