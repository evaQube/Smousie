#include <Core/AssetManager.h>
#include <iomanip>
#include <Gameplay/GameManager.h>
#include <sstream>
#include <string>

bool GameManager::init(sf::Font& textFont)
{
	createRandom();

	positionFruitSpaces(textFont);
	generateFruitsOnScreen(4);

	setTotalScoreText(textFont);
	setRequestedFruits(textFont);

	m_gameOver = false;

	return true;
}

void GameManager::createRandom()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	m_RandomGenerator = mt;
	std::uniform_int_distribution<std::mt19937::result_type> uniformDistribution(1, 3);
	m_uniformDistribution = uniformDistribution;
}

void GameManager::setTotalScoreText(sf::Font& textFont)
{
	m_textTotalScore.setFont(textFont);
	m_textTotalScore.setCharacterSize(80);
	m_textTotalScore.setPosition(75, 5);
	m_textTotalScore.setFillColor(sf::Color{50, 50, 50, 255});

	updateTotalScoreText();
}

void GameManager::updateTotalScoreText()
{
	std::ostringstream ss;
	ss << std::setw(9) << std::setfill('0') << m_totalScore;
	m_textTotalScore.setString(ss.str());
}

int GameManager::getRandomFruitType()
{
	return m_uniformDistribution(m_RandomGenerator);
}

void GameManager::update(float deltaMilliseconds)
{
	for (int cRow = 0; cRow < ROWS; cRow++)
	{
		for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++)
		{
			if (!isEmptySpace(&m_fruitsOnScreen[cRow][cRowPos]) || m_fruitsOnScreen[cRow][cRowPos].m_popping)
			{
				m_fruitsOnScreen[cRow][cRowPos].update(deltaMilliseconds);
			}
		}
	}

	if (m_fruitsFalling.size() > 0) 
	{
		for (Fruit_moving* fruitFalling : m_fruitsFalling) 
		{
			fruitFalling->update(deltaMilliseconds);
		}
	}

	if (m_showSmoothieScore)
	{
		animateSmoothieScore(deltaMilliseconds);
	}
}

void GameManager::animateSmoothieScore(float deltaMilliseconds)
{
	m_timeElapsedSmoothieScore += deltaMilliseconds;
	m_textSmoothieScore.setPosition(m_textSmoothieScore.getPosition() - sf::Vector2f(.0f, .5f));

	if (m_timeElapsedSmoothieScore >= 1000.0f)
	{
		m_showSmoothieScore = false;
		m_textSmoothieScore.setPosition(m_textSmoothieScoreInitPos);
		m_timeElapsedSmoothieScore = .0f;
	}
}

void GameManager::positionLaunchedFruit(int launchedFruitType, sf::Vector2f launchedFruitPosition, int collidedFruitRow, int collidedFruitRowPos)
{
	Fruit_static* fruitToSet = findNearEmptySpace(collidedFruitRow, collidedFruitRowPos, launchedFruitPosition);
	if (fruitToSet != nullptr)
	{
		fruitToSet->setFruit(launchedFruitType, true);
		checkConnectedFruits(fruitToSet, true);
	}
}

void GameManager::positionLaunchedFruit(int launchedFruitType, sf::Vector2f launchedFruitPosition, int collidedFruitRow)
{
	Fruit_static* fruitToSet = findNearEmptySpace(collidedFruitRow, launchedFruitPosition);
	if (fruitToSet != nullptr)
	{
		fruitToSet->setFruit(launchedFruitType, true);
		checkConnectedFruits(fruitToSet, true);
	}
}

void GameManager::requestedFruitPicked()
{
	m_fruitsRequestedQuantity--;
	updateRequestedFruitsQuantityText();
	if (m_fruitsRequestedQuantity == 0)
	{
		updateTextSmoothieScore();
		m_showSmoothieScore = true;
		m_totalScore += m_smoothieScore;
		updateTotalScoreText();
		
		setNewRequestedFruits();
	}
}

