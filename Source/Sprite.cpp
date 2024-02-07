#include "Sprite.hpp"

namespace TinyRayCaster {
Sprite::Sprite(const float xPosition, const float yPosition, const unsigned int textureId) :
		xPosition {xPosition}, yPosition {yPosition}, textureId {textureId} {
	
}

[[nodiscard]] float Sprite::getXPosition() const {
	return xPosition;
}

[[nodiscard]] float Sprite::getYPosition() const {
	return yPosition;
}

[[nodiscard]] unsigned int Sprite::getTextureId() const {
	return textureId;
}
}