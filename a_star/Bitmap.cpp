#include "stdafx.h"
#include "Bitmap.h"
#include <iostream>
#include <Windows.h>

using namespace std;

Bitmap::Bitmap(const char * filename)
{
	openFile(filename);
}

Bitmap::~Bitmap()
{
}

/*
-		bih	{biSize=0x00000028 biWidth=0x00000040 biHeight=0x00000040 ...}	tagBITMAPINFOHEADER
biSize	0x00000028	unsigned long
biWidth	0x00000040	long
biHeight	0x00000040	long
biPlanes	0x0001	unsigned short
biBitCount	0x0018	unsigned short
biCompression	0x00000000	unsigned long
biSizeImage	0x00003000	unsigned long
biXPelsPerMeter	0x00000000	long
biYPelsPerMeter	0x00000000	long
biClrUsed	0x00000000	unsigned long
biClrImportant	0x00000000	unsigned long
*/
bool Bitmap::openFile(const char * filename)
{
	FILE* fp = nullptr;
	bool retVal = false;

	do {
		fp = fopen(filename, "rb");
		if (!fp) {
			cout << "Failed to open " << filename << endl;
			break;
		}

		BITMAPFILEHEADER bfh;
		size_t readBytes = fread(&bfh, 1, sizeof(bfh), fp);
		if (readBytes != sizeof(bfh)) {
			cout << "From file " << filename << ", read only " << readBytes << " bytes, but was expecting " << sizeof(bfh) << " bytes" << endl;
			break;
		}

		if (bfh.bfType != 'MB') {
			cout << "File " << filename << ", is not BM type. Got " << hex << bfh.bfType << endl;
			break;
		}

		BITMAPINFOHEADER bih;
		uint32_t headerBytes = bfh.bfOffBits - sizeof(BITMAPFILEHEADER);
		cout << "Header byte count before pixels is:" << headerBytes << ", size of BITMAPINFOHEADER is:" << sizeof(bih) << endl;
		readBytes = fread(&bih, 1, headerBytes, fp);
		if (readBytes != headerBytes) {
			cout << "Bullshit! read:" << readBytes << " bytes, but was expecting:" << headerBytes << " bytes" << endl;
			break;
		}

		if (bih.biCompression != BI_RGB) {
			cout << "File:" << filename << " has an unsupported compression format:" << bih.biCompression << ", I support only BI_RGB:" << BI_RGB << endl;
			break;
		}

		if ((bih.biBitCount != 24) || (bih.biBitCount != 32)) {
			cout 
			break;
		}

		//resize vector of pixels.
		pixels.resize(bih.biWidth * bih.biHeight);
		uint32_t writeOffset = (bih.biHeight - 1) * bi.biWidth;
		uint32_t rowSizeBytes = 
		for (uint32_t row = 0; row < bih.biHeight; row++) {
			for (uint32_t col = 0; col < bih.biWidth; col++) {
				
			}
		}

		retVal = true;
	} while (0);

	if (fp) fclose(fp);
	return retVal;
}