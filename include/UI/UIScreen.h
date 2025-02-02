#pragma once

#include <UI/UIButton.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <Utils/Constants.h>

class UIScreen
{
	public: 

		UIScreen() = default;
		void setScreen(sf::String menuTitle, sf::Texture* backgroundTexture, sf::Font& textFont)
		{
			m_spriteBackground.setTexture(*backgroundTexture);

			m_textFont = textFont;

			m_menuTitle.setFont(m_textFont);
			m_menuTitle.setString(menuTitle);
			m_menuTitle.setCharacterSize(100);
			m_menuTitle.setFillColor(sf::Color::Black);
			m_menuTitle.setOrigin(sf::Vector2f(m_menuTitle.getGlobalBounds().width / 2, m_menuTitle.getGlobalBounds().height / 2));
			m_menuTitle.setPosition(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 200.0f));
		}
		virtual ~UIScreen() = default;

		virtual void updateOver(sf::Vector2i mousePosition) = 0;

		sf::Vector2f getTitlePosition() { return m_menuTitle.getPosition(); }
		void setTitlePosition(sf::Vector2f newPosition) { m_menuTitle.setPosition(newPosition); }
		void setTitleColor(sf::Color newColor) { m_menuTitle.setFillColor(newColor); }
		void setTitleSize(int newSize) { m_menuTitle.setCharacterSize(newSize); }

		void render(sf::RenderWindow& window) 
		{
			window.draw(m_spriteBackground);
			window.draw(m_menuTitle);
		}

	protected:

		sf::Font m_textFont;

	private:

		sf::Sprite m_spriteBackground;
		sf::Text m_menuTitle;
};