#pragma once

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "Setup.hpp"

class Handler {

public:

	Handler();
	~Handler() {};

	void handleEvents();
	void render();

	bool running() { return isRunning; }

private:

	sf::Window window;

	void initialise();
	void refresh(unsigned int width, unsigned int height);
	void cleanUp();
	void quit();

	bool isRunning{ false };

	GLuint test_shader;

	GLuint test_vertex_buffer;

};