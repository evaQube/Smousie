#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <random>

class Pointer : public GameObject
{
	public:

		~Pointer() override = default;

		bool init(sf::Vector2f position, sf::Texture* texture);
		void updateMouse(float deltaMilliseconds, sf::Vector2i mousePosition, bool mouseInsideScreen, bool mouseClicked);
		void render(sf::RenderWindow& window) override;

		Fruit_moving m_fruitToLaunch;
		void launchedFruitPositioned();
		int m_launches{ 0 };

	private:

		void update(float deltaMilliseconds) override;

		//Random
		void createRandom();
		std::mt19937 m_RandomGenerator;
		std::uniform_int_distribution<std::mt19937::result_type> m_uniformDistribution;
		int getRandomFruitType();

		//Pointer
		void setPointer(sf::Vector2f position, sf::Texture* texture);
		sf::Sprite m_sprite;
		float m_tileWidth{ .0f };
		float m_tileHeight{ .0f };
		sf::Vector2f m_position{ .0f, .0f };

		float m_rotation{ .0f };
		sf::Vector2f m_pointDirection{ .0f, .0f };

		bool m_clickJustPressed{ false };

		//Fruits to launch
		void createFruits();
		void launchFruit();
		void chargeNextFruit();
		Fruit_static m_fruitsCharged[3];
};