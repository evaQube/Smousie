#pragma once

#include <Gameplay/Fruit.h>

class Fruit_moving : public Fruit
{
	public:

		void update(float deltaMilliseconds) override;

		sf::Vector2f getMoveDirection() const { return m_moveDirection; };
		void setMoveDirection(sf::Vector2f direction) { m_moveDirection = direction; }

		bool m_moveFruit{ false };

	private:

		sf::Vector2f m_moveDirection{ .0f,.0f };
		float m_speed{ 0.75f };
};