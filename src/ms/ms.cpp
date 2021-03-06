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


#include "ms.h"
#include <stack>
#include "../ra/TransitiveClosure.h"


/**
 * @file ms.cpp
 * @brief Functions for Meanshift algorithm
 *
 * @author Damir Demirović <damir.demirovic@untz.ba>
 */




/*! \brief Function AddToStack add point to the stack.
*
*  \param i  x coordinate of the point
*  \param j  y coordinate of the point
*/
void AddToStack(std::stack<MSPoint> &stack, int i, int j)
{
    MSPoint p;
    p.x = i;
    p.y = j;
    stack.push(p);
}

/*! \brief Function MeanShift runs two phases of Mean shift algorithm Filter and Segment using Meanshift algorithm
*  The main program for Meanshift 
*  Meanshift algorithm:
*  First phase is image filtering. Filtered image is in L*u*v colorspace
*  Second phase is image segmentation.
*
*  \param image input image for Meanshift algorithm
*  \param filtered_luv output image filtered with Meanshift algorithm
*  \param labels labels
*  \param width width of the image
*  \param height height of the image
*  \param spatial_radius spatial radius
*  \param color_radius range radius
*  \param minRegion  minimal region for merging
*  \param num_iters initial number of iterations
*  \return segmented image
*/

uchar* MeanShift(uchar* image, uchar* filtered_luv, int **labels, int width, int height, int spatial_radius, double color_radius, int minRegion, int num_iters)
{
    int regCount;
 
    // First phase of Meanshift filtering
    // filtered image is in L*u*v colorspace
        
    uchar *filt;
    filt = MS_Filter(image, width, height, spatial_radius, color_radius, num_iters);
    
    memcpy(filtered_luv, filt, height*width*3);
    
    // Second phase of Meanshift is segmentation
    regCount = MS_Segment(filt, width, height, labels, color_radius, minRegion);
     
    uchar *segmented = new uchar[height*width*3];
    memcpy(segmented, filt, height*width*3);
  
    // Label regions in the segmented image with labels
    LabelImage(segmented, width, height, labels, regCount);
        
    delete [] filt;
    return segmented;
}


/*! \brief Function MS_Filter filter image usign Meanshift algorithm using a circular flat kernel and color distance in L*u*v colorspace
*
*  Based on implementation from https://imagej.nih.gov/ij/plugins/download/Mean_Shift.java
*
*   For each pixel of the image and the set ot the neighboring pixels within the specified  spatial radius and color distance
*   is determined.  For this set of neighbor pixels, the new spatial center an the new color mean values are calculated.
*   These new values will serve as the new center for the next iteration.
*   This procedure will iterate until the spatial and color means will stop changing or the maximal number of iterations is achieved.
*
*  \param image input image
*  \param width width of the image
*  \param height height of the image
*  \param spatial_radius spatial radius
*  \param color_radius range radius
*  \param initIters initial number of iterations
*  \return luv Meanshift filtered image in L*u*v colorspace.
*/
uchar* MS_Filter(uchar* image, int width, int height, int spatial_radius, double color_radius, int initIters)
{
    double color_radius_squared = color_radius * color_radius;

    // Convert image to L*u*v colorspace
    uchar * luv = ConvertRGB2LUV(image, width, height, 3);
    // Initialize number of iterations
    int  num_iters=initIters;

    for(int j = 0; j < height; j++)
        for(int i = 0; i < width; i++)
        {
            int ic = i;
            int jc = j;
            int icOld, jcOld;
            float LOld, UOld, VOld;

            float L = GetPixel(luv, width, height, i, j, 1);
            float U = GetPixel(luv, width, height, i, j, 2);
            float V = GetPixel(luv, width, height, i, j, 3);

            double ms_shift = 5; // initial value of mean shift

            for (int iters = 0; ms_shift > 1 && iters < num_iters; iters++)
            {
                float mi = 0;
                float mj = 0;
                float mL = 0;
                float mU = 0;
                float mV = 0;
                int  num = 0;

                int ifrom = max(0, i - spatial_radius), ito = min(width, i + spatial_radius + 1);
                int jfrom = max(0, j - spatial_radius), jto = min(height, j + spatial_radius + 1);

                for (int jj = jfrom; jj < jto; jj++)
                {
                    for (int ii = ifrom; ii < ito; ii++)
                    {

                        float L2 = GetPixel(luv, width, height, ii, jj, 1);
                        float U2 = GetPixel(luv, width, height, ii, jj, 2);
                        float V2 = GetPixel(luv, width, height, ii, jj, 3);

                        double dL = L2 - L;
                        double dU = U2 - U;
                        double dV = V2 - V;

                        if (dL * dL + dU * dU + dV * dV <= color_radius_squared)
                        {
                            mi += ii;
                            mj += jj;
                            mL += L2;
                            mU += U2;
                            mV += V2;
                            num++;
                        }
                    }
                }

                icOld = ic;
                jcOld = jc;
                 LOld = L;
                 UOld = U;
                 VOld = V;
                 
                //  Calculate value for uniform kernel
                float num_ = 1.f / num;
                L = mL * num_;
                U = mU * num_;
                V = mV * num_;
                ic = (int) (mi * num_ + 0.5);
                jc = (int) (mj * num_ + 0.5);
                int di = ic - icOld;
                int dj = jc - jcOld;
                double dL = L - LOld;
                double dU = U - UOld;
                double dV = V - VOld;

                // calculate mean shift vector
                ms_shift = di * di + dj * dj + dL * dL + dU * dU + dV * dV;
            }
            // Set pixel L, U and v values
            SetPixel(luv, width, height, i, j, (uchar)L, 1); // L
            SetPixel(luv, width, height, i, j, (uchar)U, 2); // u
            SetPixel(luv, width, height, i, j, (uchar)V, 3); // v
        }
        
    return luv;
}



