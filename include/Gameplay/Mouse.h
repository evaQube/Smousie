#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>


class Mouse : public GameObject
{
	public:

		
		~Mouse() override = default;

		bool init(sf::Vector2f position, sf::Texture* texture);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		bool getMove() const { return m_move; }
		void move(bool move);
		void setDirection(float direction) { m_direction.x = direction; }
		bool m_moveToInitPos{ false };
		void carryFruit(int fruitType);
		void releaseFruit();
		bool getCarryingFruit() const { return m_carryingFruit; }

	protected:

		sf::Sprite m_sprite;
		float m_tileWidth{ .0f };
		float m_tileHeight{ .0f };
		
		bool m_move{ false };
		float m_speed{ .35f };
		sf::Vector2f m_direction{ 1.0f, .0f };
		sf::Vector2f m_initPosition{ .0f,.0f };
		sf::Sprite m_spriteFruit;
		bool m_carryingFruit{ false };

		void updateAnimation(float deltaMilliseconds);
		int m_frame{ 0 };
		int m_frames[2]{ 0,1 };
		float m_millisecondsBetweenFrames{ .0f };
		float m_timeElapsedAnimation{ .0f };
		bool m_changeFrame{ true };
};