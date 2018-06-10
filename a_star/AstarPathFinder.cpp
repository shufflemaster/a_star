#include "stdafx.h"
#include "AstarPathFinder.h"

#include "Bitmap.h"

using namespace std;

#define EMPTY_COLOR   0xFFFFFFFF
#define VISITED_COLOR 0xFFFF0000

#define EDGE_COST 1

AstarPathFinder::AstarPathFinder()
{
}


AstarPathFinder::~AstarPathFinder()
{
}

const list<shared_ptr<Location>> & AstarPathFinder::find(Bitmap& bmp, bool paintSearch,
	shared_ptr<Location> startLoc, shared_ptr<Location> endLoc)
{
	clear(bmp.width(), bmp.height());

	mPerfCounter.start();

	mDestination = endLoc;
	//Make sure startLoc cost is 0.
	startLoc->setCosts(0, 0);
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

void AstarPathFinder::clear(uint32_t w, uint32_t h)
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

void AstarPathFinder::pushQueue(Bitmap& bmp, int locRow, int locCol,
								shared_ptr<Location>& prevLoc)
{
	if ((locRow < 0) || (locRow >= (int)bmp.height())) {
		return;
	}

	if ((locCol < 0) || (locCol >= (int)bmp.width())) {
		return;
	}

	//Get the current color.
	uint32_t locColor = bmp.getPixel(locRow, locCol);
	if ((locColor != EMPTY_COLOR) && (locColor != VISITED_COLOR)){
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

	//Calculate the costG.
	uint32_t costG = prevLoc->getCostG() + EDGE_COST;
	//Calculate the costH.
	//Let's do manhattan distance.
	uint32_t costH = calcCostH(locRow, locCol);
	uint32_t costF = costG + costH;

	if (costF < loc->getCostF()) {
		loc->setCosts(costG, costH);
		mPrevs[locId] = prevLoc;
		//Because we updated the cost of this node,
		//we need to resort the min priority queue.
		if (!shouldPush) {
			mPriorityQueue.resort();
		}
	}

	if (shouldPush) mPriorityQueue.push(loc);
}

void AstarPathFinder::buildThePath(shared_ptr<Location> startLoc, shared_ptr<Location> loc)
{
	mPath.push_front(loc);

	auto it = mPrevs.find(loc->mId);
	while ((loc->mId != startLoc->mId) && (it != mPrevs.end())) {
		loc = it->second;
		mPath.push_front(loc);
		it = mPrevs.find(loc->mId);
	}
}

static inline uint32_t ABS(int val)
{
	if (val >= 0) return val;
	return (uint32_t)-val;
}

uint32_t AstarPathFinder::calcCostH(int fromRow, int fromCol)
{
	//Our Heuristic Cost will be based on manhattan distance to the
	//destination location.
	return ABS(mDestination->mRow - fromRow) + ABS(mDestination->mCol - fromCol);
}