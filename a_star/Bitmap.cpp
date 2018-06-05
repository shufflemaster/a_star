#include "stdafx.h"
#include "Bitmap.h"
#include <iostream>
#include <memory>
#include <Windows.h>

using namespace std;

static inline uint32_t align(uint32_t v, uint32_t a) {
	a = a - 1;
	return (v + a) & ~a;
}

Bitmap::Bitmap(const char * filename):mW(0), mH(0)
{
	openFile(filename);
}

Bitmap::~Bitmap()
{
}

bool Bitmap::saveAs(const char *filename)
{
	FILE* fp = nullptr;
	bool retVal = false;
	do {
		fp = fopen(filename, "wb");
		if (!fp) {
			cout << "Failed to create " << filename;
			break;
		}

		uint32_t numPixels = mW * mH;
		for (uint32_t i = 0; i < numPixels; i++) {
			uint32_t color = mPixels[i];
			fwrite(&color, sizeof(color), 1, fp);
		}

		retVal = true;
	} while (0);

	if (fp) {
		fclose(fp);
	}
	return retVal;
}

void Bitmap::parseRow(uint32_t writeOffsetIdx, uint32_t bytesPerPixel, uint32_t numColumns, const uint8_t* pixData)
{
	const RGBQUAD* pQuad;

	for (int i = 0; i < numColumns; i++) {
		pQuad = reinterpret_cast<const RGBQUAD*>(pixData);
		uint32_t color = 0;
		if (bytesPerPixel == 3) {
			color = 0xFF000000 | (pQuad->rgbRed << 16) | (pQuad->rgbGreen << 8) | pQuad->rgbBlue;
			pixData += 3;
		}
		else {
			color = (pQuad->rgbReserved << 24) | (pQuad->rgbRed << 16) | (pQuad->rgbGreen << 8) | pQuad->rgbBlue;
			pixData += 4;
		}
		mPixels[writeOffsetIdx + i] = color;
	}
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

		if ((bih.biBitCount != 24) && (bih.biBitCount != 32)) {
			cout << "File:" << filename << " has unsupported bits per pixel:" << bih.biBitCount << endl;
			break;
		}

		//resize vector of pixels.
		mPixels.resize(bih.biWidth * bih.biHeight);
		uint32_t writeOffset = (bih.biHeight - 1) * bih.biWidth;
		uint32_t bytesPerPixel = bih.biBitCount / 8;
		uint32_t rowSizeBytes = bytesPerPixel * bih.biWidth;
		unique_ptr<uint8_t[]> rowArray = make_unique<uint8_t[]>(rowSizeBytes);

		//Align to 4 bytes.
		rowSizeBytes = align(rowSizeBytes, 4);
		for (uint32_t row = 0; row < bih.biHeight; row++) {
			readBytes = fread(rowArray.get(), 1, rowSizeBytes, fp);
			if (readBytes != rowSizeBytes) {
				cout << "Error. read " << readBytes << " of " << rowSizeBytes << " at row " << row << endl;
				break;
			}
			parseRow(writeOffset, bytesPerPixel, bih.biWidth, rowArray.get());
			writeOffset -= bih.biWidth;
		}

		mW = bih.biWidth;
		mH = bih.biHeight;
		retVal = true;
	} while (0);

	if (fp) fclose(fp);
	return retVal;
}