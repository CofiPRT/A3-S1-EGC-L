#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;

uniform bool use_2_textures;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// calculate the out_color using the texture2D() function
	if (use_2_textures)
		out_color = mix(texture2D(texture_1, texcoord), texture2D(texture_2, texcoord), 0.5);
	else
		out_color = texture2D(texture_1, texcoord);

	// filter specific alpha levels
	if (out_color.a < 0.5)
		discard;

}