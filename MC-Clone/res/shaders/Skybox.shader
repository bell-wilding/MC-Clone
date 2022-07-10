#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

out vec3 v_ViewDir;

uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;
uniform mat4 u_RotationMat;

void main(void) {
	//vec3 pos = (vec4(position, 1) * u_RotationMat).xyz;
	vec3 pos = position;
	mat4 invproj = inverse(u_ProjMat);
	pos.xy *= vec2(invproj[0][0], invproj[1][1]);
	pos.z = -1.0f;

	//pos = (vec4(pos, 1) * u_RotationMat).xyz;

	v_ViewDir = transpose(mat3(u_ViewMat * u_RotationMat)) * normalize(pos);
	gl_Position = vec4(position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec3 v_ViewDir;

uniform samplerCube u_SkyTex;
uniform float u_TimeOfDay;

void main(void) {
	vec4 samp = texture(u_SkyTex, normalize(v_ViewDir));
	float t = (u_TimeOfDay > 0.5 ? (u_TimeOfDay - 0.5) * 2 : 1 - u_TimeOfDay * 2) * 0.9;
	colour = mix(pow(samp, vec4(2.2f)), vec4(0.023, 0.05, 0.101, 1), t);
	//colour = pow(samp, vec4(2.2f));
}