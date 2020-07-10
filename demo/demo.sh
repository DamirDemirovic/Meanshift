#!/bin/bash

# input parameters for segmentation and filtering

s_radius=7        # spatial radius       
c_radius=6.5      # color radius
m_reg=20          # minimal region


### Segmentation and filtering demo

../bin/meanshift images/boat.png $s_radius $c_radius $m_reg "results/segment/boat"_"$s_radius"_"$c_radius"_"$m_reg.png"  "results/filter/boat"_"$s_radius"_"$c_radius"_"$m_reg.png" 
../bin/meanshift images/cameraman.png $s_radius $c_radius $m_reg "results/segment/cameraman"_"$s_radius"_"$c_radius"_"$m_reg.png"  "results/filter/cameraman"_"$s_radius"_"$c_radius"_"$m_reg.png" 
../bin/meanshift images/house.png $s_radius $c_radius $m_reg "results/segment/house"_"$s_radius"_"$c_radius"_"$m_reg.png" "results/filter/house"_"$s_radius"_"$c_radius"_"$m_reg.png" 
../bin/meanshift images/mandrill.png $s_radius $c_radius $m_reg "results/segment/mandrill"_"$s_radius"_"$c_radius"_"$m_reg.png"  "results/filter/mandrill"_"$s_radius"_"$c_radius"_"$m_reg.png" 
../bin/meanshift images/peppers.png $s_radius $c_radius $m_reg "results/segment/peppers"_"$s_radius"_"$c_radius"_"$m_reg.png"  "results/filter/peppers"_"$s_radius"_"$c_radius"_"$m_reg.png" 

