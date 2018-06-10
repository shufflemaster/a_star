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

class Location
{
public:
	Location(int row, int col, uint32_t id, uint32_t costG= MAXDWORD32, uint32_t costH = MAXDWORD32) :
		mRow(row), mCol(col), mId(id), mGcost(costG), mHcost(costH) {
		calcCostF();
	}

	bool operator==(const Location& other) {
		return (mRow == other.mRow) && (mCol == other.mCol);
	}

	friend std::ostream& operator<<(std::ostream& os, const Location& loc)
	{
		os << "Loc [row:" << loc.mRow << ", col:" << loc.mCol << "]";
		return os;
	}

	void setCosts(uint32_t costG, uint32_t costH= MAXDWORD32) {
		mGcost = costG;
		mHcost = costH;
		calcCostF();
	}

	uint32_t getCostG() { return mGcost; }
	uint32_t getCostH() { return mHcost; }
	uint32_t getCostF() { return mFcost; }

	int mRow;
	int mCol;
	uint32_t mId;

private:
	Location();

	uint32_t mGcost; //Added for Dijkstra's algorithm.
	uint32_t mHcost; //Added for A* algorithm.
	uint32_t mFcost; //Added for A* Algorithm. mFcost = mGcost + mHcost.

	void calcCostF() {
		if ((mHcost == MAXDWORD32) || ((mGcost == MAXDWORD32))) {
			mFcost = MAXDWORD32;
		}
		else {
			mFcost = mGcost + mHcost;
		}
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

