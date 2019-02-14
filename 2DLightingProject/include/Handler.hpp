#pragma once

#include <SFML/Window.hpp>

class Handler {

public:

	Handler();
	~Handler() {};

	void HandleEvents();
	void Render();

	bool Running() { return isRunning; }

private:

	sf::Window window;

	void Initialise();
	void Refresh();
	void CleanUp();
	void Quit();

	bool isRunning{ false };

};