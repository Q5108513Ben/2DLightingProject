#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Setup.hpp"

class Handler {

#pragma region Framework

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

#pragma endregion

#pragma region OpenGL Objects

private:

	GLuint test_shader;

	GLuint test_vertex_array;
	GLuint test_vertex_buffer;
	GLuint test_element_buffer;

#pragma endregion

#pragma region Textures

private:

	void loadImage(std::string file_location);
	std::vector<sf::Image> image_vector;

	GLuint test_image;

#pragma endregion

};