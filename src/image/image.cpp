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

#include "image.h"
#include <cstdlib>


/**
 * @file image.cpp
 * @brief Image helper functions
 *
 * @author Damir Demirović <damir.demirovic@untz.ba>
 */

/*! \brief Allocate space for the uchar image,
*
*
*  \param width, height and nchannel
*  \return img - allocated image
*/
uchar * AllocateUcharImage(int width, int height, int nchannel)
{
    // Allocate and initialize to zero
    uchar * img = new uchar[ width * height * nchannel]();
    return img;
}


/*! \brief Generate labels for clustering.
*
*  Function generate labels.
*  \param width, height
*  \return labels
*/
int** GenerateLabels(size_t width,size_t height)
{
    int **labels = new int *[height];

    for(size_t i = 0; i < height; i++)
        labels[i] = new int [width];
    return labels;
}


/*! \brief Function generate random numbers
*
*
*  \param count - Count of numbers to be generated
*  \return number as vector<int>
*/
vector<int> GenerateRandomNumbers(int count)
{
    vector<int> number(count);

    for(int i = 0; i < count; i++)
        number[i] = rand();

    return number;
}

/*! \brief Function LabelImage in RGB colors
*
*  \param image image to be labeled
*  \param width width of the image
*  \param height heighto of the image
*  \param labels color labels
*  \param regCount regions to be labeled
*/
void LabelImage(uchar *image, int width, int height, int** labels,int regCount)
{
    vector<int> color = GenerateRandomNumbers(regCount);

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int label = labels[i][j];
            SetPixel(image, width, height, j, i, (uchar)((color[label]) & 255),1);
            SetPixel(image, width, height, j, i, (uchar)((color[label] >> 8) & 255),2);
            SetPixel(image, width, height, j, i, (uchar)((color[label] >> 16) & 255),3);

        }
    }
}

/*! \brief Function RGB2LUV converts RGB pixel value to LUV pixel value.
*
*  \param r component of input image
*  \param g component of input image
*  \param b component of input image
*  \param l component of output image   
*  \param u component of output image
*  \param v component of output image  0<=l<=100, −134<=u<=220, −140<=v<=122
*/
inline void RGB2LUV(float r, float g, float b, uchar *l, uchar *u, uchar *v)
{
   
    float X, Y, Z;
    int R,G,B;

    R=(int)r;
    G=(int)g;
    B=(int)b;

    X = 0.412453 * R + 0.357580 * G + 0.180423 * B;
    Y = 0.212671 * R + 0.715160 * G + 0.072169 * B;
    Z = 0.019334 * R + 0.119193 * G + 0.950227 * B;

    X /= 255.0;
    Y /= 255.0;
    Z /= 255.0;


    float L1, u1, v1, u2, v2, ur2, vr2, yr, eps, k;
    eps = 216.0 / 24389.0;
    k = 24389.0 / 27.0;

    float Xr = 0.964221;
    float Yr = 1.0;
    float Zr = 0.825211;

    u2 = 4.0 * X / (X + 15.0 * Y + 3.0 * Z);
    v2 = 9.0 * Y / (X + 15.0 * Y + 3.0 * Z);

    ur2 = 4.0 * Xr / (Xr + 15.0 * Yr + 3.0 * Zr);
    vr2 = 9.0 * Yr / (Xr + 15.0 * Yr + 3.0 * Zr);

    yr = Y / Yr;

    if ( yr > eps )
    {
        L1 = (116.0 * pow(yr, 1.0 / 3.0) - 16.0);

    }
    else
    {
        L1 = k * yr;
    }

    u1 = 13.0 * (L1) * (u2 - ur2);
    v1 = 13.0 * (L1) * (v2 - vr2);

    if( X == 0.0 && Y == 0.0 && Z == 0.0)
    {
        L1 = 0;
        u1 = 0;
        v1 = 0;
    }
    else     // Convert component to 8-bit destination data type (0-255)
    {
        *l = (int)(L1 + 0.5) * 255 / 100;
        *u = (int)(u1 + 0.5 + 134) * 255 / 354;
        *v = (int)(v1 + 0.5 + 140) * 255 / 262;
    }

}

