#include <cstddef>
#include <fstream>
#include <filesystem>
#include <vector>

unsigned int packColor(const std::byte red,
                       const std::byte green,
                       const std::byte blue,
                       const std::byte alpha) {
	return (static_cast<unsigned int>(red) << 0) +
	       (static_cast<unsigned int>(green) << 8) +
	       (static_cast<unsigned int>(blue) << 16) +
	       (static_cast<unsigned int>(alpha) << 24);
}

void unpackColor(const unsigned int color,
                 std::byte& red,
                 std::byte& green,
                 std::byte& blue,
                 std::byte& alpha) {
	red = static_cast<std::byte>((color >> 0) & 255);
	green = static_cast<std::byte>((color >> 8) & 255);
	blue = static_cast<std::byte>((color >> 16) & 255);
	alpha = static_cast<std::byte>((color >> 24) & 255);
}

void writeImage(const std::filesystem::path& path,
                const std::vector<unsigned int>& image,
                const unsigned int imageWidth,
                const unsigned int imageHeight) {
	if (image.size() != imageWidth * imageHeight) {
		throw std::invalid_argument("Image size does not match width and height");
	}
	
	std::ofstream fileStream {path, std::ios::out | std::ios::binary};
	fileStream << "P6\n" << imageWidth << " " << imageHeight << "\n255\n";
	for (std::size_t pixelIndex {0}; pixelIndex < image.size(); ++pixelIndex) {
		std::byte red {};
		std::byte green {};
		std::byte blue {};
		std::byte alpha {};
		unpackColor(image[pixelIndex], red, green, blue, alpha);
		fileStream << static_cast<char>(red) << static_cast<char>(green) << static_cast<char>(blue);
	}
	
	fileStream.close();
}

int main(int argc, char* argv[]) {
	return 0;
}