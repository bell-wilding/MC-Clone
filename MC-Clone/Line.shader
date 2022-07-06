#shader vertex
#version 430 core

layout(location = 0) in vec4 position;

uniform mat4 u_VP;

void main() {
	mat4 mvp = u_VP * mat4(1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);
	gl_Position = mvp * position;
};

#shader fragment
#version 430 core

layout(location = 0) out vec4 colour;

uniform vec4 u_Colour = vec4(1, 1, 1, 1);

void main() {
	colour = vec4(1, 1, 1, 1);
};
