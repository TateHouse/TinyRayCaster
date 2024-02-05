#include <array>
#include <cstddef>
#include <fstream>
#include <filesystem>
#include <vector>

unsigned int packColor(const std::byte red,
                       const std::byte green,
                       const std::byte blue,
                       const std::byte alpha = std::byte {255}) {
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

void drawRectangle(std::vector<unsigned int>& image,
                   const unsigned int imageWidth,
                   const unsigned int imageHeight,
                   const unsigned int xPosition,
                   const unsigned int yPosition,
                   const unsigned int rectangleWidth,
                   const unsigned int rectangleHeight,
                   const unsigned int color) {
	if (image.size() != imageWidth * imageHeight) {
		throw std::invalid_argument("Image size does not match width and height");
	}
	
	for (std::size_t xIndex {0}; xIndex < rectangleWidth; ++xIndex) {
		for (std::size_t yIndex {0}; yIndex < rectangleHeight; ++yIndex) {
			const auto pixelX {xPosition + xIndex};
			const auto pixelY {yPosition + yIndex};
			
			if (pixelX < imageWidth && pixelY < imageHeight) {
				image[pixelY * imageWidth + pixelX] = color;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	const std::filesystem::path imagePath {"Output/Image.ppm"};
	const auto imageWidth {256u};
	const auto imageHeight {256u};
	std::vector<unsigned int> image(imageWidth * imageHeight);
	const auto mapWidth {16u};
	const auto mapHeight {16u};
	constexpr std::array<char, (mapWidth * mapHeight) + 1> map {
			"0000222222220000"\
            "1              0"\
            "1      11111   0"\
            "1     0        0"\
            "0     0  1110000"\
            "0     3        0"\
            "0   10000      0"\
            "0   0   11100  0"\
            "0   0   0      0"\
            "0   0   1  00000"\
            "0       1      0"\
            "2       1      0"\
            "0       0      0"\
            "0 0000000      0"\
            "0              0"\
            "0002222222200000"
	};
	
	auto playerX {3.456f};
	auto playerY {2.345f};
	
	for (std::size_t y {0}; y < imageHeight; ++y) {
		for (std::size_t x {0}; x < imageWidth; ++x) {
			const auto red {static_cast<std::byte>(x)};
			const auto green {static_cast<std::byte>(y)};
			const auto blue {static_cast<std::byte>(0)};
			image[y * imageWidth + x] = packColor(red, green, blue);
		}
	}
	
	const auto rectangleWidth {imageWidth / mapWidth};
	const auto rectangleHeight {imageHeight / mapHeight};
	
	for (std::size_t yIndex {0}; yIndex < mapHeight; ++yIndex) {
		for (std::size_t xIndex {0}; xIndex < mapWidth; ++xIndex) {
			const auto mapIndex {yIndex * mapWidth + xIndex};
			if (map[mapIndex] == ' ') {
				continue;
			}
			
			const auto xPosition {static_cast<unsigned int>(xIndex * rectangleWidth)};
			const auto yPosition {static_cast<unsigned int>(yIndex * rectangleHeight)};
			drawRectangle(image,
			              imageWidth,
			              imageHeight,
			              xPosition,
			              yPosition,
			              rectangleWidth,
			              rectangleHeight,
			              packColor(std::byte {0}, std::byte {255}, std::byte {255}, std::byte {255}));
		}
	}
	
	const auto playerMapX {static_cast<unsigned int>(playerX * rectangleWidth)};
	const auto playerMapY {static_cast<unsigned int>(playerY * rectangleHeight)};
	drawRectangle(image,
	              imageWidth,
	              imageHeight,
	              playerMapX,
	              playerMapY,
	              5,
	              5,
	              packColor(std::byte {255}, std::byte {255}, std::byte {255}, std::byte {255}));
	writeImage(imagePath, image, imageWidth, imageHeight);
	
	return 0;
}