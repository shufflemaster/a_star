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
	inline uint32_t getPixel(uint32_t r, uint32_t c) { return pixels[r*mW + c]; }
	inline void setPixel(uint32_t r, uint32_t c, uint32_t color) { pixels[r*mW + c] = color; }

private:
	uint32_t mW;
	uint32_t mH;
	vector<uint32_t> pixels;

	Bitmap();
	bool openFile(const char * filename);
};

