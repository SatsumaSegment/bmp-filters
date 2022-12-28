#include <iostream>
#include <fstream>
#include "bmp-filter.hpp" // Includes read-bmp.hpp

using namespace std;

int main() {
    ifstream refimg; // Reference image
    ofstream outimg; // Output image

    // Ask user for reference file
    char refname[50];
    cout << "Enter BMP image: ";
    cin >> refname;

    // Open input file
    refimg.open(refname, ios::binary);

    // Check file exists
    if (refimg.fail()) {
        cout << "File does not exist!";
        return 1;
    }

    // Ask user which filter to apply
    char filter;
    do {
        cout << "Choose a filter:" << endl;
        cout << "* Greyscale 'g'" << endl;
        cout << "* Sepia Greyscale 's'" << endl;
        cout << "* Blur 'b'" << endl;
        cout << ">>> ";
        cin >> filter;
    } while ((filter != 'g') && (filter != 's') && (filter != 'b'));

    // Read headers from background file
    BMPTYPE t;
    BMPHEADER h;
    BMPINFOHEADER ih;
    ReadHeader(refimg, t, h);
    ReadInfoHeader(refimg, ih);

    // Write headers to new file
    outimg.open("filtered-img.bmp", ios::binary);
    outimg.write((char *) &t, sizeof(BMPTYPE));
    outimg.write((char *) &h, sizeof(BMPHEADER));
    outimg.write((char *) &ih, sizeof(BMPINFOHEADER));

    // Seek pixel data in image
    refimg.seekg(ih.headerSize + 14, ios::beg);
    outimg.seekp(ih.headerSize + 14, ios::beg);

    // Apply filter
    if ((filter == 'g')) {
        Greyscale(ih.height, ih.width, refimg, outimg);
        refimg.close();
        outimg.close();
        return 0;
    }
    else if (filter == 's') {
        Sepia(ih.height, ih.width, refimg, outimg);
        refimg.close();
        outimg.close();
        return 0;
    }
    else if (filter == 'b') {
        Blur(ih.height, ih.width, refimg, outimg);
        refimg.close();
        outimg.close();
        return 0;
    }

    refimg.close();
    outimg.close();
    return 0;

}