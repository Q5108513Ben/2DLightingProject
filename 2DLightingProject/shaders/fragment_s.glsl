#version 330 core

out vec4 fragment_colour;

in vec2 texture_coord;

layout (std140) uniform Light {

	vec3 light_colour;
	float light_height;
	vec2 light_position;
	float light_range;

};

uniform sampler2D texture_data;
uniform sampler2D normal_map;
uniform sampler2D height_map;

float ambient_intensity = 0.1f;

void main() {


	//-------------------------HEIGHT_MAP----------------------------\\

	// Getting the height value from our height map. The height map is in
	// an RGB format however we only need access to one of the values for
	// our calculations.

	vec4 height_texel = texture(height_map, texture_coord);
	float height_rgb = height_texel.r;

	// The height value is represented in an RGB format on a scale of 
	// 0 - 255. This value needs to be converted to on screen pixels 
	// so that we can find the "real" position of a pixel or light on the
	// screen.
	//
	// The height map was created so that 1 pixel on screen is equal to 
	// 5 on a scale of 0 - 255. Therefore we can convert the RGB value 
	// into a pixel count with the following calculation.

	float height_pixels = (255 - height_rgb) * 0.2f * 3;

	// With the height in pixels calculated we can add this to the y
	// component of a 2D position to find its position in a psuedo-3D 
	// environment.

	vec2 pixel_position_3D = vec2(gl_FragCoord.x, gl_FragCoord.y);
	vec2 light_position_3D = vec2(light_position.x, light_position.y + light_height);

	// e.g. 
	// A light has a position of vec2(40, 60) and a height of vec3(230, 230, 230)
	// The height in pixels is calculated as (255 - 230) * 0.2 which returns 4.
	// This means that the height of the light is equal to 4 pixels on the screen.
	// Now when doing calculations we would use the position (40, 64) instead in
	// order to take into account the height value.

	// NOTE: The current conversion additionally multiplies the final result by
	// 3. This is because the pixel art is being scaled up 3x from its native
	// resolution. Without this the results would appear incorrect as the light
	// would always appear to be much lower than it should be.


	//----------------------------TEXTURE---------------------------\\

	// Getting the texel data from our diffuse texture.

	vec4 texture_texel = texture(texture_data, texture_coord);
		
	// Calculating the distance in pixels between the light source and the 
	// fragments position. This is used to determine if the fragment is 
	// within range of the light.

	vec2 direction_vector = light_position_3D - pixel_position_3D;
	vec2 distance_vector = light_position - gl_FragCoord.xy;
	float distance_pixels = length(distance_vector);

	if (distance_pixels < light_range) {


		//--------------------------NORMAL_MAP---------------------------\\

		// Getting normal value from our normal map.

		vec4 normal_texel = texture(normal_map, texture_coord);
		
		// Calculating L and N. The distance vector can be used as both
		// distance and direction.

		vec3 L = vec3(normalize(direction_vector), 1.0f);
		vec3 N = normalize(normal_texel.rgb);

		// Calculates the strength of the light based on the angle it hits 
		// the pixel. The closer the result is to 0, the stronger the light
		// will be.

		float diffuse_intensity = max(dot(L, N), 0.0f);
		
		//--------------------------ATTENUATION--------------------------\\

		// Setting the light_intensity based on the distance the pixel is from the light.

		float attenuation = 0.4f;

		if (distance_pixels < light_range * 0.55f) {
			attenuation = 0.8f;
		}

		if (distance_pixels < light_range * 0.25f) {
			attenuation = 1.0f;
		}

		vec3 ambient_colour = (light_colour * ambient_intensity) * attenuation;
		vec3 diffuse_colour = (light_colour * diffuse_intensity) * attenuation;

		fragment_colour = vec4((ambient_colour + diffuse_colour) * texture_texel.rgb, 1.0f);

	}
	else {

		//-------------------------OUT-OF-RANGE--------------------------\\

		// If the fragment is out of range of the light then we just output
		// the ambient colour.

		vec3 ambient_colour = light_colour * ambient_intensity;
		fragment_colour = vec4(texture_texel.rgb * ambient_colour, 1.0f);

	}

}