#pragma once
#include "PathFinder.h"
#include "MinPriorityQueue.h"

struct AstarCompare {
	bool operator()(const shared_ptr<Location>& left, const shared_ptr<Location>& right)
	{
		//REMARK: use < for max priority queue
		//        use >= for min priority queue.
		return left->getCostF() > right->getCostF();
	}
};


class AstarPathFinder :
	public PathFinder
{
public:
	AstarPathFinder();
	virtual ~AstarPathFinder();
	const list<shared_ptr<Location>> & find(Bitmap& bmp, bool paintSearch, shared_ptr<Location> startLoc, shared_ptr<Location> endLoc);

private:
	//The resulting path is stored here.
	list<shared_ptr<Location>> mPath;

	MinPriorityQueue<shared_ptr<Location>, std::vector<shared_ptr<Location>>, AstarCompare> mPriorityQueue;

	//Here we record if we have visited a location or not.
	vector<bool> mVisited;
	//This is a map that given a location id, it returns its previous location.
	map<uint32_t, shared_ptr<Location>> mPrevs;

	//We need to store the destination to minimize passing parameters into functions.
	//This is important because in A* we calculate the H(heuristic) function which is usually
	//defined as a function of the current location and the destination.
	shared_ptr<Location> mDestination;

	void clear(uint32_t w, uint32_t h);
	void pushQueue(Bitmap& bmp, int locRow, int locCol, shared_ptr<Location>& prevLoc);
	void buildThePath(shared_ptr<Location> startLoc, shared_ptr<Location> endLoc);
	uint32_t calcCostH(int fromRow, int fromCol);
};



