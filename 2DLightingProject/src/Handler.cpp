#include "Handler.hpp"

Handler::Handler() {

	sf::ContextSettings settings;
	settings.depthBits = 0;				// Disabling the Depth Buffer. This will be entirely 2D so we do not have access to a real z axis.
	settings.stencilBits = 0;			// Disabling the Stencil Buffer. I can't think of a reason to use this in a 2D environment since we will always be rendering a 2D texture to the screen.
	settings.antialiasingLevel = 0;		// Disabling Anti-aliasing. This will be using pixel art so there shouldn't be any aliasing issues.
	settings.majorVersion = 3;
	settings.minorVersion = 3;			// These two lines set the version of OpenGL to be used. This sets it to version 3.3 which is the standard we have been using.
	
	window.create(sf::VideoMode(1440, 810), "Real-Time 2D Lighting Demo", sf::Style::Default, settings);

	sf::ContextSettings applied_settings = window.getSettings();
	std::cout << "Window Context Settings\n\n"
		      << "         Depth Bits: " << applied_settings.depthBits << "\n"
			  << "       Stencil Bits: " << applied_settings.stencilBits << "\n"
			  << "Anti-Aliasing Level: " << applied_settings.antialiasingLevel << "\n"
			  << "     OpenGL Version: " << applied_settings.majorVersion << "." << applied_settings.minorVersion << std::endl;

	// The reason I am getting and outputting the context settings is to check for any changes. Different GPUs will allow for different settings and so if there is something there that
	// the GPU doesn't like, SFML will automatically find the closest settings that are valid.

	if (!gladLoadGL()) {
		std::cerr << "GLAD failed to load OpenGL context!" << std::endl;
	}

	// I have decided to use GLAD in order to load all of the OpenGL functions for me. This previous line gets GLAD to load the current OpenGL context.

	isRunning = true;
	initialise();

}

void Handler::initialise() {

	#pragma region Shader Setup

	GLuint vertex_shader = setup::shader(GL_VERTEX_SHADER, "shaders/vertex_s.glsl");
	GLuint fragment_shader = setup::shader(GL_FRAGMENT_SHADER, "shaders/fragment_s.glsl");

	shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	setup::checkLink(shader_program);
	
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	#pragma endregion

	#pragma region Vertex Buffer Setup

	// Creating the vertex position data for a full-screen quad
	float vertex_data[] = {
		// Positions			// Texture Coordinates
		-1.0f,  1.0f, 0.0f,		0.0f, 0.0f,				// Top Left
		 1.0f,  1.0f, 0.0f,  	1.0f, 0.0f,				// Top Right
		-1.0f, -1.0f, 0.0f,		0.0f, 1.0f,				// Bottom Left
		 1.0f, -1.0f, 0.0f,		1.0f, 1.0f				// Bottom Right
	};

	unsigned int elements[] = {
		0, 1, 3,
		0, 3, 2
	};

	// Creating vertex array
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	// Creating vertex buffer 
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	// Creating element buffer
	glGenBuffers(1, &element_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// Setting vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	#pragma endregion

	#pragma region Texture Loading

	// This function loads an image through SFML's sf::Image class. It is then stored within a vector for later use.
	loadImage("images/diffuse_texture.png");
	loadImage("images/normal_texture.png");
	loadImage("images/height_texture.png");

	// Generating and binding the texture we will be using when rendering.
	glGenTextures(1, &diffuse_texture);
	glGenTextures(1, &normal_texture);
	glGenTextures(1, &height_texture);

	glBindTexture(GL_TEXTURE_2D, diffuse_texture);

	// This sets the filter for when the texture is magnified. 
	// We will be using low resolution pixel art so we do not want OpenGL to attempt to smooth the texture as it is scaled up.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Adding data to the OpenGL texture.
	// This is retrieved from the SFML image we loaded in previously.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_vector[0].getSize().x, image_vector[0].getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_vector[0].getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, normal_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_vector[1].getSize().x, image_vector[1].getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_vector[1].getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, height_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_vector[2].getSize().x, image_vector[2].getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_vector[2].getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);
	
	#pragma endregion

	glClearColor(0.3f, 0.5f, 0.5f, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}

void Handler::refresh(unsigned int width, unsigned int height) {

	glViewport(0, 0, width, height);

}

void Handler::render() {

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shader_program);
	glBindVertexArray(vertex_array);

	glBindTexture(GL_TEXTURE_2D, diffuse_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal_texture);
	glUniform1i(glGetUniformLocation(shader_program, "normal_map"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, height_texture);
	glUniform1i(glGetUniformLocation(shader_program, "height_map"), 2);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	window.display();

}

void Handler::handleEvents() {

	sf::Event event;
	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) { quit(); }
		else if (event.type == sf::Event::Resized) { refresh(event.size.width, event.size.height); }

	}
}

void Handler::cleanUp() {

	glDeleteShader(shader_program);
	glDeleteVertexArrays(1, &vertex_array);
	glDeleteBuffers(1, &vertex_buffer);

}

void Handler::quit() {

	cleanUp();
	window.close();
	isRunning = false;

}

void Handler::loadImage(std::string file_location) {

	sf::Image image;

	if (image.loadFromFile(file_location)) {
		image_vector.push_back(image);
	}


}
