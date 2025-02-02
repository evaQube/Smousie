#include "UI/UIScreen_MainMenu.h"

UIScreen_MainMenu::UIScreen_MainMenu(sf::String menuTitle, sf::Texture* backgroundTexture, sf::Font& textFont, sf::Texture* mouseTexture)
{
	UIScreen::setScreen(menuTitle, backgroundTexture, textFont);
	m_buttonPlay.setButton("Play!", sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), m_textFont, 60, sf::Color::White, sf::Vector2f(240.0f, 100.0f), sf::Color(255, 138, 232, 255), sf::Color(87, 224, 100, 255));
	m_buttonCredits.setButton("Credits", m_buttonPlay.getPosition() + sf::Vector2f(.0f, 120.0f), m_textFont);
	m_buttonExit.setButton("Exit", m_buttonCredits.getPosition() + sf::Vector2f(.0f, 100.0f), m_textFont);

	m_tileWidth = 172.0f;
	m_tileHeight = 139.0f;
	m_spriteMouse.setTexture(*mouseTexture);
	m_spriteMouse.setOrigin(m_tileWidth / 2.0f, m_tileHeight);
	m_spriteMouse.setPosition(sf::Vector2f(SCREEN_WIDTH - 80.0f, SCREEN_HEIGHT - 12.5f));
}

void UIScreen_MainMenu::update(float deltaMilliseconds)
{
	updateMouseAnimation(deltaMilliseconds);
}

void UIScreen_MainMenu::updateOver(sf::Vector2i mousePosition)
{
	m_buttonPlay.checkOver(mousePosition);
	m_buttonCredits.checkOver(mousePosition);
	m_buttonExit.checkOver(mousePosition);
}

void UIScreen_MainMenu::updateMouseAnimation(float deltaMilliseconds)
{
	m_timeElapsedMouseAnimation += deltaMilliseconds;

	if (m_timeElapsedMouseAnimation >= 500.0f)
	{
		if (m_frame == 0) { m_frame = 1; }
		else { m_frame = 0; }
		m_timeElapsedMouseAnimation = .0f;
	}
}

void UIScreen_MainMenu::render(sf::RenderWindow& window)
{
	UIScreen::render(window);
	m_buttonPlay.render(window);
	m_buttonCredits.render(window);
	m_buttonExit.render(window);

	m_spriteMouse.setTextureRect(sf::IntRect(m_tileWidth * m_frame, 0, m_tileWidth, m_tileHeight));
	window.draw(m_spriteMouse);
}
