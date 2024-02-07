#include "Map.hpp"

#include <stdexcept>

namespace TinyRayCaster {
int Map::getCell(const unsigned int x, const unsigned int y) const {
	if (x >= width || y >= height) {
		throw std::out_of_range("Map cell out of range.");
	}
	
	const auto mapIndex {y * width + x};
	return map[mapIndex] - '0';
}

bool Map::isCellEmpty(const unsigned int x, const unsigned int y) const {
	if (x >= width || y >= height) {
		throw std::out_of_range("Map cell out of range.");
	}
	
	const auto mapIndex {y * width + x};
	return map[mapIndex] == ' ';
}

unsigned int Map::getWidth() const {
	return width;
}

unsigned int Map::getHeight() const {
	return height;
}
}