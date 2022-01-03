#version 330

#define CONSTANT_ATTENUATION_FACTOR (1.0)
#define LINEAR_ATTENUATION_FACTOR	(0.25)
#define SQUARED_ATTENUATION_FACTOR	(0.125)

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

void main() {
	// compute world space vectors
	vec3 world_position = (Model * vec4(v_position, 1)).xyz;			// from object space
	vec3 world_normal = normalize(mat3(Model) * normalize(v_normal));	// keep the result normalized

	vec3 L = normalize(light_position - world_position);	// light direciton
	vec3 V = normalize(eye_position - world_position);		// observer direction
	vec3 H = normalize(L + V);								// median
	vec3 R = normalize(reflect(-L, world_normal));			// perfect reflection

	// define ambient light component
	float ambient_light = 0.25;

	// compute diffuse light component
	float diffuse_light = material_kd * max(dot(world_normal, L), 0); // cannot be lower than 0

	// compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0) {
		specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
	}

	// compute light
	float distance = distance(light_position, v_position);

	float attenuation_factor = 1 / (CONSTANT_ATTENUATION_FACTOR +
									LINEAR_ATTENUATION_FACTOR * distance +
									SQUARED_ATTENUATION_FACTOR * distance * distance);

	float light = ambient_light + attenuation_factor * (diffuse_light + specular_light); // this is the formula

	// TODO: send color light output to fragment shader
	color = object_color * light;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
