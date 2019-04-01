#version 330 core

out vec4 fragment_colour;

in vec2 texture_coord;

layout (pixel_center_integer) in vec4 gl_FragCoord;

layout (std140) uniform Light {

	vec3 light_colour;
	float light_height;
	vec2 light_position;
	float light_range;

};

uniform sampler2D texture_data;
uniform sampler2D normal_map;
uniform sampler2D height_map;

float light_intensity = 0.2f;

vec3 scale(vec3 unscaled_vector, int min_val, int max_val, int new_min, int new_max) {

	// This function scales values from the range of 0 - 1 to -1 - 1
	// It is used after retrieving the normal values from the normal map as
	// these are automatically scaled down from 0 - 255 to 0 - 1. However,
	// in order to accurately represent a direction we need the minimum value
	// to be -1.

	vec3 scaled_vector;

	scaled_vector.x = (new_max - new_min) * ((unscaled_vector.x - min_val) / (max_val - min_val)) + new_min;
	scaled_vector.y = (new_max - new_min) * ((unscaled_vector.y - min_val) / (max_val - min_val)) + new_min;
	scaled_vector.z = (new_max - new_min) * ((unscaled_vector.z - min_val) / (max_val - min_val)) + new_min;

	return scaled_vector;

}

void main() {


	//-------------------------HEIGHT_MAP----------------------------\\

	// Getting the height value from our height map. The height map is in
	// an RGB format however we only need access to one of the values for
	// our calculations.

	vec4 height_texel = texture(height_map, texture_coord);
	float height_rgb = scale(height_texel.rgb, 0, 1, 0, 255).r;

	// The height value is represented in an RGB format on a scale of 
	// 0 - 255. This value needs to be converted to on screen pixels 
	// so that we can find the "real" position of a pixel or light on the
	// screen.
	//
	// The height map was created so that 1 pixel on screen is equal to 
	// 5 on a scale of 0 - 255. Therefore we can convert the RGB value 
	// into a pixel count with the following calculation.

	float height_pixels = (255 - height_rgb) * 0.2;
	float height_rounded = round(height_pixels) * 3;

	// e.g. 
	// A light has a position of vec2(40, 60) and a height of vec3(230, 230, 230)
	// The height in pixels is calculated as (255 - 230) * 0.2 which returns 4.
	// This means that the height of the light is equal to 4 pixels on the screen.
	// Now when doing calculations we would use the position (40, 64) instead in
	// order to take into account the height value.

	// NOTE: The current conversion also multiplies the final result by 3.
	// This is because the pixel art is being scaled up 3x from its native
	// resolution. Without this the results would appear incorrect as the light
	// would always appear to be much lower than it should be.

	vec3 pixel_position_3D = vec3(gl_FragCoord.x, height_rounded, gl_FragCoord.y - height_rounded);
	vec3 light_position_3D = vec3(light_position.x, light_height, light_position.y - light_height);

	// Here we are trying to simulate a position in a 3D environment using our 
	// 2D position and height value. The x axis is left alone as nothing needs 
	// to change there and the height value just acts as the y axis. In order
	// to simulate depth we are using the y position, this is because the art
	// is in 3/4 perspective so the higher up the screen the further away the pixel
	// is. Obviously we need to take into account height as there may be an object
	// close to the screen, however its height causes it to end up towards the top
	// of the screen. To do this we just subtract the height from the y position.


	//----------------------------TEXTURE---------------------------\\

	// Getting the texel data from our diffuse texture.

	vec4 texture_texel = texture(texture_data, texture_coord);
		
	// Calculating the distance in pixels between the light source and the 
	// fragments position. This is used to determine if the fragment is 
	// within range of the light.

	vec3 direction_vector = normalize(pixel_position_3D - light_position_3D);
	float distance_pixels = distance(light_position_3D, pixel_position_3D);

	if (distance_pixels < light_range) {


		//--------------------------NORMAL_MAP---------------------------\\

		// Getting normal value from our normal map.

		vec4 normal_texel = texture(normal_map, texture_coord);
		vec3 N = scale(normal_texel.rgb, 0, 1, -1, 1);
		
		// The normal RGB values are on a scale of 0 - 255. When returned from
		// the texture() function they are automatically normalised into the 
		// range of 0 - 1 since this is how OpenGL represents colours. The 
		// issue with this is that a direction vector needs the ability to be
		// negative to represent directions facing the opposite way so it needs
		// to be on a scale of -1 to 1.

		vec3 L = scale(direction_vector, 0, 1, -1, 1);

		// Calculating the cosine similarity. This equation takes the normalised
		// direction vectors and returns a float from -1 to 1 that represents
		// how similar the directions are. So if the equation returns 1 then the 
		// directions are identical, if it returns -1 then the directions are
		// opposite. This means that we can check if a pixel is facing the light
		// if it has a cosine similarity lower than 0.

		float cosine_similarity = dot(L, N) / (length(L) * length(N));
	
		if (cosine_similarity < 0) {

			//--------------------------ATTENUATION--------------------------\\

			// Setting the light_intensity based on the distance the pixel is from the light.

			light_intensity = 0.4f;

			if (distance_pixels < light_range * 0.55f) {
				light_intensity = 0.8f;
			}

			if (distance_pixels < light_range * 0.25f) {
				light_intensity = 1.0f;
			}

			// Calculating the light's contribution to the pixel's colour.

			fragment_colour = vec4((light_colour * light_intensity) * texture_texel.rgb, 1.0f);

		}
		else {

			//----------------------PIXEL-FACES-AWAY-------------------------\\

			// If the pixel is facing in the same direction as the light that is
			// hitting it. Then we know that it is impossible for the light to hit
			// the pixel. Therefore it should not be lit up.

			vec3 ambient_colour = light_colour * light_intensity;
			fragment_colour = vec4(texture_texel.rgb * ambient_colour, 1.0f);

		}
		
	}
	else {

		//-------------------------OUT-OF-RANGE--------------------------\\

		// If the fragment is out of range of the light then we just output
		// the ambient colour.

		vec3 ambient_colour = light_colour * light_intensity;
		fragment_colour = vec4(texture_texel.rgb * ambient_colour, 1.0f);

	}

}