// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdint>
#include <iostream>
using namespace std;

#include "Bitmap.h"
#include "BFSPathFinder.h"

#define VERSION "1.0.0"

#define COLOR_PATH 0xFF0000FF

static void colorPath(Bitmap& bmp, const list<shared_ptr<Location>> & lst, uint32_t color)
{
	for (auto l : lst) {
		cout << "Loc:[" << l->mRow << ", " << l->mCol << "]" << endl;
		bmp.setPixel(l->mRow, l->mCol, color);
	}
}

static void findPath(PathFinder& finder, const char * filename,
		uint32_t startRow, uint32_t startCol,
		uint32_t endRow, uint32_t endCol)
{
	Bitmap bmp(filename);
	cout << "Got bmp of width:" << bmp.width() << ", height:" << bmp.height() << endl;

	if (bmp.width() < 1) {
		cout << "Bitmap file:" << filename << " is invalid." << endl;
		return;
	}

	const list<shared_ptr<Location>> & lst = finder.find(bmp, true,
		make_shared<Location>(startRow, startCol, startRow * bmp.width() + startCol),
		make_shared<Location>(endRow, endCol, endRow * bmp.width() + endCol));
	if (lst.size() < 1) {
		cout << "No path was found" << endl;
		return;
	}

	cout << "Found a path" << endl;
	colorPath(bmp, lst, COLOR_PATH);

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

	BFSPathFinder finder;
	findPath(finder, filename, startRow, startCol, endRow, endCol);

    return 0;
}

