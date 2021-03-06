/*
 * Copyright (c) 2018, Damir Demirović <damir.demirovic@untz.ba>
 * All rights reserved.
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later
 * version. You should have received a copy of this license along
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IMAGE_H
#define IMAGE_H


#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

typedef unsigned char uchar;

uchar *AllocateUcharImage(int width, int height, int nchannel);
uchar GetPixel(uchar *im, int width, int height, int x, int y, int channel);
void SetPixel(uchar *im, int width, int height, int x, int y, const uchar val, int channel);
int** GenerateLabels(size_t width, size_t height);
void LabelImage(uchar *res, int width, int height, int** labels, int regCount);
int range_distance(uchar* image, int width, int height, int x1, int y1, int x2, int y2 );
uchar *ConvertRGB2LUV(uchar * input, int width, int height, int nchannel);
uchar *ConvertLUV2RGB(uchar * origin, int width, int height, int nchannel);
float color_distance( const float* a, const float* b);
std::vector<int> GenerateRandomNumbers(int num);


#endif /* IMAGE_H */

