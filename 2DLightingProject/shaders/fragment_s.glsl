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
	
	// Getting the texel data from our diffuse texture.
	vec4 diffuse_texel  = texture(texture_data, texture_coord);
		
	// Calculating the distance in pixels between the light source and the 
	// fragments position. This is used to determine if the fragment is 
	// within range of the light.
	vec2 distance_vector = light_position - gl_FragCoord.xy;
	float distance_pixels = length(distance_vector);

	if (distance_pixels < light_range) {

		// Getting the height data for the current pixel. Since the height map is
		// in greyscale we only need to compare one value to the light_height float
		vec4 height_texel = texture(height_map, texture_coord);

		if (height_texel.x > light_height) {

			// If the fragment is within range of the light then we have to apply
			// the lights colour. First we multiply the light colour with the 
			// intensity scalar to find the final light colour to be applied.
			vec3 final_light_colour = light_colour * light_intensity;
			fragment_colour = diffuse_texel * vec4(final_light_colour, 1.f);

		}
		else {

			// If the fragment is higher than the light then we just output
			// the ambient colour.
			float ambient_light = 0.1f;
			fragment_colour = diffuse_texel * ambient_light;

		}

	}
	else {

		// If the fragment is out of range of the light then we just output
		// the ambient colour.
		float ambient_light = 0.1f;
		fragment_colour = diffuse_texel * ambient_light;

	}

}