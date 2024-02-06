#include "STBImage.hpp"

#include <array>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <memory>
#include <numbers>
#include <random>
#include <vector>

#include "Color.hpp"
#include "Image.hpp"
#include "TextureAtlas.hpp"

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
			const TinyRayCaster::Color color {red, green, blue};
			image[y * imageWidth + x] = color.getColor();
		}
	}
}

int main(int argc, char* argv[]) {
	const std::filesystem::path imagePath {"Output/Image.ppm"};
	const auto imageWidth {1024u};
	const auto imageHeight {512u};
	const TinyRayCaster::Color backgroundColor {std::byte {255}, std::byte {255}, std::byte {255}};
	std::vector<unsigned int> pixels(imageWidth * imageHeight, backgroundColor.getColor());
	const TinyRayCaster::Color rayColor {std::byte {150}, std::byte {150}, std::byte {150}};
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
            "0   3   11100  0"\
            "5   4   0      0"\
            "5   4   1  00000"\
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
	
	std::unique_ptr<TinyRayCaster::TextureAtlas> wallTextureAtlas {};
	try {
		wallTextureAtlas = std::make_unique<TinyRayCaster::TextureAtlas>("Resources/Textures/WallTextureAtlas.png");
	} catch (const std::exception& exception) {
		std::cerr << exception.what() << std::endl;
		return -1;
	}
	
	for (std::size_t frameIndex {0}; frameIndex < 360; ++frameIndex) {
		std::stringstream stringStream {};
		stringStream << "Output/Frame" << std::setfill('0') << std::setw(5) << frameIndex << ".ppm";
		const auto framePath {stringStream.str()};
		playerViewAngle += static_cast<float>(2 * std::numbers::pi / 360);
		
		pixels = std::vector<unsigned int>(imageWidth * imageHeight, backgroundColor.getColor());
		
		for (std::size_t yIndex {0}; yIndex < mapHeight; ++yIndex) {
			for (std::size_t xIndex {0}; xIndex < mapWidth; ++xIndex) {
				const auto mapIndex {yIndex * mapWidth + xIndex};
				if (map[mapIndex] == ' ') {
					continue;
				}
				
				const auto xPosition {static_cast<unsigned int>(xIndex * rectangleWidth)};
				const auto yPosition {static_cast<unsigned int>(yIndex * rectangleHeight)};
				const auto textureId {map[mapIndex] - '0'};
				const auto textureIndex {textureId * wallTextureAtlas->getTextureSize()};
				drawRectangle(pixels,
				              imageWidth,
				              imageHeight,
				              xPosition,
				              yPosition,
				              rectangleWidth,
				              rectangleHeight,
				              wallTextureAtlas->getPixel(textureIndex, 0, 0));
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
				auto rayScreenX {static_cast<int>(rayX * rectangleWidth)};
				auto rayScreenY {static_cast<int>(rayY * rectangleHeight)};
				auto pixelIndex {static_cast<int>(rayScreenY * imageWidth + rayScreenX)};
				pixels[pixelIndex] = rayColor.getColor();
				
				const auto mapIndex {static_cast<int>(rayY) * mapWidth + static_cast<int>(rayX)};
				if (map[mapIndex] != ' ') {
					const auto columnHeight {static_cast<unsigned int>(imageHeight /
					                                                   (rayDistance *
					                                                    std::cos(rayAngle - playerViewAngle)))};
					const auto textureId {map[mapIndex] - '0'};
					
					auto hitX {rayX - std::floor(rayX + 0.5f)};
					auto hitY {rayY - std::floor(rayY + 0.5f)};
					auto textureCoordinateX {static_cast<int>(hitX * wallTextureAtlas->getTextureSize())};
					
					if (std::fabs(hitY) > std::fabs(hitX)) {
						textureCoordinateX = static_cast<int>(hitY * wallTextureAtlas->getTextureSize());
					}
					
					if (textureCoordinateX < 0) {
						textureCoordinateX += wallTextureAtlas->getTextureSize();
					}
					
					if (textureCoordinateX < 0 && textureCoordinateX < wallTextureAtlas->getTextureSize()) {
						throw std::runtime_error("TextureAtlas coordinate X is out of bounds");
					}
					
					const auto column {wallTextureAtlas->getPixelColumn(textureId, textureCoordinateX, columnHeight)};
					rayScreenX = static_cast<int>((imageWidth / 2) + index);
					
					for (std::size_t yIndex {0}; yIndex < columnHeight; ++yIndex) {
						rayScreenY = (imageHeight / 2) - (columnHeight / 2) + yIndex;
						if (rayScreenY < 0 || rayScreenY >= imageHeight) {
							continue;
						}
						
						pixelIndex = static_cast<int>(rayScreenY * imageWidth + rayScreenX);
						pixels[pixelIndex] = column[yIndex];
					}
					
					break;
				}
			}
		}
		
		TinyRayCaster::Image image {framePath, imageWidth, imageHeight};
		image.write(pixels);
	}
	
	return 0;
}