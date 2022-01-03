#version 330

#define EARTH_ROTATION_SPEED_FACTOR (0.2f)

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float elapsed_time;

out vec2 texcoord;

void main()
{
	// pass v_texture_coord as output to Fragment Shader
	if (elapsed_time > 0)
		texcoord = vec2(v_texture_coord.x - elapsed_time * EARTH_ROTATION_SPEED_FACTOR, v_texture_coord.y);
	else
		texcoord = v_texture_coord;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
