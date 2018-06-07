#pragma once

#include <cstdint>
#include <list>
#include <queue>
#include <iostream>
#include <memory>
#include <map>

#include "PerfCounter.h"

using namespace std;

class Bitmap;

struct Location
{
	Location(int row, int col, uint32_t id, uint32_t cost=0xFFFFFFFF) : mRow(row), mCol(col), mId(id), mCost(cost){
	}

	int mRow;
	int mCol;
	uint32_t mId;
	uint32_t mCost; //Added for Dijkstra's algorithm.

	bool operator==(const Location& other) {
		return (mRow == other.mRow) && (mCol == other.mCol);
	}

	friend std::ostream& operator<<(std::ostream& os, const Location& loc)
	{
		os << "Loc [row:" << loc.mRow << ", col:" << loc.mCol << "]";
		return os;
	}
};

class PathFinder
{
public:
	PathFinder();
	virtual ~PathFinder();

	virtual const list<shared_ptr<Location>> & find(Bitmap& bmp, bool paintSearch, shared_ptr<Location> startLoc, shared_ptr<Location> endLoc) = 0;
	const CPerfCounter& getPerfCounter() { return mPerfCounter; }

protected:
	CPerfCounter mPerfCounter;
};

