#pragma once

#include <cmath>

namespace Settings {
	inline constexpr int WIDTH = 720;
	inline constexpr int HEIGHT = 1280;

	inline constexpr float WIDTHf = 720;
	inline constexpr float HEIGHTf = 1280;

	inline constexpr float TRANSITION_TIME = 100;

	inline constexpr float ENEMIES_SPEED = M_PI/(15*60);

	inline constexpr float DEFAULT_PROJECTILES_COOLDOWN = 60;
	inline constexpr float FREEZE_PROJECTILES_COOLDOWN = 180;

	inline constexpr float START_FREEZE = 0.1;
}
