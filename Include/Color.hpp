#pragma once

#include <cstddef>

namespace TinyRayCaster {
class Color final {
public:
	Color(const std::byte red, const std::byte green, const std::byte blue, const std::byte alpha = std::byte {255});
	explicit Color(const unsigned int color);
	Color(const Color& other) = default;
	Color(Color&& other) noexcept = default;
	~Color() = default;

public:
	Color& operator=(const Color& other) = default;
	Color& operator=(Color&& other) noexcept = default;

public:
	[[nodiscard]] unsigned int getColor() const;
	[[nodiscard]] std::byte getRed() const;
	[[nodiscard]] std::byte getGreen() const;
	[[nodiscard]] std::byte getBlue() const;
	[[nodiscard]] std::byte getAlpha() const;

private:
	unsigned int color;
};
}