void GameManager::render(sf::RenderWindow& window)
{
	for (int cRow = 0; cRow < ROWS; cRow++)
	{
		for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++)
		{
			if(!isEmptySpace(&m_fruitsOnScreen[cRow][cRowPos]) || m_fruitsOnScreen[cRow][cRowPos].m_popping)
				{ m_fruitsOnScreen[cRow][cRowPos].render(window); }
		}
	}

	if (m_fruitsFalling.size() > 0)
	{
		for (Fruit_moving* fruitFalling : m_fruitsFalling)
		{
			fruitFalling->render(window);
		}
	}

	window.draw(m_textTotalScore);
	window.draw(m_spriteSmoothieRequested);
	window.draw(m_textFruitsRequestedQuantity);
	if(m_showSmoothieScore){ window.draw(m_textSmoothieScore); }
}

void GameManager::positionFruitSpaces(sf::Font& textFont)
{
	for (int cRow = 0; cRow < ROWS; cRow++)
	{
		if (cRow > 0)
		{
			if ((m_fruitsOnScreen[cRow - 1][ROW_POSITIONS - 1].getType() != ROW_END))
			{
				for (int cRowPos = 0; cRowPos < ROW_POSITIONS - 1; cRowPos++)
				{
					m_fruitsOnScreen[cRow][cRowPos].init(EMPTY, cRow, cRowPos, true, textFont);
					m_fruitsOnScreen[cRow][cRowPos].repositionOddFruit();
				}
				m_fruitsOnScreen[cRow][ROW_POSITIONS - 1].init(ROW_END, 0, ROW_POSITIONS - 1, true, textFont);
				m_fruitsOnScreen[cRow][ROW_POSITIONS - 1].repositionOddFruit();
				continue;
			}
		}
		for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++)
		{
			m_fruitsOnScreen[cRow][cRowPos].init(EMPTY, cRow, cRowPos, true, textFont);
		}
	}
}

void GameManager::generateFruitsOnScreen(int rowQuantity)
{
	for (int cRow = 0; cRow < rowQuantity; cRow++) //Generate X rows of fruits
	{
		if (cRow > 0)
		{
			if ((m_fruitsOnScreen[cRow - 1][ROW_POSITIONS - 1].getType() != ROW_END))
			{
				for (int cRowPos = 0; cRowPos < ROW_POSITIONS - 1; cRowPos++) //Smaller row
				{
					m_fruitsOnScreen[cRow][cRowPos].setFruit(getRandomFruitType(), true);
				}
				continue;
			}
		}
		for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++) //Bigger row
		{
			m_fruitsOnScreen[cRow][cRowPos].setFruit(getRandomFruitType(), true);
		}
	}
}

void GameManager::addFruitsRow()
{
	for (int cRow = ROWS - 1; cRow > 0; cRow--)
	{
		for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++) 
		{
			m_fruitsOnScreen[cRow][cRowPos] = m_fruitsOnScreen[cRow - 1][cRowPos];
			m_fruitsOnScreen[cRow][cRowPos].repositionRowFruit();
		}
	}
	if ((m_fruitsOnScreen[1][ROW_POSITIONS - 1].getType() != ROW_END))
	{
		for (int cRowPos = 0; cRowPos < ROW_POSITIONS - 1; cRowPos++)
		{
			m_fruitsOnScreen[0][cRowPos].setFruit(getRandomFruitType(), 0, cRowPos, true);
			m_fruitsOnScreen[0][cRowPos].repositionOddFruit();
		}
		m_fruitsOnScreen[0][ROW_POSITIONS - 1].setFruit(ROW_END, 0, ROW_POSITIONS - 1, true);
		m_fruitsOnScreen[0][ROW_POSITIONS - 1].repositionOddFruit();
	}
	else 
	{
		for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++)
		{
			m_fruitsOnScreen[0][cRowPos].setFruit(getRandomFruitType(), 0, cRowPos, true);
		}
	}
	
	checkGameOver();
}


