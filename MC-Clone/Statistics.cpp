#include "Statistics.h"

void Statistics::Update(float dt) {
	elapsedTime += dt;
	frames++;

	if (elapsedTime > 0.5f) {
		fps = frames * 0.5f + fps * 0.5f;
		frames = 0;
		elapsedTime = 0;
	}
}
