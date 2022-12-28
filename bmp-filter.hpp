#include <iostream>
#include <fstream>
#include <math.h>
#include "read-bmp.hpp"

using namespace std;

void Greyscale(int h, int w, ifstream &inimg, ofstream &oimg) {
    float average;
    PIXEL p;
    for (int y = 0; y < abs(h); y++) {
        for (int x = 0; x < abs(w); x++) {
            inimg.read((char *) &p, sizeof(PIXEL));
            average = p.rgbtBlue + p.rgbtGreen + p.rgbtRed;
            average = round(average / 3);
            p.rgbtBlue = average;
            p.rgbtGreen = average;
            p.rgbtRed = average;
            oimg.write((char *) &p, sizeof(PIXEL));
        }
    }
    return;
}

void Sepia(int h, int w, ifstream &inimg, ofstream &oimg) {
    int b;
    int g;
    int r;
    PIXEL p;
    for (int y = 0; y < abs(h); y++) {
        for (int x = 0; x < abs(w); x++) {
            inimg.read((char *) &p, sizeof(PIXEL));
            b = round(.272 * p.rgbtRed + .534 * p.rgbtGreen + .131 * p.rgbtBlue);
            g = round(.349 * p.rgbtRed + .686 * p.rgbtGreen + .168 * p.rgbtBlue);
            r = round(.393 * p.rgbtRed + .769 * p.rgbtGreen + .189 * p.rgbtBlue);
            if (b > 255)
                b = 255;
            if (g > 255)
                g = 255;
            if (r > 255)
                r = 255;
            p.rgbtBlue = b;
            p.rgbtGreen = g;
            p.rgbtRed = r;
            oimg.write((char *) &p, sizeof(PIXEL));
        }
    }
    return;
}

void Blur(int h, int w, ifstream &inimg, ofstream &oimg) {
    h = abs(h);
    w = abs(w);
    // Copy input file's pixels to PIXEL array
    PIXEL p;
    PIXEL copy[h][w];
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            inimg.read((char *) &p, sizeof(PIXEL));
            copy[y][x] = p;
        }
    }

    // Create space for getting pixel values
    float average_red = 0;
    float average_green = 0;
    float average_blue = 0;
    int n_pixels = 0; // number of pixels to consider
    int k_w = 0; // Kernel width (starting x coordinate)
    int k_h = 0; // kernel height (starting y coordinate)
    int m_w = 0; // max width
    int m_h = 0; // max height

    // Iterate through PIXEL array
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            // For each pixel, run the kernel; consider corner and edge cases
            // Center
            if (i > 0 && i < h - 1 && j > 0 && j < w - 1) {
                k_h = -1;
                k_w = -1;
                m_h = 1;
                m_w = 1;
            }
            // Top left
            else if (i == 0 && j == 0) {
                k_h = 0;
                k_w = 0;
                m_h = 1;
                m_w = 1;
            }
            // Top right
            else if (i == 0 && j == w - 1) {
                k_h = 0;
                k_w = -1;
                m_h = 1;
                m_w = 0;
            }
            // Bottom left
            else if (i == h - 1 && j == 0) {
                k_h = -1;
                k_w = 0;
                m_h = 0;
                m_w = 1;
            }
            else if (i == h - 1 && j == w - 1) // Bottom right
            {
                k_h = -1;
                k_w = -1;
                m_h = 0;
                m_w = 0;
            }
            // Top
            else if (i == 0 && j > 0 && j < w - 1) {
                k_h = 0;
                k_w = -1;
                m_h = 1;
                m_w = 1;
            }
            // Right
            else if (i > 0 && i < h - 1 && j == w - 1) {
                k_h = -1;
                k_w = -1;
                m_h = 1;
                m_w = 0;
            }
            // Bottom
            else if (i == h - 1 && j > 0 && j < w - 1) {
                k_h = -1;
                k_w = -1;
                m_h = 0;
                m_w = 1;
            }
            // Left
            else if (i < h - 1 && i > 0 && j == 0) {
                k_h = -1;
                k_w = 0;
                m_h = 1;
                m_w = 1;
            }

            // Kernel loop
            for (int y = k_h; y <= m_h; y++) {
                for (int x = k_w; x <= m_w; x++) {
                    n_pixels++;
                    average_red += copy[i + y][j + x].rgbtRed;      //  Kernel  -1,-1   -1,0   -1,1
                    average_green += copy[i + y][j + x].rgbtGreen;  //  map     0,-1    0,0     0,1
                    average_blue += copy[i + y][j + x].rgbtBlue;    //          1,-1    1,0     1,1
                }
            }

            // Set pixel values
            average_red = round(average_red / n_pixels);
            average_green = round(average_green / n_pixels);
            average_blue = round(average_blue / n_pixels);
            copy[i][j].rgbtRed = average_red;
            copy[i][j].rgbtGreen = average_green;
            copy[i][j].rgbtBlue = average_blue;

            // Write pixel data
            oimg.write((char *) &copy[i][j], sizeof(PIXEL));

            // Reset values
            average_red = 0;
            average_green = 0;
            average_blue = 0;

            n_pixels = 0;
        }
    }
    return;
}