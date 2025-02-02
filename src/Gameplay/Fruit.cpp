#include <Core/AssetManager.h>
#include <Gameplay/Fruit.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

void Fruit::setFruit(int type, bool insideBubble)
{
	m_type = type;
	sf::Texture* fruitTexture = nullptr;
	if (insideBubble) 
	{
		switch (m_type)
		{
		case EMPTY:
			fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/EmptySpace.png");
			break;
		case BERRY:
			fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Berry_bubble.png");
			break;
		case ORANGE:
			fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Orange_bubble.png");
			break;
		case STRAWBERRY:
			fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Strawberry_bubble.png");
			break;
		case ROW_END:
			fruitTexture = nullptr;
			break;
		}
	}
	else 
	{
		switch (m_type)
		{
		case EMPTY: 
			fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/EmptySpace.png");
			break;
		case BERRY:
			fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Berry.png");
			break;
		case ORANGE:
			fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Orange.png");
			break;
		case STRAWBERRY:
			fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Strawberry.png");
			break;
		}
	}
	
	if (fruitTexture != nullptr)
	{
		m_sprite.setTexture(*fruitTexture);
		m_tileWidth = 80.0f;
		m_tileHeight = 80.0f;
		m_sprite.setOrigin(m_tileWidth / 2, m_tileHeight / 2);
	}
}

void Fruit::setFruit(int type, sf::Vector2f position, bool insideBubble)
{
	setFruit(type, insideBubble);

	m_position = position;
	m_sprite.setPosition(m_position);
}


void Fruit::update(float deltaMilliseconds)
{

}

void Fruit::render(sf::RenderWindow& window)
{
	m_sprite.setTextureRect(sf::IntRect(m_tileWidth * m_frame, 0, m_tileWidth, m_tileHeight));

	window.draw(m_sprite);
}