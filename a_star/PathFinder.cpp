#include "stdafx.h"
#include "PathFinder.h"
#include <queue>

#include "Bitmap.h"

using namespace std;

#define EMPTY_COLOR   0xFFFFFFFF
#define VISITED_COLOR 0xFFFF0000


PathFinder::PathFinder()
{
}


PathFinder::~PathFinder()
{
}

const list<shared_ptr<Location>> & PathFinder::find(Bitmap& bmp, bool paintSearch, shared_ptr<Location> startLoc, shared_ptr<Location> endLoc)
{
	clear(bmp.width(), bmp.height());

	mSearchQueue.push(startLoc);

	while (!mSearchQueue.empty()) {
		shared_ptr<Location> loc = mSearchQueue.front();
		mSearchQueue.pop();

		//Get the current color.
		uint32_t locColor = bmp.getPixel(loc->mRow, loc->mCol);
		if (locColor != EMPTY_COLOR) {
			//This location is a wall.
			continue;
		}

		if (*loc.get() == *endLoc.get()) {
			//Found the path
			buildThePath(startLoc, loc);
			break;
		}


		//Ok, this location has not been visited. Mark it blue.
		if (paintSearch) bmp.setPixel(loc->mRow, loc->mCol, VISITED_COLOR);
		mVisited[loc->mId] = true;

		pushQueue(bmp, loc->mRow - 1, loc->mCol, loc);
		pushQueue(bmp, loc->mRow - 1, loc->mCol + 1, loc);
		pushQueue(bmp, loc->mRow ,    loc->mCol + 1, loc);
		pushQueue(bmp, loc->mRow + 1, loc->mCol + 1, loc);
		pushQueue(bmp, loc->mRow + 1, loc->mCol, loc);
		pushQueue(bmp, loc->mRow + 1, loc->mCol - 1, loc);
		pushQueue(bmp, loc->mRow,     loc->mCol - 1, loc);
		pushQueue(bmp, loc->mRow - 1, loc->mCol - 1, loc);
	}

	return mPath;
}

void PathFinder::clear(uint32_t w, uint32_t h)
{
	mPath.clear();
	while (!mSearchQueue.empty()) {
		mSearchQueue.pop();
	}
	mVisited.clear();
	mPrevs.clear();

	mVisited.resize(w * h);
	for (size_t i = 0; i < mVisited.size(); i++) {
		mVisited[i] = false;
	}
}

void PathFinder::pushQueue(Bitmap& bmp, int locRow, int locCol, shared_ptr<Location> prevLoc)
{
	if ((locRow < 0) || (locRow >= (int)bmp.height())) {
		return;
	}

	if ((locCol < 0) || (locCol >= (int)bmp.width())) {
		return;
	}


	uint32_t locId = locRow * bmp.width() + locCol;

	if (mVisited[locId]) {
		//cout << "VISITED r:" << locRow << ", c:" << locCol << endl;
		return;
	}

	mSearchQueue.push(make_shared<Location>(locRow, locCol, locId));
	mPrevs[locId] = prevLoc;
}

void PathFinder::buildThePath(shared_ptr<Location> startLoc, shared_ptr<Location> loc)
{
	mPath.push_front(loc);

	auto it = mPrevs.find(loc->mId);
	while ((loc->mId != startLoc->mId) && (it != mPrevs.end())) {
		loc = it->second;
		mPath.push_front(loc);
		it = mPrevs.find(loc->mId);
	}
}