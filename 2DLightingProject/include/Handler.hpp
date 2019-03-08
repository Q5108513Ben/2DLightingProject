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

	GLuint shader_program;

	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint element_buffer;

#pragma endregion

#pragma region Textures

private:

	void loadImage(std::string file_location);
	std::vector<sf::Image> image_vector;

	GLuint diffuse_texture;
	GLuint normal_texture;
	GLuint height_texture;

#pragma endregion

};