#include "Application.hpp"

#include "Color.hpp"
#include "Image.hpp"

namespace TinyRayCaster {

void Application::run() {
	sprites.emplace_back(1.834f, 8.765f, 0);
	sprites.emplace_back(5.323f, 5.365f, 1);
	sprites.emplace_back(4.123f, 10.265f, 2);
	
	update(0);
}

void Application::update(const unsigned int frameIndex) {
	std::stringstream frameNameStream {};
	frameNameStream << "Output/Frame" << std::setfill('0') << std::setw(5) << frameIndex << ".ppm";
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
				const auto distanceToWall {rayDistance * std::cos(rayAngle - player.getViewAngle())};
				const auto columnHeight {static_cast<unsigned int>(frameBuffer.getHeight() / distanceToWall)};
				const auto textureId {map.getCell(static_cast<int>(rayX),
				                                  static_cast<int>(rayY))};
				
				const auto textureCoordinateX {getTextureCoordinateX(rayX, rayY)};
				const auto column {wallTextureAtlas.getPixelColumn(textureId, textureCoordinateX, columnHeight)};
				rayScreenX = static_cast<int>((imageWidth / 2) + index);
				
				for (std::size_t yIndex {0}; yIndex < columnHeight; ++yIndex) {
					rayScreenY = yIndex + frameBuffer.getHeight() / 2 - columnHeight / 2;
					
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
		
		for (std::size_t spriteIndex {0}; spriteIndex < sprites.size(); ++spriteIndex) {
			const auto& sprite {sprites[spriteIndex]};
			drawMapSprite(sprite);
			drawSprite(sprites[spriteIndex], monsterTextureAtlas);
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

void Application::drawMapSprite(const TinyRayCaster::Sprite& sprite) {
	const auto rectangleWidth {frameBuffer.getWidth() / (map.getWidth() * 2)};
	const auto rectangleHeight {frameBuffer.getHeight() / map.getHeight()};
	const auto spriteX {static_cast<int>(sprite.getXPosition() * rectangleWidth - 3)};
	const auto spriteY {static_cast<int>(sprite.getYPosition() * rectangleHeight - 3)};
	constexpr auto spriteWidth {6};
	constexpr auto spriteHeight {6};
	const Color spriteColor {std::byte {255}, std::byte {0}, std::byte {0}};
	frameBuffer.drawRectangle(spriteX, spriteY, spriteWidth, spriteHeight, spriteColor);
}

void Application::drawSprite(const TinyRayCaster::Sprite& sprite, const TinyRayCaster::TextureAtlas& textureAtlas) {
	auto spriteAngle {std::atan2(sprite.getYPosition() - player.getYPosition(),
	                             sprite.getXPosition() - player.getXPosition())};
	while (spriteAngle - player.getViewAngle() > std::numbers::pi) {
		spriteAngle -= 2 * std::numbers::pi;
	}
	
	while (spriteAngle - player.getViewAngle() < -std::numbers::pi) {
		spriteAngle += 2 * std::numbers::pi;
	}
	
	const auto spriteDistance {std::sqrt(std::pow(player.getXPosition() - sprite.getXPosition(), 2) +
	                                     std::pow(player.getYPosition() - sprite.getYPosition(), 2))};
	const auto spriteScreenSize {
			static_cast<unsigned int>(std::min(1000, static_cast<int>(frameBuffer.getHeight() / spriteDistance)))};
	const auto horizontalOffset {
			(spriteAngle - player.getViewAngle()) /
			player.getFieldOfView() * (frameBuffer.getWidth() / 2) +
			(frameBuffer.getWidth() / 2) / 2 - textureAtlas.getTextureSize() / 2};
	const auto verticalOffset {frameBuffer.getHeight() / 2 - spriteScreenSize / 2};
	
	for (std::size_t xIndex {0}; xIndex < spriteScreenSize; ++xIndex) {
		if (horizontalOffset + xIndex < 0 || horizontalOffset + xIndex >= frameBuffer.getWidth() / 2) {
			continue;
		}
		
		for (std::size_t yIndex {0}; yIndex < spriteScreenSize; ++yIndex) {
			if (verticalOffset + yIndex < 0 || verticalOffset + yIndex >= frameBuffer.getHeight()) {
				continue;
			}
			
			const auto screenX {static_cast<unsigned int>(frameBuffer.getWidth() / 2 + horizontalOffset + xIndex)};
			const auto screenY {static_cast<unsigned int>(verticalOffset + yIndex)};
			const Color pixelColor {std::byte {0}, std::byte {0}, std::byte {0}};
			frameBuffer.setPixel(screenX, screenY, pixelColor);
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
	const auto x {hitX - std::floor(hitX + 0.5f)};
	const auto y {hitY - std::floor(hitY + 0.5f)};
	auto textureCoordinateX {static_cast<int>(x * wallTextureAtlas.getTextureSize())};
	
	if (std::fabs(y) > std::fabs(x)) {
		textureCoordinateX = y * wallTextureAtlas.getTextureSize();
	}
	
	if (textureCoordinateX < 0) {
		textureCoordinateX += wallTextureAtlas.getTextureSize();
	}
	
	if (textureCoordinateX < 0 || textureCoordinateX >= wallTextureAtlas.getTextureSize()) {
		throw std::out_of_range {"TextureAtlas coordinates out of range."};
	}
	
	return textureCoordinateX;
}
}