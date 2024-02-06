#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

namespace TinyRayCaster {
class Image final {
public:
	Image(const std::filesystem::path& filePath, unsigned int width, unsigned int height);
	Image(const Image& other) = delete;
	Image(Image&& other) = delete;
	~Image() = default;

public:
	Image& operator=(const Image& other) = delete;
	Image& operator=(Image&& other) = delete;

public:
	void write(const std::vector<unsigned int>& pixels);

private:
	std::ofstream fileStream {};
	unsigned int width;
	unsigned int height;
};
}