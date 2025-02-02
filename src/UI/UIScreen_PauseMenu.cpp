#include "UI/UIScreen_PauseMenu.h"

UIScreen_PauseMenu::UIScreen_PauseMenu(sf::String menuTitle, sf::Texture* backgroundTexture, sf::Font& textFont)
{
	UIScreen::setScreen(menuTitle, backgroundTexture, textFont);
	m_buttonReturn.setButton("Return", sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), m_textFont);
	m_buttonExit.setButton("Exit game", m_buttonReturn.getPosition() + sf::Vector2f(.0f, 100.0f), m_textFont, 35);
}

void UIScreen_PauseMenu::updateOver(sf::Vector2i mousePosition)
{
	m_buttonReturn.checkOver(mousePosition);
	m_buttonExit.checkOver(mousePosition);
}

void UIScreen_PauseMenu::render(sf::RenderWindow& window)
{
	UIScreen::render(window);
	m_buttonReturn.render(window);
	m_buttonExit.render(window);
}