/*! \brief Function LUV2RGB converts LUV pixel value to RGB pixel value.
*
*  \param L component of input image
*  \param u component of input image
*  \param v component of input image
*  \param r component of output image
*  \param g component of output image
*  \param b component of output image
*/
inline void LUV2RGB(float L, float u, float v, uchar *R, uchar *G, uchar *B)
{
    int LL = (int)L;
    int uu = (int)u;
    int vv = (int)v;

    L= (float)(LL * 100 / 255);
    u= (float)((uu) * 354 / 255 - 134);
    v= (float)((vv) * 262 / 255 - 140);


    float X, Y, Z, ud, vd, u0, v0, TEMP, L1;
    int r, g, b;
    float eps = 216.0 / 24389.0;
    float k = 24389.0 / 27.0;
    float Xr = 0.964221;
    float Yr = 1.0;
    float Zr = 0.825211;

    u0 = 4.0 * Xr / (Xr + 15.0 * Yr + 3.0 * Zr);
    v0 = 9.0 * Yr / (Xr + 15.0 * Yr + 3.0 * Zr);
    L1 = ((float)(L)) / 1.0;
    if((float)(L1) > k * eps)
    {
        TEMP = (((float)(L1) + 16.0) / 116.0);
        Y = TEMP * TEMP * TEMP;
    }
    else
    {
        Y = ((float)(L1)) / k;
    }
    if((L == 0) && (u == 0) && (v == 0))
    {
        X = 0;
        Y = 0;
        Z = 0;
    }
    else
    {
        ud = (u / (13.0 * L1) + u0);
        vd = (v / (13.0 * L1) + v0);
        X = (ud / vd) * Y * 9.0 / 4.0;
        Z = (Y / vd - ((ud / vd) * Y / 4.0 + 15.0 * Y / 9.0)) * 3.0;
    }

    X *= 255.0;
    Y *= 255.0;
    Z *= 255.0;

    r = (int)(3.2404813432005 * X - 1.5371515162713 * Y - 0.49853632616889 * Z + 0.5);
    g = (int)(-0.96925494999657 * X + 1.8759900014899 * Y + 0.041555926558293 * Z + 0.5);
    b = (int)(0.055646639135177 * X - 0.20404133836651 * Y + 1.0573110696453 * Z + 0.5);

    *R = r < 0 ? 0 : r > 255 ? 255 : r;
    *G = g < 0 ? 0 : g > 255 ? 255 : g;
    *B = b < 0 ? 0 : b > 255 ? 255 : b;
}


/*! \brief Function ConvertRGB2LUV convert RGB image to  LUV
*
*  \param rgb RGB image to convert
*  \param width width of the image
*  \param height height of the image
*  \param nchannel number of image channels
*  \return luv the converted image
*/
uchar * ConvertRGB2LUV(uchar * rgb, int width, int height, int nchannel)
{
    uchar *luv = AllocateUcharImage(width,height,nchannel);

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int index_R = i * width + j;
            int index_G = height * width + i * width + j;
            int index_B = 2 * height * width + i * width + j;

            RGB2LUV(rgb[index_R], rgb[index_G], rgb[index_B], &luv[index_R], &luv[index_G], &luv[index_B]);

        }
    }

    return luv;

}


/*! \brief  Function ConvertLUV2RGB converts input image image from LUV to RGB color space
*
*  \param luv LUV image to convert
*  \param width width of the image
*  \param height height of the image
*  \param nchannel number of image channels
*  \return The input character
*/
uchar * ConvertLUV2RGB(uchar * luv, int width, int height, int nchannel)
{
    uchar *rgb = AllocateUcharImage(width, height, nchannel);

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int index_L = i * width + j;
            int index_U = height * width + i * width + j;
            int index_V = 2  * height * width + i * width + j;

            LUV2RGB(luv[index_L], luv[index_U], luv[index_V], &rgb[index_L], &rgb[index_U], &rgb[index_V]);
        }
    }
    return rgb;

}

/*! \brief Set Pixel at channel component of image at postition given with x and y
*
*  \param im image to convert
*  \param width width of the image
*  \param height height of the image
*  \param x x position in the image
*  \param y y position in the image
*  \param val value to set at pixel location
*  \param nchannel number of image channels
*/
void SetPixel(uchar *im, int width, int height, int x, int y, const uchar val, int nchannel)
{
    *(im + (nchannel - 1) * height * width + y * width + x) = val;
}

/*! \brief Get Pixel at channel component of image at postition given with x and y
*  \param im image to convert
*  \param width width of the image
*  \param height height of the image
*  \param x x position in the image   0 < x < width
*  \param y y position in the image   0 < y < height
*  \param nchannel number of image channels
*  \return pixel value
*/
uchar GetPixel(uchar *im, int width, int height, int x, int y, int nchannel)
{
    return *(im + (nchannel - 1) * height * width + y * width + x);
}

/*! \brief Function range_distance calculate range distance between two pixels
*  \param image image to calculate
*  \param width width of the image
*  \param height height of the image
*  \param x1 first x position in the image
*  \param y1 first y position in the image
*  \param x2 second x position in the image
*  \param y2 second y position in the image
*  \return squared sum of distances
*/
int range_distance(uchar* image, int width, int height, int x1, int y1, int x2, int y2 )
{
    int r = GetPixel(image, width, height, x1, y1, 1) - GetPixel(image, width, height, x2, y2, 1);
    int g = GetPixel(image, width, height, x1, y1, 2) - GetPixel(image, width, height, x2, y2, 2);
    int b = GetPixel(image, width, height, x1, y1, 3) - GetPixel(image, width, height, x2, y2, 3);

    return r * r + g * g + b * b;
}

/*! \brief Function color_distance calculate color distance between the two vectors
*
*  \param a, b input images
*  \return the color distance
*/
float color_distance( const float* a, const float* b)
{
    float x = a[0] - b[0], y=a[1] - b[1], z = a[2] - b[2];
    return x * x + y * y + z * z;
}

