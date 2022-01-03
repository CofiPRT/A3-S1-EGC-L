#version 330

// get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform float cut_off;
uniform int light_type; // no bools in glsl, use 0 for normal and 1 for spot-light

layout(location = 0) out vec4 out_color;

void main()
{
	// acquire vectors
	vec3 N = normalize(world_normal);

	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);

	vec3 R = normalize(reflect(L, world_normal));
	vec3 H = normalize(L + V);

	// define ambient light component
	float ambient_light = 0.25;

	// compute diffuse light component
	float diffuse_light = material_kd * max(0, dot(normalize(N), L));

	// compute specular light component
	float specular_light = diffuse_light > 0 ? material_ks * pow(max(0, dot(N, H)), material_shininess) : 0;

	// compute light
	float light = 0;

	if (light_type == 0) {
		
		// normal light type
		float dist = distance(light_position, world_position);
		float att = 1 / max(1, dist * dist); // "faster" than the distance change

		light = ambient_light + att * (diffuse_light + specular_light);

	} else if (light_type == 1) {
		
		// spot light type
		float spot_light_max = cos(cut_off);
		float spot_light_curr = dot(-L, light_direction);

		if (spot_light_curr < spot_light_max) {

			// don't override the light from other objects
			light = ambient_light;

		} else {
			
			// use quadratic attenuation
			float spot_light_rel_diff = (spot_light_curr - spot_light_max) / (1 - spot_light_max);
			float att = spot_light_rel_diff * spot_light_rel_diff;
			
			light = ambient_light + att * (diffuse_light + specular_light);

		}

	}

	// write pixel out color
	out_color = vec4(object_color * light, 1);
}