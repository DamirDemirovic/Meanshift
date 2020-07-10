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
@mainpage Mean shift implementation
@author Damir Demirović  damir.demirovic@untz.ba

Mean shift algorithm represent a general non-parametric mode finding procedure.  It's a hill-climbing algorithm on the density defined by a finite mixture or a kernel density estimate. Mean shift can be used as a non-parametric clustering method, for object tracking, image segmentation.
*/

#include <stdio.h>
#include <cstdlib>
#include "ms/ms.h"
#include "io_png/io_png.h"

using namespace std;

/**
 * @file meanshift.cpp
 * @brief Main program for Meanshift segmentation
 *
 * @author Damir Demirović <damir.demirovic@untz.ba>
 */


int main(int argc, char* argv[])
{
    // initial value
    int num_iters = 100; // Initial number of iterations for Meanshift
    
    if (argc < 6)
    {
        // Tell the user how to run the program
        std::cerr << "Meanshift segmentation and filtering" << std::endl;
        std::cerr << "Usage: " << argv[0] << " image spatial_radius color_radius minRegion output_segmented [output_filtered]" << std::endl;
        std::cerr << "Example save only segmented image: " << argv[0] << " input.png 7 6.5 20 output_segmented.png" << std::endl;
        std::cerr << "Example save segmented and filtered image: " << argv[0] << " input.png 7 6.5 20 output_segmented.png output_filtered.png" << std::endl;
       return 1;
    }

    size_t width, height;
    // Read image to be segmented
    uchar * image = io_png_read_u8_rgb(argv[1], &width, &height);

    const int spatial_radius = atoi(argv[2]); // Spatial radius for Meanshift algorithm
    const double color_radius = atof(argv[3]); // Range radius for Meanshift algorithm
    const int minRegion = atoi(argv[4]); // Minimal region for merging
    const string filename_segment = argv[5]; // Filename for segmented image
       

    int **ilabels = GenerateLabels(width, height);
    
    uchar *segmented;
    uchar *filtered = AllocateUcharImage(width,height,3);
    
    segmented = MeanShift(image, filtered, ilabels, width, height, spatial_radius, color_radius, minRegion, num_iters);
 
    //Save segmented image
    io_png_write_u8(filename_segment.c_str(), segmented, width, height, 3);
    
    // Optional; save the filtered image
    if (argc == 7){
      const string filename_filtered = argv[6]; // Filename for filtered image
      uchar *rgb = ConvertLUV2RGB(filtered, width, height, 3); // Convert to ConvertLUV2RGB
      io_png_write_u8(filename_filtered.c_str(), rgb, width, height, 3);
      
      delete [] rgb;
     }
    
    for(size_t i=0; i<height; i++) delete [] ilabels[i];
    delete [] ilabels;

    delete [] segmented;
    delete [] filtered;
    free (image);
    
    return 0;
}
