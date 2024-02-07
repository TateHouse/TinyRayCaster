#pragma once

#include <memory>
#include <numbers>

#include "FrameBuffer.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Sprite.hpp"
#include "TextureAtlas.hpp"

namespace TinyRayCaster {
class Application final {
public:
	void run();

private:
	void update(const unsigned int frameIndex);
	void render(const Color& rayColor);
	void drawMap(unsigned int rectangleWidth, unsigned int rectangleHeight);
	void drawMapSprite(const Sprite& sprite);
	void drawSprite(const Sprite& sprite, const TextureAtlas& spriteTextureAtlas);
	[[nodiscard]] float getRayAngle(size_t index) const;
	[[nodiscard]] int getTextureCoordinateX(const float hitX, const float hitY) const;

private:
	constexpr static auto imageWidth {1024u};
	constexpr static auto imageHeight {512u};
	Player player {3.456f, 2.345f, 1.523f, static_cast<float>(std::numbers::pi) / 3.0f};
	Map map {};
	TextureAtlas wallTextureAtlas {"Resources/Textures/WallTextureAtlas.png"};
	TextureAtlas monsterTextureAtlas {"Resources/Textures/MonsterTextureAtlas.png"};
	FrameBuffer frameBuffer {imageWidth, imageHeight, Color {std::byte {255}, std::byte {255}, std::byte {255}}};
	std::vector<Sprite> sprites {};
};
}