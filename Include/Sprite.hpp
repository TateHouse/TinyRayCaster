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
	bool operator<(const Sprite& other) const;

public:
	[[nodiscard]] float getXPosition() const;
	[[nodiscard]] float getYPosition() const;
	[[nodiscard]] float getPlayerDistance() const;
	void setPlayerDistance(const float playerDistance);
	[[nodiscard]] unsigned int getTextureId() const;

private:
	float xPosition;
	float yPosition;
	float playerDistance;
	unsigned int textureId;
};
}