# bmp-filters
## A C++ implementation of some image filters for use with .bmp files

read-bmp.hpp - can be used to read and print .bmp file headers and info headers, also features a pixel struct to store 24 bit pixel values.


bmp-filter.hpp - pulls from read-bmp.hpp and contains functions for three image filters:
* Greyscale
* Sepia
* Blur


filter-bmp.cpp - creates a CLI for users to choose an image to apply filters to. A filtered copy of the image is created.
