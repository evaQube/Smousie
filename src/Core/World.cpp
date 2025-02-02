#include <Core/AssetManager.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>

bool World::load(sf::Font& textFont)
{
	constexpr float millisecondsToSeconds = 1 / 1000.f;

	loadTextures();

	sf::Texture* backgroundTexture = AssetManager::getInstance()->getTexture("../Data/Images/Background.png");
	m_spriteBackground.setTexture(*backgroundTexture);

	m_colliderScreenTop = sf::FloatRect(sf::Vector2f(.0f, .0f), sf::Vector2f(SCREEN_WIDTH, 105.0f));
	m_colliderScreenLeft = sf::FloatRect(sf::Vector2f(.0f, .0f), sf::Vector2f(10.0f, SCREEN_HEIGHT));
	m_colliderScreenRight = sf::FloatRect(sf::Vector2f(SCREEN_WIDTH - 10.0f, .0f), sf::Vector2f(10.0f, SCREEN_HEIGHT));
	m_colliderScreenDown = sf::FloatRect(sf::Vector2f(.0f, SCREEN_HEIGHT - 10.0f), sf::Vector2f(SCREEN_WIDTH, 10.0f));

	m_pointer = new Pointer();
	sf::Vector2f pointerPosition;
	pointerPosition.x = (SCREEN_WIDTH / 2.0f);
	pointerPosition.y = (SCREEN_HEIGHT - 170.0f);
	sf::Texture* pointerTexture = AssetManager::getInstance()->getTexture("../Data/Images/Pointer.png");
	const bool initOk = m_pointer->init(pointerPosition, pointerTexture);
	
	m_launchedFruit = &m_pointer->m_fruitToLaunch;

	m_gameManager = new GameManager();
	m_gameManager->init(textFont);
	m_fruitsOnScreen = &m_gameManager->m_fruitsOnScreen;

	m_mouse = new Mouse();
	sf::Vector2f mousePosition;
	mousePosition.x = (SCREEN_WIDTH - 90.0f);
	mousePosition.y = (SCREEN_HEIGHT - 12.5f);
	sf::Texture* mouseTexture = AssetManager::getInstance()->getTexture("../Data/Images/Mouse.png");
	m_mouse->init(mousePosition, mouseTexture);

	m_gameOver = false;

	return initOk;
}

void World::unload()
{
	unloadTextures();

	delete m_gameManager;
	delete m_pointer;
	delete m_mouse;
}

void World::update(uint32_t deltaMilliseconds, sf::Vector2i mousePosition, bool mouseInsideScreen, bool mouseClicked)
{
	m_pointer->updateMouse(deltaMilliseconds, mousePosition, mouseInsideScreen, mouseClicked);
	m_gameManager->update(deltaMilliseconds);
	m_mouse->update(deltaMilliseconds);

	checkCollisions();

	if (m_pointer->m_launches == 3)
	{
		m_pointer->m_launches = 0;
		m_gameManager->addFruitsRow();
	}

	m_gameOver = m_gameManager->m_gameOver;
	if (m_gameOver) { m_totalScoreString = m_gameManager->getTotalScoreString(); }
}

void World::render(sf::RenderWindow& window)
{
	window.draw(m_spriteBackground);
	m_gameManager->render(window);
	m_mouse->render(window);
	m_pointer->render(window);
}



void World::checkCollisions()
{
	if (m_launchedFruit->m_moveFruit)
	{
		checkCollisionLaunchedFruit();
	}

	if (m_mouse->getMove()) 
	{
		checkCollisionMouse();
	}

	if (m_gameManager->m_fruitsFalling.size() > 0)
	{
		checkCollisionsFallingFruits();
	}
}

