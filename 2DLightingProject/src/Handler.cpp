#include "Handler.hpp"

#include <SFML/OpenGL.hpp>
#include <iostream>

Handler::Handler() {

	sf::ContextSettings settings;
	settings.depthBits = 0;				// Disabling the Depth Buffer. This will be entirely 2D so we do not have access to a real z axis.
	settings.stencilBits = 0;			// Disabling the Stencil Buffer. I can't think of a reason to use this in a 2D environment since we will always be rendering a 2D texture to the screen.
	settings.antialiasingLevel = 0;		// Disabling Anti-aliasing. This will be using pixel art so there shouldn't be any aliasing issues.
	settings.majorVersion = 3;
	settings.minorVersion = 0;			// These two lines set the version of OpenGL to be used. This sets it to version 3.0 which is the standard we have been using.
	
	window.create(sf::VideoMode(1440, 810), "Real-Time 2D Lighting Demo", sf::Style::Default, settings);

	sf::ContextSettings applied_settings = window.getSettings();
	std::cout << "Window Context Settings\n\n"
		      << "         Depth Bits: " << applied_settings.depthBits << "\n"
			  << "       Stencil Bits: " << applied_settings.stencilBits << "\n"
			  << "Anti-Aliasing Level: " << applied_settings.antialiasingLevel << "\n"
			  << "     OpenGL Version: " << applied_settings.majorVersion << "." << applied_settings.minorVersion << std::endl;

	// The reason I am getting and outputting the context settings is to check for any changes. Different GPUs will allow for different settings and so if there is something there that
	// the GPU doesn't like, SFML will automatically find the closest settings that will work.

	isRunning = true;
	Initialise();

}

void Handler::Initialise() {

	glClearColor(0.36f, 0.28f, 0.54f, 0.f);

}

void Handler::Refresh() {

	// Will be used when the window is resized.

}

void Handler::Render() {

	glClear(GL_COLOR_BUFFER_BIT);

	window.display();

}

void Handler::HandleEvents() {

	sf::Event event;
	while (window.pollEvent(event)) {

		if (event.type == sf::Event::Closed) { Quit(); }
		else if (event.type == sf::Event::Resized) { Refresh(); }

	}
}


void Handler::CleanUp() {

	// Will be used for deleting OpenGL stuff.

}

void Handler::Quit() {

	CleanUp();
	window.close();
	isRunning = false;

}
