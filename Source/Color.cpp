#include "Color.hpp"

namespace TinyRayCaster {
Color::Color(const std::byte red, const std::byte green, const std::byte blue, const std::byte alpha) : color {
		(static_cast<unsigned int>(red) << 0) +
		(static_cast<unsigned int>(green) << 8) +
		(static_cast<unsigned int>(blue) << 16) +
		(static_cast<unsigned int>(alpha) << 24)} {
	
}

Color::Color(const unsigned int color) : color {color} {

}

unsigned int Color::getColor() const {
	return color;
}

std::byte Color::getRed() const {
	return static_cast<std::byte>((color >> 0) & 255);
}

std::byte Color::getGreen() const {
	return static_cast<std::byte>((color >> 8) & 255);
}

std::byte Color::getBlue() const {
	return static_cast<std::byte>((color >> 16) & 255);
}

std::byte Color::getAlpha() const {
	return static_cast<std::byte>((color >> 24) & 255);
}
}