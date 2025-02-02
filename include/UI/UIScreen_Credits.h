#pragma once

#include <list>
#include <UI/UIScreen.h>

class UIScreen_Credits : public UIScreen
{
	public:

		UIScreen_Credits(sf::String menuTitle, sf::Texture* backgroundTexture, sf::Font& textFont);

		void updateOver(sf::Vector2i mousePosition) override;
		void render(sf::RenderWindow& window);

		bool getBackClicked(sf::Vector2i& mousePosition) { return m_buttonBack.isClicked(mousePosition); }

	private:

		sf::Text m_text;
		std::list<sf::Text> m_textCredits;
		UIButton m_buttonBack;

		sf::Text setNewTextLine(sf::String text, int textSize = 0);
};