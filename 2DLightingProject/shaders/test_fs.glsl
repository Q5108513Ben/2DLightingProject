#version 330 core

out vec4 fragment_colour;

in vec2 texture_coord;

uniform sampler2D texture_data;

void main() {
	
	fragment_colour = texture(texture_data, texture_coord);

}