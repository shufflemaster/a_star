#pragma once

#include <cstdint>
#include <vector>

using namespace std;

class Bitmap
{
public:
	Bitmap(const char * filename);
	virtual ~Bitmap();

	inline uint32_t width() { return mW; }
	inline uint32_t height() { return mH; }
	inline uint32_t getPixel(uint32_t r, uint32_t c) { return mPixels[r*mW + c]; }
	inline void setPixel(uint32_t r, uint32_t c, uint32_t color) { mPixels[r*mW + c] = color; }

	bool saveAs(const char *filename);

private:
	uint32_t mW;
	uint32_t mH;
	vector<uint32_t> mPixels;

	Bitmap();
	bool openFile(const char * filename);
	void parseRow(uint32_t writeOffsetIdx, uint32_t bytesPerPixel, uint32_t numColumns, const uint8_t* pixData);
};

