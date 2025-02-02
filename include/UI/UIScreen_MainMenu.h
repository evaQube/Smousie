#pragma once

#include <UI/UIScreen.h>

class UIScreen_MainMenu : public UIScreen
{
	public:
		
		UIScreen_MainMenu(sf::String menuTitle, sf::Texture* backgroundTexture, sf::Font& textFont,
						  sf::Texture* mouseTexture);
		
		void update(float deltaMilliseconds);
		void updateOver(sf::Vector2i mousePosition) override;
		void render(sf::RenderWindow& window);

		bool getPlayClicked(sf::Vector2i& mousePosition) { return m_buttonPlay.isClicked(mousePosition); }
		bool getCreditsClicked(sf::Vector2i& mousePosition) { return m_buttonCredits.isClicked(mousePosition); }
		bool getExitClicked(sf::Vector2i& mousePosition) { return m_buttonExit.isClicked(mousePosition); }

	private:

		UIButton m_buttonPlay;
		UIButton m_buttonCredits;
		UIButton m_buttonExit;

		sf::Sprite m_spriteMouse;
		float m_tileWidth{ .0f };
		float m_tileHeight{ .0f };
		int m_frame{ 0 };
		float m_timeElapsedMouseAnimation{ .0f };
		void updateMouseAnimation(float deltaMilliseconds);
};