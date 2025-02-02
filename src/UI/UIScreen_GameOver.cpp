#include "UI/UIScreen_GameOver.h"

UIScreen_GameOver::UIScreen_GameOver(sf::String menuTitle, sf::Texture* backgroundTexture, sf::Font& textFont)
{
	UIScreen::setScreen(menuTitle, backgroundTexture, textFont);

	m_textScore.setFont(m_textFont);
	m_textScore.setString("000000000");
	m_textScore.setCharacterSize(60);
	m_textScore.setFillColor(sf::Color(50, 50, 50, 255));
	m_textScore.setOrigin(sf::Vector2f(m_textScore.getGlobalBounds().width / 2, 0));
	m_textScore.setPosition(UIScreen::getTitlePosition() + sf::Vector2f(.0f, 90.0f));

	m_buttonPlay.setButton("Try again!", sf::Vector2f(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 40.0f), m_textFont, 50, sf::Color::White, sf::Vector2f(300.0f, 100.0f), sf::Color(255, 138, 232, 255), sf::Color(87, 224, 100, 255));
	m_buttonExit.setButton("Exit game", m_buttonPlay.getPosition() + sf::Vector2f(.0f, 120.0f), m_textFont, 35);
}

void UIScreen_GameOver::updateOver(sf::Vector2i mousePosition)
{
	m_buttonPlay.checkOver(mousePosition);
	m_buttonExit.checkOver(mousePosition);
}

void UIScreen_GameOver::render(sf::RenderWindow& window)
{
	UIScreen::render(window);
	
	m_buttonPlay.render(window);
	m_buttonExit.render(window);
	window.draw(m_textScore);
}
