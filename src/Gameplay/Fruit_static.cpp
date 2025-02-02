#include <Gameplay/Fruit_static.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

void Fruit_static::init(int type, int row, int rowPos, bool insideBubble, sf::Font& textFont)
{
	setFruit(type, row, rowPos, insideBubble);
	setTextScore(textFont);
}

void Fruit_static::setTextScore(sf::Font& textFont)
{
	m_textScore.setFont(textFont);
	m_textScore.setCharacterSize(40);
	m_textScore.setOutlineColor(sf::Color::White);
	m_textScore.setOutlineThickness(2.0f);
}

void Fruit_static::updateTextScore(int score)
{
	m_textScore.setString(std::to_string(score));
	switch (m_type)
	{
	case 1: //Berry
		m_textScore.setFillColor(sf::Color{ 156, 87, 214, 255 });
		break;
	case 2: //Orange
		m_textScore.setFillColor(sf::Color{ 255, 147, 37, 255 });
		break;
	case 3: //Strawberry
		m_textScore.setFillColor(sf::Color{ 243, 17, 9, 255 });
		break;
	}
}

void Fruit_static::setFruit(int type, bool insideBubble)
{
	Fruit::setFruit(type, insideBubble);
}

void Fruit_static::setFruit(int type, int row, int rowPos, bool insideBubble)
{
	Fruit::setFruit(type, insideBubble);

	m_row = row;
	m_rowPos = rowPos;

	m_position = sf::Vector2f((rowPos * m_tileWidth) + 55.0f, (row * (m_tileHeight - 10.0f)) + 145.0f);
	m_sprite.setPosition(m_position);

	m_visited = false;
}

void Fruit_static::setFruit(int type, sf::Vector2f position, bool insideBubble)
{
	Fruit::setFruit(type, insideBubble);

	m_position = position;
	m_sprite.setPosition(m_position);
}

void Fruit_static::repositionOddFruit() 
{
	m_sprite.setPosition(m_sprite.getPosition()+ sf::Vector2f(m_tileWidth/2, .0f));
}

void Fruit_static::repositionRowFruit() 
{
	m_sprite.setPosition(m_sprite.getPosition() + sf::Vector2f(.0f, (m_tileHeight - 10.0f)));
	m_row++;
}


bool Fruit_static::checkCollision(sf::FloatRect other) 
{ 
	if (m_type != EMPTY && m_type != ROW_END) 
	{
		return getBounds().intersects(other);
	}
	else 
	{
		return false;
	}
}

void Fruit_static::pop(int score)
{
	m_popping = true;
	updateTextScore(score);
	m_type = EMPTY;
	m_visited = false;
}

void Fruit_static::update(float deltaMilliseconds)
{
	if (m_popping)
	{
		animatePop(deltaMilliseconds);
	}
}

void Fruit_static::animatePop(float deltaMilliseconds)
{
	m_timeElapsedPop += deltaMilliseconds;

	if (m_timeElapsedPop >= 80.0f)
	{
		m_frame++;

		if (m_frame == 5)
		{
			m_frame = 0;
			m_popping = false;
			setFruit(0, false);
		}
		m_timeElapsedPop = .0f;
	}
}

void Fruit_static::render(sf::RenderWindow& window)
{
	Fruit::render(window);

	if (m_frame > 1)
	{
		m_textScore.setPosition(m_sprite.getPosition() - sf::Vector2f(20.0f, 20.0f));
		window.draw(m_textScore);
	}
}
