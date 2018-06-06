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

	const list<shared_ptr<Location>> & find(Bitmap& bmp, bool paintSearch, shared_ptr<Location> startLoc, shared_ptr<Location> endLoc);

private:
	list<shared_ptr<Location>> mPath;
	queue<shared_ptr<Location>> mSearchQueue;
	//Here we record if we have visited a location or not.
	vector<bool> mVisited;
	//This is a map that given a location id, it returns its previous location.
	map<uint32_t, shared_ptr<Location>> mPrevs;

	void clear(uint32_t w, uint32_t h);
	void pushQueue(Bitmap& bmp, int locRow, int locCol, shared_ptr<Location> prevLoc);
	void buildThePath(shared_ptr<Location> startLoc, shared_ptr<Location> endLoc);
};

