/*
 * Copyright (c) 2019, Damir Demirović <damir.demirovic@untz.ba>
 * All rights reserved.
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later
 * version. You should have received a copy of this license along
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
@mainpage Meanshift implementation
@author Damir Demirović  damir.demirovic@untz.ba

Mean shift algorithm represent a general non-parametric mode finding procedure.  It's a hill-climbing algorithm on the density defined by a finite mixture or a kernel density estimate. Mean shift can be used as a non-parametric clustering method, for object tracking, image segmentation.
*/

#include <stdio.h>
#include <cstdlib>
#include "ms/ms.h"
#include "io_png/io_png.h"

using namespace std;



/**
 * @file msfilter.cpp
 * @brief Main program for Meanshift filtering
 *
 * @author Damir Demirović <damir.demirovic@untz.ba>
 */


int main(int argc, char* argv[])
{
    // initial value
    int num_iters = 100; // Initial number of iterations for Meanshift

     if (argc < 5)
    {
        // Tell the user how to run the program
        std::cerr << "Meanshift filtering" << std::endl;
        std::cerr << "Usage: " << argv[0] << " input_image spatial_radius color_radius output_filename" << std::endl;
        std::cerr << "Example: " << argv[0] << " input.png 7 6.5 output.png" << std::endl;
       return 1;
    }
  
    size_t width, height;
    // Read image to be segmented or filtered
    uchar * image = io_png_read_u8_rgb(argv[1], &width, &height);
    const int spatial_radius = atoi(argv[2]); // Spatial radius for Meanshift algorithm
    const double color_radius = atof(argv[3]); // Range radius for Meanshift algorithm
  //  const int minRegion = atoi(argv[4]); // Minimal region for merging
 
    const string filename_filter = argv[4];  // Filename for filtered image
    // Filter phase in L*u*v color space
    uchar *filtered = MS_Filter(image, width, height, spatial_radius, color_radius, num_iters);
    // Convert image to RGB and save
    uchar *rgb = ConvertLUV2RGB(filtered, width, height, 3);
    io_png_write_u8(filename_filter.c_str(), rgb, width, height, 3);

    delete [] filtered;
    delete [] rgb;

    free (image);


    return 0;
}
