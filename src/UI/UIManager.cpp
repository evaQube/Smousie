#include <Core/AssetManager.h>
#include <SFML/Window/Mouse.hpp>
#include <UI/UIManager.h>

void UIManager::init(sf::Font textFont)
{
	sf::Texture* mainMenuTexture = AssetManager::getInstance()->getTexture("../Data/Images/UI/UIBackgroundMainMenu.png");
	sf::Texture* mouseTexture = AssetManager::getInstance()->getTexture("../Data/Images/Mouse.png");
	m_mainMenu = new UIScreen_MainMenu("Smousie", mainMenuTexture, textFont, mouseTexture);

	sf::Texture* creditsTexture = AssetManager::getInstance()->getTexture("../Data/Images/UI/UIBackgroundCredits.png");
	m_credits = new UIScreen_Credits("Credits", creditsTexture, textFont);

	m_pauseButton = new UIButton();
	m_pauseButton->setInvisibleButton(sf::Vector2f(SCREEN_WIDTH - 100.0f, .0f));

	sf::Texture* pauseMenuTexture = AssetManager::getInstance()->getTexture("../Data/Images/UI/UIBackgroundPauseMenu.png");
	m_pauseMenu = new UIScreen_PauseMenu("Pause", pauseMenuTexture, textFont);

	sf::Texture* gameOverTexture = AssetManager::getInstance()->getTexture("../Data/Images/UI/UIBackgroundGameOver.png");
	m_gameOver = new UIScreen_GameOver("Game Over", gameOverTexture, textFont);
}

void UIManager::finish()
{
	delete m_mainMenu;
	delete m_credits;
	delete m_pauseButton;
	delete m_pauseMenu;
	delete m_gameOver;
}

void UIManager::update(float deltaMilliseconds, sf::Vector2i mousePosition, bool mouseInsideScreen, bool mouseClicked)
{
	if (mouseInsideScreen) 
	{
		if (mouseClicked)
		{
			if (m_showMainMenu)
			{
				if (m_showCredits)
				{
					if (m_credits->getBackClicked(mousePosition)) { m_showCredits = false; }
				}
				else
				{
					if (m_mainMenu->getPlayClicked(mousePosition)) { m_showMainMenu = false; }
					else if (m_mainMenu->getCreditsClicked(mousePosition)) { m_showCredits = true; }
					else if (m_mainMenu->getExitClicked(mousePosition)) { m_exitRequested = true; }
				}
			}
			else if (m_showPauseMenu)
			{
				if (m_pauseMenu->getReturnClicked(mousePosition)) { m_showPauseMenu = false; }
				else if (m_pauseMenu->getExitClicked(mousePosition)) 
				{ 
					m_resetGameRequested = true;
					m_showMainMenu = true; 
					m_showPauseMenu = false;
				}
			}
			else if (m_showGameOver)
			{
				if (m_gameOver->getPlayClicked(mousePosition)) 
				{ 
					m_resetGameRequested = true;
					m_showGameOver = false; 
				}
				else if (m_gameOver->getExitClicked(mousePosition)) 
				{ 
					m_resetGameRequested = true;
					m_showMainMenu = true; 
					m_showGameOver = false;
				}
			}
			else
			{
				if (m_pauseButton->isClicked(mousePosition)) { m_showPauseMenu = true; }
			}
		}
	}

	if (m_showMainMenu) 
	{ 
		m_mainMenu->update(deltaMilliseconds); 
		if (m_showCredits) { m_credits->updateOver(mousePosition); }
		else { m_mainMenu->updateOver(mousePosition); }
	}
	else if (m_showPauseMenu) { m_pauseMenu->updateOver(mousePosition); }
	else if (m_showGameOver) { m_gameOver->updateOver(mousePosition); }
}

void UIManager::render(sf::RenderWindow& window)
{
	if (m_showMainMenu)
	{
		if (m_showCredits) { m_credits->render(window); }
		else { m_mainMenu->render(window); }
	}
	else if (m_showPauseMenu) { m_pauseMenu->render(window); }
	else if (m_showGameOver) { m_gameOver->render(window); }
}