Fruit_static* GameManager::findNearEmptySpace(int row, int rowPos, sf::Vector2f position)
{
	Fruit_static* fruitSpace = nullptr;

	int rowsAndRowPos[8] =
	{ 
		row + 1, rowPos, //One of the fruits below
		row + 1, 0,		 //The other fruit below
		row, rowPos - 1, //Fruit left
		row, rowPos + 1  //Fruit right
	};
	if (m_fruitsOnScreen[row][ROW_POSITIONS - 1].getType() == ROW_END) //Are we in a shorter row?
	{
		rowsAndRowPos[3] = rowPos + 1;
	}
	else
	{
		rowsAndRowPos[3] = rowPos - 1;
	}


	for (int i = 0; i < ROW_POSITIONS; i += 2)
	{
		int rowToCheck = rowsAndRowPos[i];
		int rowPosToCheck = rowsAndRowPos[i + 1];
		if (m_fruitsOnScreen[rowToCheck][rowPosToCheck].getType() == EMPTY)
		{
			if (m_fruitsOnScreen[rowToCheck][rowPosToCheck].getBounds().contains(position))
			{
				fruitSpace = &m_fruitsOnScreen[rowToCheck][rowPosToCheck];
				break;
			}
		}
	}
	if (fruitSpace == nullptr) 
	//SPECIAL CASE: When the launched fruit collisioned with a fruit near the wall
	{
		if (rowPos == 0) 
		{
			fruitSpace = &m_fruitsOnScreen[row + 1][rowPos];
		}
		else 
		{
			fruitSpace = &m_fruitsOnScreen[row + 1][rowPos - 1];
		}
	}

	return fruitSpace;
}

Fruit_static* GameManager::findNearEmptySpace(int row, sf::Vector2f position)
{
	Fruit_static* fruitSpace = nullptr;
	for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++)
	{
		if (isEmptySpace(&m_fruitsOnScreen[row][cRowPos]))
		{
			if (m_fruitsOnScreen[row][cRowPos].getBounds().contains(position))
			{
				fruitSpace = &m_fruitsOnScreen[row][cRowPos];
				break;
			}
		}
	}

	return fruitSpace;
}

void GameManager::checkConnectedFruits(Fruit_static* fruitToCheck, bool compareTypes)
{
	m_fruitsConnected.push_front(fruitToCheck);
	fruitToCheck->m_visited = true;

	for (Fruit_static* fruit : m_fruitsConnected) 
	{
		connectNearbyFruits(fruit, compareTypes);
	}

	if (compareTypes) 
	{
		if (m_fruitsConnected.size() >= m_fruitsNeddedToPop) { popFruits(m_fruitsConnected, true); }
		else { resetVisitedAndClearConnected(); }
	}

	checkGameOver();
}

void GameManager::connectNearbyFruits(Fruit_static* fruitToCheck, bool compareTypes)
{
	int row = fruitToCheck->getRow();
	int rowPos = fruitToCheck->getRowPos();
	int fruitToCheckType = fruitToCheck->getType();

	int rowsAndRowPos[12] = 
	{   row, rowPos - 1, //Fruit left
		row, rowPos + 1, //Fruit right
		row + 1, rowPos, //One of the fruits below
		row - 1, rowPos, //One of the fruits above
		row + 1, 0,		 //The other fruit below
		row - 1, 0		 //The other fruit above
	};
	if (m_fruitsOnScreen[row][ROW_POSITIONS - 1].getType() == ROW_END) //Are we in a shorter row?
	{ 
		rowsAndRowPos[9] = rowPos + 1;
		rowsAndRowPos[11] = rowPos + 1; 
	}
	else 
	{
		rowsAndRowPos[9] = rowPos - 1;
		rowsAndRowPos[11] = rowPos - 1;
	}


	for (int i = 0; i < 12; i += 2)
	{
		int rowToCheck = rowsAndRowPos[i];
		int rowPosToCheck = rowsAndRowPos[i + 1];
		if (compareTypes)
		{
			compareFruitType(rowToCheck, rowPosToCheck, fruitToCheckType);
		}
		else 
		{
			if (isFruitValid(rowToCheck, rowPosToCheck))
			{
				m_fruitsConnected.push_back(&m_fruitsOnScreen[rowToCheck][rowPosToCheck]); 
			}
		}
	}
}

