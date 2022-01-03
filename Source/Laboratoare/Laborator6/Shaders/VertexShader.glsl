#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float ElapsedTime;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;

void main() {
	float timeBasedValue = asin(sin(ElapsedTime));

	frag_position = v_position;
	frag_normal	= v_normal + vec3(abs(timeBasedValue), 0, 0);
	frag_texture = v_texture;
	frag_color = v_color;

	vec3 shiftedPos = v_position + vec3(timeBasedValue, 0, 0);

	vec4 pos = Model * vec4(shiftedPos, 1);
	gl_Position	= Projection * View * pos;
}