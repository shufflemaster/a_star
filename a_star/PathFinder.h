#pragma once

#include <cstdint>
#include <list>
#include <queue>
#include <iostream>
#include <memory>
#include <map>

using namespace std;

class Bitmap;

struct Location
{
	Location(int row, int col, uint32_t id) : mRow(row), mCol(col), mId(id){
	}

	int mRow;
	int mCol;
	uint32_t mId;

	bool operator==(const Location& other) {
		return (mRow == other.mRow) && (mCol == other.mCol);
	}
};

class PathFinder
{
public:
	PathFinder();
	virtual ~PathFinder();

	virtual const list<shared_ptr<Location>> & find(Bitmap& bmp, bool paintSearch, shared_ptr<Location> startLoc, shared_ptr<Location> endLoc) = 0;
};

