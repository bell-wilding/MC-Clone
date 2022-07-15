#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

out vec3 v_ViewDir;

uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;
uniform mat4 u_RotationMat;

void main(void) {
	vec3 pos = position;
	mat4 invproj = inverse(u_ProjMat);
	pos.xy *= vec2(invproj[0][0], invproj[1][1]);
	pos.z = -1.0f;

	v_ViewDir = transpose(mat3(u_ViewMat * u_RotationMat)) * normalize(pos);
	gl_Position = vec4(position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec3 v_ViewDir;

uniform samplerCube u_SkyTex;
//uniform sampler2D u_StarTex;
uniform float u_TimeOfDay;

void main(void) {
	vec4 samp = texture(u_SkyTex, normalize(v_ViewDir));
	//vec4 star = texture(u_StarTex, v_TexCoord);
	float t = 1.0;
	if (u_TimeOfDay > 0.75) {
		t = clamp((u_TimeOfDay - 0.75) * 8.0, 0.0, 1.0);
	}
	else if (u_TimeOfDay > 0.25) {
		t = clamp(1 - ((u_TimeOfDay - 0.25) * 8.0), 0.0, 1.0);
	}

	if (samp.b > 0.995)
		colour = mix(samp, vec4(0, 0, 0, 1), t * 0.8);
	else
		colour = colour = mix(samp, vec4(0, 0, 0, 1), t * 0.2);
	//colour = pow(samp, vec4(2.2f));
}