void World::checkCollisionLaunchedFruit()
{
	if (m_launchedFruit->getBounds().intersects(m_colliderScreenTop))
	{
		m_gameManager->positionLaunchedFruit(m_launchedFruit->getType(), m_launchedFruit->getPosition(), 0);
		m_pointer->launchedFruitPositioned();
	}
	else if (m_launchedFruit->getBounds().intersects(m_colliderScreenLeft))
	{
		sf::Vector2f normalVector{ 1.0f,.0f };
		sf::Vector2f reflectionVector = getReflectionVector(m_launchedFruit->getMoveDirection(), normalVector);
		m_launchedFruit->setMoveDirection(reflectionVector);
	}
	else if (m_launchedFruit->getBounds().intersects(m_colliderScreenRight))
	{
		sf::Vector2f normalVector{ -1.0f,.0f };
		sf::Vector2f reflectionVector = getReflectionVector(m_launchedFruit->getMoveDirection(), normalVector);
		m_launchedFruit->setMoveDirection(reflectionVector);
	}
	else
	{
		bool loopBreak = false;
		for (int cRow = 0; cRow < ROWS; cRow++)
		{
			for (int cRowPos = 0; cRowPos < ROW_POSITIONS; cRowPos++)
			{
				if ((*m_fruitsOnScreen)[cRow][cRowPos].checkCollision(m_launchedFruit->getBounds()))
				{
					m_launchedFruit->m_moveFruit = false;
					m_gameManager->positionLaunchedFruit(m_launchedFruit->getType(), m_launchedFruit->getPosition(), cRow, cRowPos);
					m_pointer->launchedFruitPositioned();

					loopBreak = true;
					break;
				}
			}
			if (loopBreak) { break; }
		}
	}
}

sf::Vector2f World::getReflectionVector(sf::Vector2f direction, sf::Vector2f normal)
{
	float dotProduct = direction.x * normal.x + direction.y * normal.y;
	sf::Vector2f reflection = direction - 2.f * dotProduct * normal;
	return reflection;
}

void World::checkCollisionMouse()
{
	if (m_mouse->getBounds().intersects(m_colliderScreenLeft))
	{
		m_mouse->setDirection(1.0f);
		if (m_mouse->getCarryingFruit())
		{
			m_mouse->releaseFruit();
			m_gameManager->requestedFruitPicked();
			if (m_gameManager->m_fruitsFalling.size() == 0)
			{
				m_mouse->m_moveToInitPos = true;
			}
		}
	}
	else if (m_mouse->getBounds().intersects(m_colliderScreenRight))
	{
		m_mouse->setDirection(-1.0f);
	}
}

void World::checkCollisionsFallingFruits()
{
	for (Fruit_moving* fruitFalling : m_gameManager->m_fruitsFalling)
	{
		if (fruitFalling->m_moveFruit)
		{
			if (fruitFalling->getBounds().intersects(m_colliderScreenDown))
			{
				fruitFalling->m_moveFruit = false;
				if (!m_mouse->getMove())
				{
					m_mouse->move(true);
				}
			}
		}
		else if (!m_mouse->getCarryingFruit())
		{
			if (fruitFalling->getBounds().intersects(m_mouse->getBounds()))
			{
				m_mouse->carryFruit(fruitFalling->getType());
				m_mouse->setDirection(-1.0f);
				m_gameManager->m_fruitsFalling.remove(fruitFalling);
				
				break;
			}
		}
	}
}

void World::loadTextures()
{
	AssetManager::getInstance()->loadTexture("../Data/Images/Background.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Pointer.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Mouse.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Fruits/EmptySpace.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Fruits/Berry_bubble.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Fruits/Orange_bubble.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Fruits/Strawberry_bubble.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Fruits/Berry.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Fruits/Orange.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Fruits/Strawberry.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Fruits/Berry_smoothie.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Fruits/Orange_smoothie.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/Fruits/Strawberry_smoothie.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/UI/UIBackgroundMainMenu.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/UI/UIBackgroundCredits.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/UI/UIBackgroundPauseMenu.png");
	AssetManager::getInstance()->loadTexture("../Data/Images/UI/UIBackgroundGameOver.png");
}

void World::unloadTextures()
{
	AssetManager::getInstance()->clear();
}


