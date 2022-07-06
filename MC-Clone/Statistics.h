#pragma once

class Statistics {
public:
	Statistics() {};
	~Statistics() {};

	void Update(float dt);

	float GetFPS() { return fps; }

protected:
	float fps = 30, elapsedTime = 0;
	unsigned int frames = 0;
};