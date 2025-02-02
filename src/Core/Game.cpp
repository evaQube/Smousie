#include <cassert>
#include <Core/Game.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

bool Game::init(GameCreateInfo& createInfo)
{
	m_createInfo = createInfo;

	assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

	m_window = new sf::RenderWindow({ SCREEN_WIDTH, SCREEN_HEIGHT }, m_createInfo.gameTitle);
	m_window->setFramerateLimit(m_createInfo.frameRateLimit);
	
	m_textFont.loadFromFile("../Data/Fonts/Eracake.ttf");
	m_world = new World();
	const bool loadOk = m_world->load(m_textFont);
	m_uiManager = new UIManager();
	m_uiManager->init(m_textFont);
	m_uiManager->showMainMenu();

	return loadOk;
}

void Game::finish()
{
	m_world->unload();
	m_uiManager->finish();
}

void Game::replay()
{
	m_world->load(m_textFont);
	m_uiManager->init(m_textFont);
}

Game::~Game()
{
	finish();

	delete m_uiManager;
	delete m_world;
	delete m_window;
}

bool Game::isRunning() const 
{ 
	return m_window->isOpen(); 
}

void Game::update(uint32_t deltaMilliseconds)
{
	for (auto event = sf::Event(); m_window->pollEvent(event);)
	{
		if (event.type == sf::Event::Closed)
		{
			m_window->close();
		}
	}

	m_mousePosition = sf::Mouse::getPosition(*m_window);
	m_mouseClicked = m_mousePressed && !sf::Mouse::isButtonPressed(sf::Mouse::Left);
	m_mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	m_mouseInsideScreen = m_mousePosition.x > 0 && m_mousePosition.x < m_window->getSize().x && 
						  m_mousePosition.y > 0 && m_mousePosition.y < m_window->getSize().y;

	m_uiManager->update(deltaMilliseconds, m_mousePosition, m_mouseInsideScreen, m_mouseClicked);
	if (!m_uiManager->isShowingSomeMenu()) { m_world->update(deltaMilliseconds, m_mousePosition, m_mouseInsideScreen, m_mousePressed); }
	
	if (m_uiManager->m_exitRequested) 
	{ 
		m_window->close();
	}
	else if (m_uiManager->m_resetGameRequested)
	{
		m_uiManager->m_resetGameRequested = false;
		finish();
		replay();
	}
	else if (m_world->m_gameOver) 
	{
		m_uiManager->showGameOver(m_world->m_totalScoreString);
	}
}

void Game::render()
{
	m_window->clear();

	if (m_uiManager->isShowingSomeMenu()) { m_uiManager->render(*m_window); }
	else { m_world->render(*m_window); }

	m_window->display();
}