/*! \brief Function MS_Segment segments the image using Meanshift algorithm
*
*
*  \param image in L*u*v colorspace,
*  \param width width of the image
*  \param height height of the image
*  \param labels contain labels
*  \param color_radius  range radius
*  \param minRegion minimal region for merging
*  \return regCount Number of segmented regions
*/
int MS_Segment(uchar * image, int width, int height, int **labels, double color_radius, int minRegion)
{
    int regCount;
    float* mode = new float[height * width * 3];
    int* modePoints = new int[height * width];

    // Initialize modePoints to zero
    for (int i = 0; i < width * height; i++)
        modePoints[i] = 0;
    // Cluster image with  Mean shift
    regCount = MS_Cluster(image, width, height, labels, modePoints, mode, color_radius);
    // Run transitive closure algorithm
    TransitiveClosure(width, height, labels, modePoints, mode, color_radius, regCount, minRegion);

    // mode and modePoints deleted at the end of TransitiveClousure
    return regCount;

}

/*! \brief Function MS_Cluster cluster the image using Meanshift
*
*
*  \param image in L*u*v colorspace,
*  \param width width of the image
*  \param height height of the image
*  \param labels contain labels
*  \param modePoints data about mode points
*  \param mode   data about mode
*  \param color_radius  range radius
*  \return regCount number of regions
*/
int MS_Cluster(uchar  *image, int width, int height, int **labels,int* modePoints, float *mode, double color_radius)
{
    int regCount = 0;
    int lbl = -1;
    double color_radius2 = color_radius * color_radius;

    //  8-connected neighbors
    const int dxdy[8][2] = { {-1,-1} , {-1,0} , {-1,1} , {0,-1} , {0,1} , {1,-1} , {1,0} , {1,1} };

    // Initialize matrix of labels with value -1
    for(int j = 0; j < height; j++)
        for(int i = 0; i < width; i++)
            labels[j][i] = -1;

    for(int j = 0; j < height; j++)
    {
        for(int i = 0; i < width; i++)
        {
            if(labels[j][i] < 0)   // if label is not assigned
            {
                labels[j][i] = ++lbl;

                float L = GetPixel(image, width, height, i, j, 1);  // L
                float U = GetPixel(image, width, height, i, j, 2);  // u
                float V = GetPixel(image, width, height, i, j, 3);  // v

                // Convert 8-bit data to L*u*v range 0<=l<=100, −134<=u<=220, −140<=v<=122
                mode[lbl * 3 + 0] = 100 * L / 255;
                mode[lbl * 3 + 1] = 354 * U / 255 - 134;
                mode[lbl * 3 + 2] = 256 * V / 255 - 140;

                std::stack<MSPoint> stack;
                AddToStack(stack, i, j);

                while(!stack.empty())
                {
                    MSPoint point = stack.top();
                    stack.pop();
                    
                    for(int k = 0; k < 8; k++) // calculate for 8 connected pixels
                    {
                        int ii = point.x + dxdy[k][0];
                        int jj = point.y + dxdy[k][1];

                        if(ii >= 0 && jj >= 0 && jj < height && ii < width && labels[jj][ii] < 0 && range_distance(image,width, height,i,j,ii,jj) < color_radius2)
                        {
                            labels[jj][ii] = lbl;
                            AddToStack(stack, ii,  jj);
                            modePoints[lbl]++;

                            float L = GetPixel(image, width, height, ii, jj, 1);  // L
                            float U = GetPixel(image, width, height, ii, jj, 2);  // u
                            float V = GetPixel(image, width, height, ii, jj, 3);  // v

                            // Convert 8-bit data to L*u*v range 0<=l<=100, −134<=u<=220, −140<=v<=122
                            mode[lbl * 3 + 0] += 100 * L / 255;
                            mode[lbl * 3 + 1] += 354 * U / 255 - 134;
                            mode[lbl * 3 + 2] += 256 * V / 255 - 140;

                        }
                    }
                }
                
                mode[lbl * 3 + 0] /= modePoints[lbl];
                mode[lbl * 3 + 1] /= modePoints[lbl];
                mode[lbl * 3 + 2] /= modePoints[lbl];
            }
        }
    }
    
    regCount = ++lbl;

    return regCount;
}


