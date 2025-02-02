#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class UIButton
{
	public:

        UIButton() = default;

        void setButton(const std::string& text, const sf::Vector2f& position, sf::Font& font,
                 const int textSize = 40, const sf::Color textColor = sf::Color::Black,
                 const sf::Vector2f boxSize = sf::Vector2f(210.0f, 80.0f), const sf::Color boxColor = sf::Color(255, 229, 250, 255),
                 const sf::Color boxOverColor = sf::Color(216, 255, 222, 255))
        {
            m_text.setFont(font);
            m_text.setString(text);
            m_text.setCharacterSize(textSize);
            m_text.setFillColor(textColor);
            m_text.setOrigin(sf::Vector2f(m_text.getGlobalBounds().width / 2, m_text.getGlobalBounds().height / 2 + 10.0f));
            m_text.setPosition(position);

            m_box.setSize(boxSize);
            m_box.setOrigin(sf::Vector2f(m_box.getGlobalBounds().width / 2, m_box.getGlobalBounds().height / 2));
            m_box.setPosition(position);
            m_boxInitColor = boxColor;
            m_boxOverColor = boxOverColor;
            m_box.setFillColor(m_boxInitColor);
        }
        void setInvisibleButton(const sf::Vector2f& position, const sf::Vector2f boxSize = sf::Vector2f(100.0f, 100.0f))
        {
            m_box.setSize(boxSize);
            m_box.setPosition(position);
            m_box.setFillColor(sf::Color::Transparent);
        }
		~UIButton() = default;

        sf::Vector2f getPosition() const { return m_text.getPosition(); }

        void render(sf::RenderWindow& window) 
        {
            window.draw(m_box);
            window.draw(m_text);
        }

        bool isClicked(sf::Vector2i& mousePosition) const 
        {
            return m_box.getGlobalBounds().contains(sf::Vector2f(mousePosition)); 
        }

        void checkOver(sf::Vector2i& mousePosition) 
        {
            if (isClicked(mousePosition)) { m_box.setFillColor(m_boxOverColor); }
            else { m_box.setFillColor(m_boxInitColor); }
        }

	private:
        sf::Text m_text;
        sf::RectangleShape m_box;

        sf::Color m_boxInitColor;
        sf::Color m_boxOverColor;
};