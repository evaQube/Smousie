#pragma once

#include <Gameplay/Fruit.h>
#include <Gameplay/Fruit_moving.h>
#include <SFML/Graphics/Text.hpp>

class Fruit_static : public Fruit
{
	public:

		void init(int type, int row, int rowPos, bool insideBubble, sf::Font& textFont);
		void setFruit(int type, bool insideBubble);
		void setFruit(int type, int row, int rowPos, bool insideBubble);
		void setFruit(int type, sf::Vector2f position, bool insideBubble);
		void repositionOddFruit();
		void repositionRowFruit();

		int getRow() const { return m_row; }
		int getRowPos() const { return m_rowPos; }

		bool checkCollision(sf::FloatRect other);
		void pop(int score);

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		bool m_visited{ false };
		bool m_popping{ false };

	private:

		int m_row{ 99 };
		int m_rowPos{ 99 };

		void animatePop(float deltaMilliseconds);
		sf::Text m_textScore;
		void setTextScore(sf::Font& textFont);
		void updateTextScore(int score);
		float m_timeElapsedPop{ .0f };
};