void GameManager::compareFruitType(int row, int rowPos, int type)
{
	if (isFruitValid(row, rowPos) && m_fruitsOnScreen[row][rowPos].getType() == type) //Is there a valid fruit and is it the same type?
	{
		m_fruitsConnected.push_back(&m_fruitsOnScreen[row][rowPos]);
	}
}

bool GameManager::isFruitValid(int row, int rowPos)
{
	bool fruitValid = false;
	if (rowPos >= 0 && rowPos <= (ROW_POSITIONS - 1) &&
		row >= 0 && row <= (ROWS - 1)) //Are we checking inside the limits of the array?
	{
		Fruit_static* fruitOnScreen = &m_fruitsOnScreen[row][rowPos];
		if (!isEmptySpace(fruitOnScreen) && //Is there a fruit?
			!fruitOnScreen->m_visited) //Was it visited before?
		{
			fruitOnScreen->m_visited = true;
			fruitValid = true;
		}
	}

	return fruitValid;
}

bool GameManager::isEmptySpace(Fruit_static* fruitSpace) 
{
	return (fruitSpace->getType() == EMPTY || fruitSpace->getType() == ROW_END);
}

void GameManager::resetVisitedAndClearConnected()
{
	for (int cRow = 0; cRow < ROWS; cRow++)
	{
		for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++)
		{
			if (!isEmptySpace(&m_fruitsOnScreen[cRow][cRowPos]))
				{ m_fruitsOnScreen[cRow][cRowPos].m_visited = false; }
		}
	}
	m_fruitsConnected.clear();
}

void GameManager::popFruits(std::list<Fruit_static*> fruitsToPop, bool checkFallsAfter)
{
	for(Fruit_static* fruit : fruitsToPop)
	{
		if (m_fruitsRequestedToFall > 0 && m_fruitsRequestedType == fruit->getType())
		{
			Fruit_moving* fruitFalling = new Fruit_moving;
			fruitFalling->setFruit(fruit->getType(), fruit->getSpritePosition(), false);
			fruitFalling->setMoveDirection(sf::Vector2f(.0f, 1.0f));
			fruitFalling->m_moveFruit = true;
			m_fruitsFalling.push_back(fruitFalling);
			m_fruitsRequestedToFall--;
		}
		if (checkFallsAfter) 
		{
			fruit->pop(m_fruitsScore);
			m_totalScore += m_fruitsScore;
		}
		else 
		{
			fruit->pop(m_fruitsScore * 2);
			m_totalScore += m_fruitsScore * 2;
		}
		
	}
	updateTotalScoreText();

	resetVisitedAndClearConnected();

	if (checkFallsAfter) 
	{
		checkFallingFruits();
	}
}

void GameManager::popAllFruits()
{
	for (int cRow = 0; cRow < ROWS; cRow++)
	{
		for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++) 
		{
			if (!isEmptySpace(&m_fruitsOnScreen[cRow][cRowPos]))
			{ 
				if (m_fruitsRequestedQuantity > 0 && m_fruitsRequestedType == m_fruitsOnScreen[cRow][cRowPos].getType())
				{
					m_fruitsRequestedQuantity--;
					updateRequestedFruitsQuantityText();
				}

				m_fruitsOnScreen[cRow][cRowPos].pop(m_fruitsScore * 5);
				m_totalScore += m_fruitsScore * 5;
			}
		}
	}

	if (m_fruitsRequestedQuantity == 0)
	{
		m_totalScore += m_smoothieScore;
		setNewRequestedFruits();
	}
	updateTotalScoreText();
}

void GameManager::checkFallingFruits()
{
	bool allFruitsDisconnected = true;
	for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++)
	//Loop to see if there is any fruit attached to the ceiling
	{
		if (!isEmptySpace(&m_fruitsOnScreen[0][cRowPos])) 
		{
			allFruitsDisconnected = false;
			checkConnectedFruits(&m_fruitsOnScreen[0][cRowPos], false); //Save all fruits connected to the ceiling on an array
		}
	}

	if (allFruitsDisconnected) { popAllFruits(); } //If there isn't any fruit attached to the ceiling, pop all
	else 
	{
		std::list<Fruit_static*> fruitsDisconnected;
		for (int cRow = 0; cRow < ROWS; cRow++)
		{
			for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++)
			//While searching for the connected fruits, we set visited to true.
			//Fruits that aren't visited, are disconnected from the ceiling
			{
				if (!isEmptySpace(&m_fruitsOnScreen[cRow][cRowPos]) && !m_fruitsOnScreen[cRow][cRowPos].m_visited)
					{ fruitsDisconnected.push_back(&m_fruitsOnScreen[cRow][cRowPos]); }
			}
		}

		if (fruitsDisconnected.size() > 0) { popFruits(fruitsDisconnected, false); } //Pop disconnected fruits
		else { resetVisitedAndClearConnected(); }
	}
}

