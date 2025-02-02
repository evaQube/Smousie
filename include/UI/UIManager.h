#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <UI/UIScreen_Credits.h>
#include <UI/UIScreen_GameOver.h>
#include <UI/UIScreen_MainMenu.h>
#include <UI/UIScreen_PauseMenu.h>

class UIManager 
{
	public:

		~UIManager() = default;

		void init(sf::Font textFont);
		void finish();

		void update(float deltaMilliseconds, sf::Vector2i mousePosition, bool mouseInsideScreen, bool mouseClicked);
		void render(sf::RenderWindow& window);

		void showMainMenu() { m_showMainMenu = true; }
		void showGameOver(sf::String score) { m_showGameOver = true; m_gameOver->setScore(score); }
		bool isShowingSomeMenu() const { return (m_showMainMenu || m_showPauseMenu || m_showGameOver); }

		bool m_exitRequested{ false };
		bool m_resetGameRequested{ false };

	private:

		UIScreen_MainMenu* m_mainMenu{ nullptr };
		UIScreen_Credits* m_credits{ nullptr };
		UIButton* m_pauseButton{ nullptr };
		UIScreen_PauseMenu* m_pauseMenu{ nullptr };
		UIScreen_GameOver* m_gameOver{ nullptr };

		bool m_showMainMenu{ false };
		bool m_showCredits{ false };
		bool m_showPauseMenu{ false };
		bool m_showGameOver{ false };
};