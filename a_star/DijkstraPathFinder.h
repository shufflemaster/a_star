#pragma once
#include "PathFinder.h"
#include "MinPriorityQueue.h"

struct DijkstraCompare {
	bool operator()(const shared_ptr<Location>& left, const shared_ptr<Location>& right)
	{
		//REMARK: use < for max priority queue
		//        use >= for min priority queue.
		return left->getCostG() > right->getCostG();
	}
};


class DijkstraPathFinder :
	public PathFinder
{
public:
	DijkstraPathFinder();
	virtual ~DijkstraPathFinder();
	const list<shared_ptr<Location>> & find(Bitmap& bmp, bool paintSearch, shared_ptr<Location> startLoc, shared_ptr<Location> endLoc);

private:
	//The resulting path is stored here.
	list<shared_ptr<Location>> mPath;

	MinPriorityQueue<shared_ptr<Location>, std::vector<shared_ptr<Location>>, DijkstraCompare> mPriorityQueue;

	//Here we record if we have visited a location or not.
	vector<bool> mVisited;
	//This is a map that given a location id, it returns its previous location.
	map<uint32_t, shared_ptr<Location>> mPrevs;

	void clear(uint32_t w, uint32_t h);
	void pushQueue(Bitmap& bmp, int locRow, int locCol, shared_ptr<Location> prevLoc);
	void buildThePath(shared_ptr<Location> startLoc, shared_ptr<Location> endLoc);
};

