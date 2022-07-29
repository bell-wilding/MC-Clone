#shader vertex
#version 430 core

layout(location = 0) in vec2 position;

out vec2 v_TexCoord;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	v_TexCoord = (position + 1.0) / 2.0;
};

#shader fragment
#version 430 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;

uniform sampler2D u_ScreenTexture;
uniform float u_Offset;
uniform bool u_UnderWater;

void main() {
	//colour = vec4(vec3(1.0 - texture(u_ScreenTexture, v_TexCoord)), 1.0);
	if (u_UnderWater) {
		vec2 texcoord = v_TexCoord;
		texcoord.x += sin(texcoord.y * 4 * 2 * 3.14159 + u_Offset) / 400;
		vec4 defaultColour = texture(u_ScreenTexture, texcoord);
		colour = mix(defaultColour, vec4(0, 0.0196, 0.3882, 1), 0.7);
	}
	else {
		colour = texture(u_ScreenTexture, v_TexCoord);
	}
};
