#pragma once

#include  "PathFinder.h"

/*
Breadth First Search Implementation of a PathFinder.
This implementation is capable of finding the shortest path for
an unweighted graph. In other words, all edges have the same cost.
*/
class BFSPathFinder: public PathFinder
{
public:
	BFSPathFinder();
	virtual ~BFSPathFinder();

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

