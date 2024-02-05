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
	const std::filesystem::path imagePath {"Output/Image.ppm"};
	const unsigned int imageWidth {256};
	const unsigned int imageHeight {256};
	std::vector<unsigned int> image(imageWidth * imageHeight);
	
	for (std::size_t y {0}; y < imageHeight; ++y) {
		for (std::size_t x {0}; x < imageWidth; ++x) {
			const auto red {static_cast<std::byte>(x)};
			const auto green {static_cast<std::byte>(y)};
			const auto blue {static_cast<std::byte>(0)};
			const auto alpha {static_cast<std::byte>(255)};
			image[y * imageWidth + x] = packColor(red, green, blue, alpha);
		}
	}
	
	writeImage(imagePath, image, imageWidth, imageHeight);
	
	return 0;
}