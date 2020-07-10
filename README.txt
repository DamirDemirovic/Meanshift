===========================
Version 1.0 September 16, 2019
by Damir Demirović <damir.demirovic@untz.ba>
Copyright : (C) 2019 IPOL Image Processing On Line http://www.ipol.im/


Introduction
____________________________

This source code provides an implementation of Meanshift algorithm for segmentation and filtering
and accompanying the paper "An Implementation of the Mean shift algorithm"

This program reads and writes PNG images, but can be easliy adapted to another formats.

Files
_______

README.txt                  - This File

COPYING                     - GNU AFFERO GENERAL PUBLIC LICENSE Version 3.

src/                        - Source directory with subdirectories imghelper, io_png, ms and ra

demo/                       - Demo for demonstration for meanshift segmentation and filtering

bin/                        - Compiled binaries

doc/                        - doxygen generated documentation
 

Requirements 
_____________________________

The code is written in ASCI C and C++. 
For compiling the code GCC C++ or another compatible compiler is needed.
Code depends only on libpng, and the header and libraries are required for cimpilation and execution.

 
Compiling the code
_____________________________

For compiling the code run the Makefile

$ make all

Output are the following executables

bin/meanshift   -  for Mean shift segmentation
bin/msfilter    -  for Mean shift filtering


Usage
_____________________________

Program use 6 parameters, input image, spatial radius, color radius and minimal region and output image.

// Run meanshift segmentation and filtering on boat.png with spatial_radius=7 color_radius=6.5 minimal_region=10

./meanshift boat.png 7 6.5 10 boat_segmented.png boat_filtered.png

// Run meanshift filtering image on boat.png

./msfilter boat.png 7 6.5 10 boat_filtered.png


Copyright and Licence
________________________________
Most the code is Copyright (C) 2019 by Damir Demirović <damir.demirovic@untz.ba>
This is a free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.
This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Affero General Public License for more details.
You should have received a copy of the GNU Affero General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

The io_png.c io_png.h   Copyright (c) 2010-2011, Nicolas Limare <nicolas.limare@cmla.ens-cachan.fr>
The RAList.cpp RAList.h TransitiveClosure.h TransitiveClosure.cpp are Copyright (c) 2002,  Chris M. Christoudias, Bogdan Georgescu


Thanks
---------
Author would be grateful to receive any comment, especially about errors, or strange results.
