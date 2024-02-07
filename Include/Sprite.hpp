#pragma once

namespace TinyRayCaster {
class Sprite final {
public:
	Sprite(const float xPosition, const float yPosition, const unsigned int textureId);
	Sprite(const Sprite& other) = default;
	Sprite(Sprite&& other) = default;
	~Sprite() = default;

public:
	Sprite& operator=(const Sprite& other) = default;
	Sprite& operator=(Sprite&& other) = default;

public:
	[[nodiscard]] float getXPosition() const;
	[[nodiscard]] float getYPosition() const;
	[[nodiscard]] unsigned int getTextureId() const;

private:
	float xPosition;
	float yPosition;
	unsigned int textureId;
};
}