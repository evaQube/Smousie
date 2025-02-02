#include "UI/UIScreen_Credits.h"

UIScreen_Credits::UIScreen_Credits(sf::String menuTitle, sf::Texture* backgroundTexture, sf::Font& textFont)
{
	UIScreen::setScreen(menuTitle, backgroundTexture, textFont);
	
	m_text.setFont(m_textFont);
	m_text.setCharacterSize(60);
	m_text.setFillColor(sf::Color(50, 50, 50, 255));
	m_text.setPosition(UIScreen::getTitlePosition() + sf::Vector2f(.0f, 140.0f));

	m_textCredits.push_back(setNewTextLine("Game made by"));
	m_textCredits.push_back(setNewTextLine("Eva Qube"));
	m_textCredits.push_back(setNewTextLine(" "));
	m_textCredits.push_back(setNewTextLine("Special thanks to", 50));
	m_textCredits.push_back(setNewTextLine("The Core School", 50));

	m_buttonBack.setButton("Back", m_text.getPosition() + sf::Vector2f(.0f, m_text.getGlobalBounds().height + 300.0f), m_textFont);
}

void UIScreen_Credits::updateOver(sf::Vector2i mousePosition)
{
	m_buttonBack.checkOver(mousePosition);
}

void UIScreen_Credits::render(sf::RenderWindow& window)
{
	UIScreen::render(window);

	for (sf::Text text : m_textCredits) 
	{
		window.draw(text);
	}
	m_buttonBack.render(window);
}

sf::Text UIScreen_Credits::setNewTextLine(sf::String text, int textSize)
{
	sf::Text textLine;
	textLine.setFont(m_textFont);
	textLine.setString(text);
	if (textSize == 0) { textLine.setCharacterSize(m_text.getCharacterSize()); }
	else { textLine.setCharacterSize(textSize); }
	textLine.setFillColor(m_text.getColor());
	textLine.setOrigin(sf::Vector2f(textLine.getGlobalBounds().width / 2, 0));
	textLine.setPosition(sf::Vector2f(m_text.getPosition().x, m_text.getPosition().y + (textLine.getGlobalBounds().height * m_textCredits.size())));
	
	return textLine;
}
