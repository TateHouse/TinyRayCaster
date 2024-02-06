#pragma once

#include <vector>

#include "Color.hpp"

namespace TinyRayCaster {
class FrameBuffer final {
public:
	FrameBuffer(const unsigned int width, const unsigned int height, const Color& backgroundColor);
	FrameBuffer(const FrameBuffer& other) = default;
	FrameBuffer(FrameBuffer&& other) = default;
	~FrameBuffer() = default;

public:
	FrameBuffer& operator=(const FrameBuffer& other) = default;
	FrameBuffer& operator=(FrameBuffer&& other) = default;

public:
	void clear(const Color& color);
	void setPixel(const unsigned int x, const unsigned int y, const Color& color);
	void drawRectangle(const unsigned int x,
	                   const unsigned int y,
	                   const unsigned int rectangleWidth,
	                   const unsigned int rectangleHeight,
	                   const Color& color);

private:
	unsigned int width;
	unsigned int height;
	std::vector<unsigned int> pixels;
};
}