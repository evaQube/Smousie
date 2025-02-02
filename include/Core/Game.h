#pragma once

#include <Core/World.h>
#include <cstdint>
#include <string>
#include <UI/UIManager.h>

namespace sf
{
	class RenderWindow;
}

class World;

class Game
{
	public:

		struct GameCreateInfo
		{
			std::string gameTitle;
			uint32_t frameRateLimit{ 60u };
		};

		~Game();

		bool init(GameCreateInfo& createInfo);
		void finish();
		void replay();

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

	private:

		GameCreateInfo m_createInfo;

		sf::RenderWindow* m_window{ nullptr };
		UIManager* m_uiManager{ nullptr };
		World* m_world{ nullptr };
		sf::Font m_textFont;

		sf::Vector2i m_mousePosition{ 0, 0 };
		bool m_mouseInsideScreen{ false };
		bool m_mousePressed{ false };
		bool m_mouseClicked{ false };
};