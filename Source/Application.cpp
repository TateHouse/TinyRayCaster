#include "Application.hpp"

#include "Color.hpp"
#include "Image.hpp"

namespace TinyRayCaster {

void Application::run() {
	constexpr auto frameCount {360u};
	for (auto frameIndex {0u}; frameIndex < frameCount; ++frameIndex) {
		update(frameIndex);
	}
}

void Application::update(const unsigned int frameIndex) {
	std::stringstream frameNameStream {};
	frameNameStream << "Output/Frame" << std::setfill('0') << std::setw(5) << frameIndex << ".ppm";
	player.rotate(static_cast<float>(2 * std::numbers::pi) / 360.0f);
	const Color rayColor {std::byte {160}, std::byte {160}, std::byte {160}};
	render(rayColor);
	Image image {frameNameStream.str(), frameBuffer.getWidth(), frameBuffer.getHeight()};
	image.write(frameBuffer.getPixels());
}

void Application::render(const Color& rayColor) {
	const TinyRayCaster::Color backgroundColor {std::byte {255}, std::byte {255}, std::byte {255}};
	frameBuffer.clear(backgroundColor);
	const auto rectangleWidth {frameBuffer.getWidth() / (map.getWidth() * 2)};
	const auto rectangleHeight {frameBuffer.getHeight() / map.getHeight()};
	
	drawMap(rectangleWidth, rectangleHeight);
	
	for (std::size_t index {0}; index < imageWidth / 2; ++index) {
		const auto rayAngle {getRayAngle(index)};
		constexpr auto maxRayDistance {20.0f};
		constexpr auto rayStepSize {0.01f};
		auto rayDistance {0.0f};
		
		while (rayDistance < maxRayDistance) {
			const auto rayX {player.getXPosition() + rayDistance * std::cos(rayAngle)};
			const auto rayY {player.getYPosition() + rayDistance * std::sin(rayAngle)};
			auto rayScreenX {static_cast<int>(rayX * rectangleWidth)};
			auto rayScreenY {static_cast<int>(rayY * rectangleHeight)};
			frameBuffer.setPixel(rayScreenX, rayScreenY, rayColor);
			
			if (!map.isCellEmpty(static_cast<unsigned int>(rayX), static_cast<unsigned int>(rayY))) {
				const auto columnHeight {static_cast<unsigned int>(imageHeight /
				                                                   (rayDistance *
				                                                    std::cos(rayAngle - player.getViewAngle())))};
				const auto textureId {map.getCell(static_cast<int>(rayX),
				                                  static_cast<int>(rayY))};
				
				auto hitX {rayX - std::floor(rayX + 0.5f)};
				auto hitY {rayY - std::floor(rayY + 0.5f)};
				const auto textureCoordinateX {getTextureCoordinateX(hitX, hitY)};
				const auto column {wallTextureAtlas.getPixelColumn(textureId, textureCoordinateX, columnHeight)};
				rayScreenX = static_cast<int>((imageWidth / 2) + index);
				
				for (std::size_t yIndex {0}; yIndex < columnHeight; ++yIndex) {
					rayScreenY = (imageHeight / 2) - (columnHeight / 2) + yIndex;
					
					if (rayScreenY < 0 || rayScreenY >= imageHeight) {
						continue;
					}
					
					const Color pixelColor {column[yIndex]};
					frameBuffer.setPixel(rayScreenX, rayScreenY, pixelColor);
				}
				
				break;
			}
			
			rayDistance += rayStepSize;
		}
	}
}

void Application::drawMap(unsigned int rectangleWidth, unsigned int rectangleHeight) {
	for (size_t yIndex {0}; yIndex < map.getHeight(); ++yIndex) {
		for (size_t xIndex {0}; xIndex < map.getWidth(); ++xIndex) {
			if (map.isCellEmpty(xIndex, yIndex)) {
				continue;
			}
			
			const auto xPosition {static_cast<unsigned int>(xIndex * rectangleWidth)};
			const auto yPosition {static_cast<unsigned int>(yIndex * rectangleHeight)};
			const auto textureId {map.getCell(xIndex, yIndex)};
			const auto textureIndex {textureId * wallTextureAtlas.getTextureSize()};
			const auto texturePixel {wallTextureAtlas.getPixel(textureIndex, 0, 0)};
			const auto color {Color {texturePixel}};
			frameBuffer.drawRectangle(xPosition,
			                          yPosition,
			                          rectangleWidth,
			                          rectangleHeight,
			                          color);
		}
	}
}

float Application::getRayAngle(size_t index) const {
	const auto startingAngle {player.getViewAngle() - player.getFieldOfView() / 2.0f};
	const auto rayIndexNormalized {static_cast<float>(index) / static_cast<float>(imageWidth / 2)};
	const auto rayAngle {startingAngle + player.getFieldOfView() * rayIndexNormalized};
	return rayAngle;
}

int Application::getTextureCoordinateX(const float hitX, const float hitY) const {
	auto textureCoordinateX {static_cast<int>(hitX * wallTextureAtlas.getTextureSize())};
	
	if (std::fabs(hitY) > std::fabs(hitX)) {
		textureCoordinateX = static_cast<int>(hitY * wallTextureAtlas.getTextureSize());
	}
	
	if (textureCoordinateX < 0) {
		textureCoordinateX += wallTextureAtlas.getTextureSize();
	}
	
	if (textureCoordinateX < 0 && textureCoordinateX < wallTextureAtlas.getTextureSize()) {
		throw std::runtime_error("TextureAtlas coordinate X is out of bounds");
	}
	
	return textureCoordinateX;
}
}