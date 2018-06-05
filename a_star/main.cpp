// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdint>
#include <iostream>
using namespace std;

#include "Bitmap.h"


static void findPath(const char * filename,
		uint32_t startRow, uint32_t startCol,
		uint32_t endRow, uint32_t endCol)
{
	Bitmap bmp(filename);
	cout << "Got bmp of width:" << bmp.width() << ", height:" << bmp.height() << endl;

	if (bmp.width() < 1) {
		return;
	}

	char name[256];
	snprintf(name, 256, "resaved_%u_%u_32bpp.data", bmp.width(), bmp.height());
	bmp.saveAs(name);
}

int main()
{
	const char * filename = "testmap.bmp";
	uint32_t startRow = 20;
	uint32_t startCol = 10;
	uint32_t endRow = 25;
	uint32_t endCol = 42;

	findPath(filename, startRow, startCol, endRow, endCol);

    return 0;
}

