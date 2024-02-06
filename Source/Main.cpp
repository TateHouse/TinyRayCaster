#include <array>
#include <cstddef>
#include <fstream>
#include <filesystem>
#include <numbers>
#include <random>
#include <sstream>
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
			
			if (pixelX >= imageWidth || pixelY >= imageHeight) {
				continue;
			}
			
			const auto pixelIndex {pixelY * imageWidth + pixelX};
			image[pixelIndex] = color;
		}
	}
}

[[maybe_unused]] void drawGradientBackground(const unsigned int imageWidth,
                                             const unsigned int imageHeight,
                                             std::vector<unsigned int>& image) {
	for (size_t y {0}; y < imageHeight; ++y) {
		for (size_t x {0}; x < imageWidth; ++x) {
			const auto red {static_cast<std::byte>(255 * (y / static_cast<float>(imageHeight)))};
			const auto green {static_cast<std::byte>(255 * (x / static_cast<float>(imageWidth)))};
			const std::byte blue {0};
			image[y * imageWidth + x] = packColor(red, green, blue);
		}
	}
}

int main(int argc, char* argv[]) {
	const std::filesystem::path imagePath {"Output/Image.ppm"};
	const auto imageWidth {1024u};
	const auto imageHeight {512u};
	std::vector<unsigned int> image(imageWidth * imageHeight,
	                                packColor(std::byte {255}, std::byte {255}, std::byte {255}));
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
	auto playerViewAngle {1.523f};
	const auto playerFOV {static_cast<float>(std::numbers::pi) / 3.0f};
	const auto rectangleWidth {imageWidth / (mapWidth * 2)};
	const auto rectangleHeight {imageHeight / mapHeight};
	constexpr auto colorCount {16};
	std::vector<unsigned int> colors(colorCount);
	std::random_device randomDevice;
	std::mt19937 randomEngine(randomDevice());
	
	for (std::size_t colorIndex {0}; colorIndex < colors.size(); ++colorIndex) {
		const auto red {static_cast<std::byte>(randomEngine() % 255)};
		const auto green {static_cast<std::byte>(randomEngine() % 255)};
		const auto blue {static_cast<std::byte>(randomEngine() % 255)};
		colors[colorIndex] = packColor(red, green, blue);
	}
	
	for (std::size_t frameIndex {0}; frameIndex < 360; ++frameIndex) {
		std::stringstream stringStream {};
		stringStream << "Output/Frame" << std::setfill('0') << std::setw(5) << frameIndex << ".ppm";
		const auto framePath {stringStream.str()};
		playerViewAngle += static_cast<float>(2 * std::numbers::pi / 360);
		
		image = std::vector<unsigned int>(imageWidth * imageHeight,
		                                  packColor(std::byte {255}, std::byte {255}, std::byte {255}));
		
		for (std::size_t yIndex {0}; yIndex < mapHeight; ++yIndex) {
			for (std::size_t xIndex {0}; xIndex < mapWidth; ++xIndex) {
				const auto mapIndex {yIndex * mapWidth + xIndex};
				if (map[mapIndex] == ' ') {
					continue;
				}
				
				const auto xPosition {static_cast<unsigned int>(xIndex * rectangleWidth)};
				const auto yPosition {static_cast<unsigned int>(yIndex * rectangleHeight)};
				const auto colorIndex {map[mapIndex] - '0'};
				drawRectangle(image,
				              imageWidth,
				              imageHeight,
				              xPosition,
				              yPosition,
				              rectangleWidth,
				              rectangleHeight,
				              colors[colorIndex]);
			}
		}
		
		for (auto index {0}; index < imageWidth / 2; ++index) {
			const auto startingAngle {playerViewAngle - playerFOV / 2.0f};
			const auto rayIndexNormalized {static_cast<float>(index) / static_cast<float>(imageWidth / 2)};
			const auto rayAngle {startingAngle + playerFOV * rayIndexNormalized};
			constexpr auto maxRayDistance {20.0f};
			
			for (auto rayDistance {0.0f}; rayDistance < maxRayDistance; rayDistance += 0.01f) {
				const auto rayX {playerX + rayDistance * std::cos(rayAngle)};
				const auto rayY {playerY + rayDistance * std::sin(rayAngle)};
				const auto rayScreenX {static_cast<unsigned int>(rayX * rectangleWidth)};
				const auto rayScreenY {static_cast<unsigned int>(rayY * rectangleHeight)};
				const auto imageIndex {static_cast<int>(rayScreenY * imageWidth + rayScreenX)};
				image[imageIndex] = packColor(std::byte {160}, std::byte {160}, std::byte {160});
				
				const auto mapIndex {static_cast<int>(rayY) * mapWidth + static_cast<int>(rayX)};
				if (map[mapIndex] != ' ') {
					const auto columnHeight {static_cast<unsigned int>(imageHeight /
					                                                   (rayDistance *
					                                                    std::cos(rayAngle - playerViewAngle)))};
					const auto colorIndex {map[mapIndex] - '0'};
					drawRectangle(image,
					              imageWidth,
					              imageHeight,
					              (imageWidth / 2) + index,
					              (imageHeight / 2) - (columnHeight / 2),
					              1,
					              columnHeight,
					              colors[colorIndex]);
					break;
				}
			}
		}
		
		writeImage(framePath, image, imageWidth, imageHeight);
	}
	
	return 0;
}