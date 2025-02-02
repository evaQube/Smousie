#pragma once

#include <Gameplay/Fruit_moving.h>
#include <Gameplay/Fruit_static.h>
#include <list>
#include <random>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Utils/Constants.h>

class GameManager
{
	public:

		GameManager() = default;
		~GameManager() = default;

		bool init(sf::Font& textFont);
		void update(float deltaMilliseconds);
		void render(sf::RenderWindow& window);

		void addFruitsRow();
		Fruit_static m_fruitsOnScreen[ROWS][ROW_POSITIONS];
		std::list<Fruit_moving*> m_fruitsFalling;
		void requestedFruitPicked();

		void positionLaunchedFruit(int launchedFruitType, sf::Vector2f launchedFruitPosition, int collidedFruitRow, int collidedFruitRowPos);
		void positionLaunchedFruit(int launchedFruitType, sf::Vector2f launchedFruitPosition, int collidedFruitRow);
		
		bool m_gameOver{ false };
		sf::String getTotalScoreString() const { return m_textTotalScore.getString(); }
		

	private:

		//Random
		void createRandom();
		std::mt19937 m_RandomGenerator;
		std::uniform_int_distribution<std::mt19937::result_type> m_uniformDistribution;
		int getRandomFruitType();


		int m_fruitsScore{ 10 };
		int m_totalScore{ 0 };
		sf::Text m_textTotalScore;
		void setTotalScoreText(sf::Font& textFont);
		void updateTotalScoreText();


		std::list<Fruit_static*> m_fruitsConnected;
		int m_fruitsNeddedToPop{ 3 };

		void positionFruitSpaces(sf::Font& textFont);
		void generateFruitsOnScreen(int rowQuantity);

		Fruit_static* findNearEmptySpace(int row, int rowPos, sf::Vector2f direction);
		Fruit_static* findNearEmptySpace(int row, sf::Vector2f direction);
		void checkConnectedFruits(Fruit_static* fruitToCheck, bool compareTypes);
		void connectNearbyFruits(Fruit_static* fruitToCheck, bool compareTypes);
		void compareFruitType(int row, int rowPos, int type);
		bool isFruitValid(int row, int rowPos);
		bool isEmptySpace(Fruit_static* fruitSpace);
		void resetVisitedAndClearConnected();
		void popFruits(std::list<Fruit_static*> fruitsToPop, bool checkFallsAfter);
		void popAllFruits();
		void checkFallingFruits();


		sf::Sprite m_spriteSmoothieRequested;
		sf::Text m_textFruitsRequestedQuantity;
		int m_fruitsRequestedType{ 0 };
		int m_fruitsRequestedQuantity{ 0 };
		int m_fruitsRequestedToFall{ 0 };
		void setRequestedFruits(sf::Font& textFont);
		void setNewRequestedFruits();
		void updateRequestedFruitsQuantityText();

		sf::Text m_textSmoothieScore;
		int m_smoothieScore{ 0 };
		bool m_showSmoothieScore{ false };
		sf::Vector2f m_textSmoothieScoreInitPos{ .0f,.0f };
		float m_timeElapsedSmoothieScore{ .0f };
		void updateTextSmoothieScore();
		void animateSmoothieScore(float deltaMilliseconds);

		
		void checkGameOver();
};