#pragma once

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <glad/glad.h>

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
	void Refresh(unsigned int width, unsigned int height);
	void CleanUp();
	void Quit();

	bool isRunning{ false };

	GLuint test_vertex_buffer;

};