void GameManager::setRequestedFruits(sf::Font& textFont)
{
	setNewRequestedFruits();

	m_spriteSmoothieRequested.setPosition(15.0f,920.0f);

	m_textFruitsRequestedQuantity.setFont(textFont);
	m_textFruitsRequestedQuantity.setCharacterSize(40);
	m_textFruitsRequestedQuantity.setPosition(m_spriteSmoothieRequested.getPosition() + 
		sf::Vector2f(m_spriteSmoothieRequested.getTextureRect().width + 5.0f, 
					(m_spriteSmoothieRequested.getTextureRect().height - 45.0f)));
	m_textFruitsRequestedQuantity.setOutlineColor(sf::Color::White);
	m_textFruitsRequestedQuantity.setOutlineThickness(2.0f);


	m_textSmoothieScore.setFont(textFont);
	m_textSmoothieScore.setCharacterSize(40);
	m_textSmoothieScore.setPosition(m_spriteSmoothieRequested.getPosition() +
		sf::Vector2f(m_spriteSmoothieRequested.getTextureRect().width/2, (- 35.0f)));
	m_textSmoothieScoreInitPos = m_textSmoothieScore.getPosition();
	m_textSmoothieScore.setOutlineColor(sf::Color::White);
	m_textSmoothieScore.setOutlineThickness(2.0f);
}

void GameManager::setNewRequestedFruits()
{
	std::uniform_int_distribution<std::mt19937::result_type> uniformDistribution(3, 6);
	m_fruitsRequestedQuantity = uniformDistribution(m_RandomGenerator);
	m_fruitsRequestedToFall = m_fruitsRequestedQuantity;
	m_smoothieScore = m_fruitsRequestedQuantity * 100;
	m_fruitsRequestedType = getRandomFruitType();

	updateRequestedFruitsQuantityText();

	sf::Texture* smoothieTexture = nullptr;
	switch (m_fruitsRequestedType)
	{
	case BERRY:
		smoothieTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Berry_smoothie.png");
		m_textFruitsRequestedQuantity.setFillColor(sf::Color{ 156, 87, 214, 255 });
		break;
	case ORANGE:
		smoothieTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Orange_smoothie.png");
		m_textFruitsRequestedQuantity.setFillColor(sf::Color{ 255, 147, 37, 255 });
		break;
	case STRAWBERRY:
		smoothieTexture = AssetManager::getInstance()->getTexture("../Data/Images/Fruits/Strawberry_smoothie.png");
		m_textFruitsRequestedQuantity.setFillColor(sf::Color{ 243, 17, 9, 255 });
		break;
	}
	if (smoothieTexture != nullptr) { m_spriteSmoothieRequested.setTexture(*smoothieTexture); }
}

void GameManager::updateRequestedFruitsQuantityText()
{
	std::ostringstream ss;
	ss << 'x' << m_fruitsRequestedQuantity;
	m_textFruitsRequestedQuantity.setString(ss.str());
}

void GameManager::updateTextSmoothieScore()
{
	m_textSmoothieScore.setFillColor(m_textFruitsRequestedQuantity.getColor());

	std::ostringstream ss;
	ss << '+' << m_smoothieScore;
	m_textSmoothieScore.setString(ss.str());
}


void GameManager::checkGameOver()
{
	for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++) 
	{
		if (!isEmptySpace(&m_fruitsOnScreen[ROWS - 1][cRowPos])) 
		//If there is a fruit in the last row, game over
		{
			m_gameOver = true;
			break;
		}
	}
}


