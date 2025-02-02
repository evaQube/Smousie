#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Utils/Constants.h>

constexpr int BERRY = 1;
constexpr int ORANGE = 2;
constexpr int STRAWBERRY = 3;
class Fruit : public GameObject
{
	public:

		~Fruit() override = default;

		void setFruit(int type, bool insideBubble);
		void setFruit(int type, sf::Vector2f position, bool insideBubble);

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		sf::Vector2f getSpritePosition() const { return m_sprite.getPosition(); }
		float getTileWidth() const { return m_tileWidth; }
		float getTileHeight() const { return m_tileHeight; }
		int getType() const { return m_type; }
		void show() { m_sprite.setColor(sf::Color::White); }
		void hide() { m_sprite.setColor(sf::Color::Transparent); }
		
		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

	protected:

		sf::Sprite m_sprite;
		float m_tileWidth{ .0f };
		float m_tileHeight{ .0f };
		int m_frame{ 0 };

		int m_type{ EMPTY };
};