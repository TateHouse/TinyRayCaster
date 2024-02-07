#include "FrameBuffer.hpp"

#include <stdexcept>

namespace TinyRayCaster {
FrameBuffer::FrameBuffer(const unsigned int width, const unsigned int height, const Color& backgroundColor) :
		width {width}, height {height} {
	if (width == 0 || height == 0) {
		throw std::invalid_argument("Width and height must be greater than 0.");
	}
	
	pixels = std::vector<unsigned int>(width * height, backgroundColor.getColor());
}

void FrameBuffer::clear(const TinyRayCaster::Color& color) {
	pixels = std::vector<unsigned int>(width * height, color.getColor());
}

void FrameBuffer::setPixel(unsigned int x, unsigned int y, const TinyRayCaster::Color& color) {
	if (x >= width || y >= height) {
		throw std::out_of_range("Pixel coordinates out of range.");
	}
	
	const auto pixelIndex {y * width + x};
	pixels[pixelIndex] = color.getColor();
}

void FrameBuffer::drawRectangle(const unsigned int x,
                                const unsigned int y,
                                const unsigned int rectangleWidth,
                                const unsigned int rectangleHeight,
                                const TinyRayCaster::Color& color) {
	if (x >= width || y >= height) {
		throw std::out_of_range("Rectangle coordinates out of range.");
	}
	
	for (std::size_t xIndex {0}; xIndex < rectangleWidth; ++xIndex) {
		for (std::size_t yIndex {0}; yIndex < rectangleHeight; ++yIndex) {
			const auto pixelX {x + xIndex};
			const auto pixelY {y + yIndex};
			
			if (pixelX >= width || pixelY >= height) {
				continue;
			}
			
			const auto pixelIndex {pixelY * width + pixelX};
			pixels[pixelIndex] = color.getColor();
		}
	}
}

unsigned int FrameBuffer::getWidth() const {
	return width;
}

unsigned int FrameBuffer::getHeight() const {
	return height;
}

const std::vector<unsigned int>& FrameBuffer::getPixels() const {
	return pixels;
}
}