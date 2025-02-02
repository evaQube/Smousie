#pragma once

#include <cstdint>
#include <Gameplay/GameManager.h>
#include <Gameplay/Mouse.h>
#include <Gameplay/Pointer.h>
#include <SFML/Graphics/Text.hpp>
#include <Utils/Constants.h>

class Enemy;

namespace sf
{
	class RenderWindow;
}

class ObjectLayer;

class World
{
	public:

		~World() = default;

		bool load(sf::Font& textFont);
		void unload();

		void update(uint32_t deltaMilliseconds, sf::Vector2i mousePosition, bool mouseInsideScreen, bool mouseClicked);
		void render(sf::RenderWindow& window);

		bool m_gameOver{ false };
		sf::String m_totalScoreString{ "" };

	private:

		GameManager* m_gameManager{ nullptr };
		Fruit_static(*m_fruitsOnScreen)[ROWS][ROW_POSITIONS]{ nullptr };
		Pointer* m_pointer{ nullptr };
		Fruit_moving* m_launchedFruit{ nullptr };
		Mouse* m_mouse{ nullptr };

		sf::Sprite m_spriteBackground;

		sf::FloatRect m_colliderScreenTop{ sf::Vector2f(.0f,.0f), sf::Vector2f(.0f,.0f) };
		sf::FloatRect m_colliderScreenLeft{ sf::Vector2f(.0f,.0f), sf::Vector2f(.0f,.0f) };
		sf::FloatRect m_colliderScreenRight{ sf::Vector2f(.0f,.0f), sf::Vector2f(.0f,.0f) };
		sf::FloatRect m_colliderScreenDown{ sf::Vector2f(.0f,.0f), sf::Vector2f(.0f,.0f) };
		sf::Vector2f getReflectionVector(sf::Vector2f direction, sf::Vector2f normal);

		void checkCollisions();
		void checkCollisionLaunchedFruit();
		void checkCollisionMouse();
		void checkCollisionsFallingFruits();

		void loadTextures();
		void unloadTextures();
};