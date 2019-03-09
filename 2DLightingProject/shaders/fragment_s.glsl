#version 330 core

out vec4 fragment_colour;

in vec2 texture_coord;

layout (std140) uniform Light {

	vec3 light_colour;
	float light_intensity;
	vec2 light_position;
	float light_height;
	float light_range;

};

uniform sampler2D texture_data;
uniform sampler2D normal_map;
uniform sampler2D height_map;

void main() {
	
	float ambient_light = 0.1f;

	vec4 diffuse_texel  = texture(texture_data, texture_coord);

	fragment_colour = diffuse_texel * ambient_light;
	
	//fragment_colour = texture(normal_map, texture_coord);		// Tests to check that the shader was receiving the
	//fragment_colour = texture(height_map, texture_coord);		// correct data from the sampler2D's.
	//fragment_colour = vec4(light_colour, 1.f);				// Another test to check the Light data was being
																// passed correctly.

}