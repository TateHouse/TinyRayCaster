#include "Sprite.hpp"

namespace TinyRayCaster {
Sprite::Sprite(const float xPosition, const float yPosition, const unsigned int textureId) :
		xPosition {xPosition}, yPosition {yPosition}, textureId {textureId} {
	
}

bool Sprite::operator<(const Sprite& other) const {
	return playerDistance > other.playerDistance;
}

[[nodiscard]] float Sprite::getXPosition() const {
	return xPosition;
}

[[nodiscard]] float Sprite::getYPosition() const {
	return yPosition;
}

[[nodiscard]] float Sprite::getPlayerDistance() const {
	return playerDistance;
}

void Sprite::setPlayerDistance(const float playerDistance) {
	this->playerDistance = playerDistance;
}

[[nodiscard]] unsigned int Sprite::getTextureId() const {
	return textureId;
}
}