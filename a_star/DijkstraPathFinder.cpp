#include "stdafx.h"
#include "DijkstraPathFinder.h"

#include "Bitmap.h"

using namespace std;

#define EMPTY_COLOR   0xFFFFFFFF
#define VISITED_COLOR 0xFFFF0000

#define EDGE_COST 1

DijkstraPathFinder::DijkstraPathFinder()
{
}


DijkstraPathFinder::~DijkstraPathFinder()
{
}

const list<shared_ptr<Location>> & DijkstraPathFinder::find(Bitmap& bmp, bool paintSearch,
	shared_ptr<Location> startLoc, shared_ptr<Location> endLoc)
{
	clear(bmp.width(), bmp.height());

	mPerfCounter.start();

	//Make sure startLoc cost is 0.
	startLoc->mCost = 0;
	mPriorityQueue.push(startLoc);

	while (!mPriorityQueue.empty()) {
		shared_ptr<Location> loc = mPriorityQueue.top();
		mPriorityQueue.pop();

		if (*loc.get() == *endLoc.get()) {
			//Found the path
			mPerfCounter.stop();
			buildThePath(startLoc, loc);
			break;
		}


		//Ok, this location has not been visited. Mark it blue.
		if (paintSearch) bmp.setPixel(loc->mRow, loc->mCol, VISITED_COLOR);

		pushQueue(bmp, loc->mRow - 1, loc->mCol, loc);
		pushQueue(bmp, loc->mRow - 1, loc->mCol + 1, loc);
		pushQueue(bmp, loc->mRow, loc->mCol + 1, loc);
		pushQueue(bmp, loc->mRow + 1, loc->mCol + 1, loc);
		pushQueue(bmp, loc->mRow + 1, loc->mCol, loc);
		pushQueue(bmp, loc->mRow + 1, loc->mCol - 1, loc);
		pushQueue(bmp, loc->mRow, loc->mCol - 1, loc);
		pushQueue(bmp, loc->mRow - 1, loc->mCol - 1, loc);

		mVisited[loc->mId] = true;
	}


	return mPath;
}

void DijkstraPathFinder::clear(uint32_t w, uint32_t h)
{
	while (!mPriorityQueue.empty()) {
		mPriorityQueue.pop();
	}
	mPath.clear();
	mPrevs.clear();
	mVisited.clear();

	mVisited.resize(w * h);
	for (size_t i = 0; i < mVisited.size(); i++) {
		mVisited[i] = false;
	}
}

void DijkstraPathFinder::pushQueue(Bitmap& bmp, int locRow, int locCol, shared_ptr<Location> prevLoc)
{
	if ((locRow < 0) || (locRow >= (int)bmp.height())) {
		return;
	}

	if ((locCol < 0) || (locCol >= (int)bmp.width())) {
		return;
	}

	//Get the current color.
	uint32_t locColor = bmp.getPixel(locRow, locCol);
	if (locColor != EMPTY_COLOR) {
		//This location is a wall.
		return;
	}

	uint32_t locId = locRow * bmp.width() + locCol;

	if (mVisited[locId]) {
		//cout << "VISITED r:" << locRow << ", c:" << locCol << endl;
		return;
	}

	//REMARK: It is essential to not only check if the device has been visited (code Above),
	//but also it is important to not reenqueue a location.
	auto itor = mPrevs.find(locId);
	shared_ptr<Location> loc;
	bool shouldPush = false;
	if (itor == mPrevs.end()) {
		//This location has not been created before, nor visited. Let's create it with maximum cost.
		loc = make_shared<Location>(locRow, locCol, locId);
		mPrevs[locId] = prevLoc;
		shouldPush = true;
	}
	else {
		loc = itor->second;
	}

	uint32_t newCost = prevLoc->mCost + EDGE_COST;
	if (newCost < loc->mCost) {
		loc->mCost = newCost;
		mPrevs[locId] = prevLoc;
		//Because we updated the cost of this node,
		//we need to resort the min priority queue.
		if (!shouldPush) {
			mPriorityQueue.resort();
		}
	}

	if (shouldPush) mPriorityQueue.push(loc);
}

void DijkstraPathFinder::buildThePath(shared_ptr<Location> startLoc, shared_ptr<Location> loc)
{
	mPath.push_front(loc);

	auto it = mPrevs.find(loc->mId);
	while ((loc->mId != startLoc->mId) && (it != mPrevs.end())) {
		loc = it->second;
		mPath.push_front(loc);
		it = mPrevs.find(loc->mId);
	}
}
