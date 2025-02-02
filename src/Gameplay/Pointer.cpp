#include <Gameplay/Fruit_static.h>
#include <Gameplay/Fruit_moving.h>
#include <Gameplay/Pointer.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>


bool Pointer::init(sf::Vector2f position, sf::Texture* texture)
{
	createRandom();
	setPointer(position, texture);
	createFruits();

	return true;
}





void Pointer::createRandom()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	m_RandomGenerator = mt;
	std::uniform_int_distribution<std::mt19937::result_type> uniformDistribution(1, 3);
	m_uniformDistribution = uniformDistribution;
}

int Pointer::getRandomFruitType()
{
	return m_uniformDistribution(m_RandomGenerator);
}

void Pointer::setPointer(sf::Vector2f position, sf::Texture* texture)
{
	m_tileWidth = 140.0f;
	m_tileHeight = 160.0f;
	m_sprite.setOrigin(m_tileWidth / 2.0f, m_tileHeight - 60.0f);
	m_sprite.setTexture(*texture);

	m_position = position;
	m_sprite.setPosition(m_position);

	m_clickJustPressed = false;
}

void Pointer::createFruits()
{
	m_fruitsCharged[0] = Fruit_static();
	m_fruitsCharged[0].setFruit(getRandomFruitType(), m_position, true);
	m_fruitToLaunch = Fruit_moving();
	m_fruitToLaunch.setFruit(m_fruitsCharged[0].getType(), m_position, true);
	m_fruitToLaunch.hide();
	
	m_fruitsCharged[1] = Fruit_static();
	m_fruitsCharged[1].setFruit(getRandomFruitType(), m_fruitsCharged[0].getPosition() + sf::Vector2f(120.0f, 35.0f), true);

	m_fruitsCharged[2] = Fruit_static();
	m_fruitsCharged[2].setFruit(getRandomFruitType(), m_fruitsCharged[1].getPosition() + sf::Vector2f(.0f, 80.0f), true);
}

void Pointer::updateMouse(float deltaMilliseconds, sf::Vector2i mousePosition, bool mouseInsideScreen, bool mouseClicked)
{
	if (mouseInsideScreen)
	{
		if (mouseClicked)
		//While the click is pressed, point to the mouse position
		{
			m_pointDirection = sf::Vector2f(mousePosition.x, mousePosition.y) - m_position;
			m_rotation = std::atan2(m_pointDirection.y, m_pointDirection.x) * 180 / 3.14159f;
			m_rotation += 90.0f;
			if (m_rotation > -75.0f && m_rotation < 75.0f)
			//Allow to launch fruit only when the pointer is aiming up
			{
				m_sprite.setRotation(m_rotation);
				m_clickJustPressed = true;
			}
		}
		else if (m_clickJustPressed)
		//When the click is released, launch the fruit
		{
			m_clickJustPressed = false;

			if (!m_fruitToLaunch.m_moveFruit)
			//Only if the previous launched fruit stopped
			{
				launchFruit();
			}
		}
	}

	update(deltaMilliseconds);
}

void Pointer::update(float deltaMilliseconds)
{
	m_fruitToLaunch.update(deltaMilliseconds);
}

void Pointer::render(sf::RenderWindow& window)
{
	window.draw(m_sprite);

	m_fruitToLaunch.render(window);
	m_fruitsCharged[0].render(window);
	m_fruitsCharged[1].render(window);
	m_fruitsCharged[2].render(window);
}

void Pointer::launchFruit() 
{
	m_fruitToLaunch.show();
	m_fruitsCharged[0].hide();
	sf::Vector2f m_fruitDirection = m_pointDirection / std::sqrt(m_pointDirection.x * m_pointDirection.x + m_pointDirection.y * m_pointDirection.y);
	m_fruitToLaunch.setMoveDirection(m_fruitDirection);
	m_fruitToLaunch.m_moveFruit = true;
	
}

void Pointer::launchedFruitPositioned()
{
	m_launches++;
	m_fruitToLaunch.m_moveFruit = false;
	m_fruitToLaunch.setPosition(m_position);
	m_fruitToLaunch.hide();
	m_fruitsCharged[0].show();
	chargeNextFruit();
}

void Pointer::chargeNextFruit()
{
	m_fruitsCharged[0].setFruit(m_fruitsCharged[1].getType(), true);
	m_fruitToLaunch.setFruit(m_fruitsCharged[0].getType(), true);
	m_fruitsCharged[1].setFruit(m_fruitsCharged[2].getType(), true);
	m_fruitsCharged[2].setFruit(getRandomFruitType(), true);
}