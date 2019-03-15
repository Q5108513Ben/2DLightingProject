#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Setup.hpp"

class Handler {

#pragma region Structs

private:

	struct vec3 {
		float r, g, b;
		vec3(float x, float y, float z) : r(x), g(y), b(z) {};
	};

	struct vec2 {
		float x, y;
		vec2(float a, float b) : x(a), y(b) {};
	};

	struct LightData {

		vec3 colour;
		float intensity;
		vec2 position;
		float height;
		float range;

	};

#pragma endregion

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

#pragma region Controls

private:

	sf::Vector2i mouse_position;
	float mouse_scroll{ 120.f };

#pragma endregion

#pragma region OpenGL Objects

private:

	GLuint shader_program;

	GLuint vertex_array;
	GLuint vertex_buffer;
	GLuint element_buffer;
	GLuint uniform_buffer;

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