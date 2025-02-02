#pragma once

#include <UI/UIScreen.h>

class UIScreen_PauseMenu : public UIScreen
{
public:

	UIScreen_PauseMenu(sf::String menuTitle, sf::Texture* backgroundTexture, sf::Font& textFont);

	void updateOver(sf::Vector2i mousePosition) override;
	void render(sf::RenderWindow& window);

	bool getReturnClicked(sf::Vector2i& mousePosition) { return m_buttonReturn.isClicked(mousePosition); }
	bool getExitClicked(sf::Vector2i& mousePosition) { return m_buttonExit.isClicked(mousePosition); }

private:

	UIButton m_buttonReturn;
	UIButton m_buttonExit;
};