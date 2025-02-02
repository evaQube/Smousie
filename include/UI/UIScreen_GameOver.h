#pragma once

#include <UI/UIScreen.h>

class UIScreen_GameOver : public UIScreen
{
public:

	UIScreen_GameOver(sf::String menuTitle, sf::Texture* backgroundTexture, sf::Font& textFont);

	void updateOver(sf::Vector2i mousePosition) override;
	void render(sf::RenderWindow& window);

	bool getPlayClicked(sf::Vector2i& mousePosition) { return m_buttonPlay.isClicked(mousePosition); }
	bool getExitClicked(sf::Vector2i& mousePosition) { return m_buttonExit.isClicked(mousePosition); }
	void setScore(sf::String score) { m_textScore.setString(score); }

private:

	sf::Text m_textScore;
	UIButton m_buttonPlay;
	UIButton m_buttonExit;
};