#pragma once

namespace TinyRayCaster {
class Player final {
public:
	Player(float xPosition, float yPosition, float viewDirection, float fieldOfView);
	Player(const Player& other) = default;
	Player(Player&& other) = default;
	~Player() = default;

public:
	Player& operator=(const Player& other) = default;
	Player& operator=(Player&& other) = default;

public:
	void rotate(float angle);
	[[nodiscard]] float getXPosition() const;
	[[nodiscard]] float getYPosition() const;
	[[nodiscard]] float getViewAngle() const;
	[[nodiscard]] float getFieldOfView() const;

private:
	float xPosition;
	float yPosition;
	float viewAngle;
	float fieldOfView;
};
}