#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main() {
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;

uniform vec4 u_Colour = vec4(1, 1, 1, 1);
uniform sampler2D u_Texture;

void main() {
	vec4 texColour = texture(u_Texture, v_TexCoord / 16);
	colour = texColour * u_Colour;
	if (colour.a < 0.01)
		discard;
	//colour = u_Colour;
};