#shader vertex
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_FragPos;
out vec3 v_Normal;

uniform mat4 u_VP;

layout(std430, binding = 0) buffer positionBuffer {
	vec4 worldPositions[];
};

void main() {
	vec4 worldPos = worldPositions[gl_InstanceID];
	mat4 mvp = u_VP * mat4(	1.0, 0.0, 0.0, 0.0,
						0.0, 1.0, 0.0, 0.0,
						0.0, 0.0, 1.0, 0.0,
						worldPos.x, worldPos.y, worldPos.z, 1.0);
	gl_Position = mvp * position;
	v_TexCoord = texCoord;
	v_FragPos = worldPos.xyz;
	v_Normal = normal;
};

#shader fragment
#version 430 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;
in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec4 u_Colour = vec4(1, 1, 1, 1);
uniform sampler2D u_Texture;

uniform vec3 u_CamPos;

uniform vec3 u_LightDir;

void main() {
	float fogMaxDist = 500;
	float fogMinDist = 100;
	vec4 fogColour = vec4(0.41, 0.64, 1, 1);
	float dist = length(u_CamPos - v_FragPos);
	float fogFactor = (fogMaxDist - dist) / (fogMaxDist - fogMinDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	vec4 texColour = texture(u_Texture, v_TexCoord / 16);

	vec4 ambient = vec4(0.6, 0.6, 0.6, 1.0) * texColour * u_Colour;

	float diff = max(dot(v_Normal, -u_LightDir), 0.0);
	vec4 diffuse = vec4(0.4, 0.4, 0.4, 1.0) * diff * texColour * u_Colour;

	colour = ambient + diffuse;
	colour = mix(fogColour, colour, fogFactor);
	if (colour.a < 0.01)
		discard;
	//colour = u_Colour;
};