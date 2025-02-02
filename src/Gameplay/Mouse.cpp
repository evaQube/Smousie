#include <cmath>
#include <Core/AssetManager.h>
#include <Gameplay/Mouse.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

bool Mouse::init(sf::Vector2f position, sf::Texture* texture)
{
	m_tileWidth = 172.0f;
	m_tileHeight = 139.0f;
	m_sprite.setTexture(*texture);
	m_sprite.setOrigin(m_tileWidth / 2.0f, m_tileHeight);
	m_millisecondsBetweenFrames = 500.0f;

	m_position = position;
	m_initPosition = m_position;
	m_sprite.setPosition(m_position);

	m_spriteFruit.setOrigin(m_sprite.getOrigin() + sf::Vector2f(40.0f,-70.0f));

	return true;
}

void Mouse::update(float deltaMilliseconds) 
{
	if (m_move) 
	{
		if (m_direction.x > 0) { m_sprite.setScale(-1.0f, 1.0f);  m_spriteFruit.setScale(-1.0f, 1.0f); }
		else { m_sprite.setScale(1.0f, 1.0f); m_spriteFruit.setScale(1.0f, 1.0f); }
		m_position += (m_direction * m_speed * deltaMilliseconds);
	}
	if (m_moveToInitPos)
	{
		if (abs(m_position.x - m_initPosition.x) <= 10.0f)
		{
			m_moveToInitPos = false;
			move(false);
		}
	}

	m_sprite.setPosition(m_position);
	m_spriteFruit.setPosition(m_position);

	updateAnimation(deltaMilliseconds);
}

void Mouse::updateAnimation(float deltaMilliseconds)
{
	m_timeElapsedAnimation += deltaMilliseconds;

	if (m_timeElapsedAnimation >= m_millisecondsBetweenFrames)
	{
		if (m_changeFrame)
		{
			m_frame = m_frames[0];
			m_changeFrame = false;
		}
		else
		{
			m_frame = m_frames[1];
			m_changeFrame = true;
		}

		m_timeElapsedAnimation = .0f;
	}
}

void Mouse::move(bool move) 
{
	m_move = move;

	if (m_move)
	{
		m_millisecondsBetweenFrames = 250.0f;
		m_frames[0] = 2;
		m_frames[1] = 3;
	}
	else
	{
		m_millisecondsBetweenFrames = 500.0f;
		m_frames[0] = 0;
		m_frames[1] = 1;
		m_sprite.setScale(1.0f, 1.0f);
	}
	m_changeFrame = true;
	m_frame = m_frames[0];
}

void Mouse::carryFruit(int fruitType)
{
	m_carryingFruit = true;

	sf::Texture* fruitTexture = nullptr;
	switch (fruitType)
	{
	case 1: //Berry
		fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Berry.png");
		break;
	case 2: //Orange
		fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Orange.png");
		break;
	case 3: //Strawberry
		fruitTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Strawberry.png");
		break;
	}
	if (fruitTexture != nullptr) { m_spriteFruit.setTexture(*fruitTexture); }
}

void Mouse::releaseFruit()
{
	m_carryingFruit = false;
}

void Mouse::render(sf::RenderWindow& window)
{
	m_sprite.setTextureRect(sf::IntRect(m_tileWidth * m_frame, 0, m_tileWidth, m_tileHeight));

	window.draw(m_sprite);

	if (m_carryingFruit) 
	{
		window.draw(m_spriteFruit);
	}
}

