#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 texture_coordinate;

out vec2 texture_coord;

void main() {

	texture_coord = texture_coordinate;
	gl_Position = vec4(vertex_position, 1.0);

}