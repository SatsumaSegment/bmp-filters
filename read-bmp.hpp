#include <iostream>
#include <fstream>

using namespace std;

// Define some datatypes
typedef uint8_t     BYTE; // 1 byte unsigned
typedef uint16_t    DOUBLEBYTE; // 2 bytes unsigned
typedef uint32_t    QUADBYTE; // 4 bytes unsigned
typedef int32_t     SQUADBYTE; // 4 bytes signed

// BMP type signature (usually 'BM')
struct __attribute__((__packed__)) BMPTYPE {
    BYTE data[2];
    BMPTYPE() { data[0] = data[1] = 0; }
};

// BMP header
struct __attribute__((__packed__)) BMPHEADER {
    QUADBYTE fileSize; // File size in bytes
    DOUBLEBYTE reserved1; // Reserved bytes
    DOUBLEBYTE reserved2; // Reserved bytes
    QUADBYTE dataOffset; // Data offset (bytes before pixel data begins)
    BMPHEADER() : fileSize(0),  reserved1(0), reserved2(0), dataOffset(0) { }
};

// BMP info header
struct __attribute__((__packed__)) BMPINFOHEADER {
    QUADBYTE headerSize; // Info header size
    SQUADBYTE width; // Image width in pixels
    SQUADBYTE height; // Image height in pixels
    DOUBLEBYTE planes; // Number of color planes (should be '1')
    DOUBLEBYTE bpp; // Number of bits per pixel
    QUADBYTE compression; // Compression type ('0' = no compression)
    QUADBYTE sizeImage; // Size of raw bitmap data ('0' for BI_RGB bitmaps)
    QUADBYTE xppm; // X-axis pixels per meter
    QUADBYTE yppm; // Y-axis pixels per meter
    QUADBYTE colors; // Colors in color pallet ('0' = default)
    QUADBYTE impColors; // Number of important colors ('0' if every color is important)
    BMPINFOHEADER() : headerSize(0), width(0), height(0), planes(0), bpp(0), compression(0), sizeImage(0), xppm(0), yppm(0), colors(0), impColors(0) { } 
};

// 24 Bit pixel data (use to read and write pixel data)
struct __attribute__((__packed__)) PIXEL {
    BYTE rgbtBlue; // Blue
    BYTE rgbtGreen; // Green
    BYTE rgbtRed; // Red
    PIXEL() : rgbtBlue(0), rgbtGreen(0), rgbtRed(0) { }
};

// Function to read BMP header
void ReadHeader(ifstream &infile, BMPTYPE &type, BMPHEADER &header) {
    // Check file exists
    if (!infile) {
        cerr << "ERROR: Invalid file" << endl;
        return;
    }

    // Go to begining of file
    infile.seekg(0, ios::beg);
    
    // Read type and header
    infile.read((char*) &type, sizeof(BMPTYPE));
    infile.read((char*) &header, sizeof(BMPHEADER));
}

// Function to read BMP info header
void ReadInfoHeader(ifstream &infile, BMPINFOHEADER &infoheader) {
    // Check file exists
    if (!infile) {
        cerr << "ERROR: Invalid file" << endl;
        return;
    }

    // Go to beginning of info header
    infile.seekg(14, ios::beg);
    
    // Read info header
    infile.read((char*) &infoheader, sizeof(BMPINFOHEADER));
}

// Function to print BMP header
void PrintHeader(BMPTYPE type, BMPHEADER header) {
    cout << "\n";
    cout << "##### BMP HEADER #####" << endl;
    cout << "* Signature/Type  : " << type.data[0] << type.data[1] << endl;
    cout << "* File Size  : " << header.fileSize << " bytes" << endl;
    cout << "* Reserved1  : " << header.reserved1 << endl;
    cout << "* Reserved2  : " << header.reserved2 << endl;
    cout << "* Data Offset : " << header.dataOffset << " bytes" <<endl;
}

// Function to print BMP info header
void PrintInfoHeader(BMPINFOHEADER infoheader) {
    cout  << "\n";
    cout << "##### BMP INFO HEADER #####" << endl;
    cout << "* Header Size : " << infoheader.headerSize << " bytes" << endl;
    cout << "* Width : " << infoheader.width << " pixel(s)" << endl;
    cout << "* Height : " << infoheader.height << " pixel(s)" << endl;
    cout << "* Planes : " << infoheader.planes << endl;
    cout << "* Bits Per Pixel : " << infoheader.bpp << " bits" << endl;
    cout << "* Compression Type : " << infoheader.compression << endl;
    cout << "* Raw Size : " << infoheader.sizeImage << " bytes" << endl;
    cout << "* X Pixels Per Meter : " << infoheader.xppm << " pixel(s)" << endl;
    cout << "* Y Pixels Per Meter : " << infoheader.yppm << " pixel(s)" << endl;
    cout << "* Colors : " << infoheader.colors << endl;
    cout << "* Important Colors : " << infoheader.impColors << endl;
}
