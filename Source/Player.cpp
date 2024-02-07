#include "Player.hpp"

#include <stdexcept>

namespace TinyRayCaster {
Player::Player(float xPosition, float yPosition, float viewDirection, float fieldOfView) :
		xPosition {xPosition}, yPosition {yPosition}, viewAngle {viewDirection}, fieldOfView {fieldOfView} {
	if (fieldOfView <= 0.0f || fieldOfView >= 180.0f) {
		throw std::invalid_argument("Field of view must be greater than 0 and less than 180.");
	}
}

void Player::rotate(float angle) {
	viewAngle += angle;
}

[[nodiscard]] float Player::getXPosition() const {
	return xPosition;
}

[[nodiscard]] float Player::getYPosition() const {
	return yPosition;
}

[[nodiscard]] float Player::getViewAngle() const {
	return viewAngle;
}

[[nodiscard]] float Player::getFieldOfView() const {
	return fieldOfView;
}
}