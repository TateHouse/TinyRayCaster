#pragma once

#include<array>

namespace TinyRayCaster {
class Map final {
public:
	Map() = default;
	Map(const Map& other) = default;
	Map(Map&& other) = default;
	~Map() = default;

public:
	Map& operator=(const Map& other) = default;
	Map& operator=(Map&& other) = default;

public:
	[[nodiscard]] unsigned int getCell(const unsigned int x, const unsigned int y) const;
	[[nodiscard]] bool isEmptyCell(const unsigned int x, const unsigned int y) const;
	[[nodiscard]] unsigned int getWidth() const;
	[[nodiscard]] unsigned int getHeight() const;

private:
	unsigned int width {16};
	unsigned int height {16};
	std::array<char, 257> map {
			"0000222222220000"
			"1              0"
			"1      11111   0"
			"1     0        0"
			"0     0  1110000"
			"0     3        0"
			"0   10000      0"
			"0   3   11100  0"
			"5   4   0      0"
			"5   4   1  00000"
			"0       1      0"
			"2       1      0"
			"0       0      0"
			"0 0000000      0"
			"0              0"
			"0002222222200000"
	};
};
}