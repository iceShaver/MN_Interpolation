#!/bin/bash
cmake . &&
make &&
./MN_Interpolation $1 $2 $3 &&
./make_plot.py cubic_spline_$1 points_$1 $1 &&
./make_plot.py lagrange_$1 points_$1 $1