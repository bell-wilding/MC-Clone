#pragma once

#include "Shader.h"
#include "Texture.h"
#include "PerspectiveCamera.h"

class Skybox {
public:
	Skybox();
	~Skybox();

	void Render(float dt, PerspectiveCamera* camera);

	float GetTimeOfDay() { return currentDayTime / dayLengthInSeconds; }

	glm::vec4 GetFogColour() { return fogColour; }

protected:
	Shader shader;

	unsigned int vao;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;

	unsigned int skyboxTex;
	Texture starTex;

	float rotation;
	float currentDayTime;
	float dayLengthInSeconds;

	glm::vec4 fogColour;
};