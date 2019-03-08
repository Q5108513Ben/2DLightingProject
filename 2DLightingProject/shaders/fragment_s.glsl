#version 330 core

out vec4 fragment_colour;

in vec2 texture_coord;

uniform sampler2D texture_data;
uniform sampler2D normal_map;
uniform sampler2D height_map;

void main() {
	
	float ambient_light = 0.1f;

	vec4 diffuse_texel  = texture(texture_data, texture_coord);

	fragment_colour = diffuse_texel * ambient_light;
	//fragment_colour = texture(normal_map, texture_coord);		// Tests to check that the shader was receiving the
	//fragment_colour = texture(height_map, texture_coord);		// correct data from the sampler2D's.

}