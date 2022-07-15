#shader vertex
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_FragPos;
out vec3 v_Normal;

uniform mat4 u_VP;

void main() {
	mat4 mvp = u_VP * mat4(	1.0, 0.0, 0.0, 0.0,
							0.0, 1.0, 0.0, 0.0,
							0.0, 0.0, 1.0, 0.0,
							0.0, 0.0, 0.0, 1.0);
	gl_Position = mvp * position;
	v_TexCoord = texCoord;
	v_FragPos = position.xyz; // FIX
	v_Normal = normal;
};

#shader fragment
#version 430 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;
in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec4 u_Colour = vec4(1, 1, 1, 1);
uniform vec4 u_FogColour = vec4(0.41, 0.64, 1, 1);
uniform sampler2D u_Texture;

uniform vec3 u_CamPos;

uniform vec4 u_SunLight;
uniform vec4 u_MoonLight;

void main() {
	float fogMaxDist = 300;
	float fogMinDist = 250;
	float dist = length(u_CamPos - v_FragPos);
	float fogFactor = (fogMaxDist - dist) / (fogMaxDist - fogMinDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	vec4 texColour = texture(u_Texture, v_TexCoord / 16);

	if (texColour.a < 0.01) {
		discard;
	}

	float avg = (u_SunLight.w + u_MoonLight.w) * 0.5;
	vec4 ambient = vec4(avg, avg, avg, 1.0) * texColour * u_Colour;

	float diffSun = max(dot(v_Normal, -u_SunLight.xyz), 0.0) * u_SunLight.w;
	float diffMoon = max(dot(v_Normal, -u_MoonLight.xyz), 0.0) * (u_MoonLight.w * 2);
	vec4 diffuse = vec4(0.5, 0.5, 0.5, 1.0) * (diffSun + diffMoon) * texColour * u_Colour;

	colour = ambient + diffuse;
	colour = mix(u_FogColour, colour, fogFactor);

	/*if (v_TexCoord.x - floor(v_TexCoord.x) < 0.0075 || v_TexCoord.x - floor(v_TexCoord.x) > 0.9925 || v_TexCoord.y - floor(v_TexCoord.y) < 0.0075 || v_TexCoord.y - floor(v_TexCoord.y) > 0.9925) {
		colour = vec4(0.25, 0.25, 0.25, 1);
	}
	else*/

	colour.a = texColour.a;

};
