#pragma once

#include <cmath>

namespace Settings {
	inline constexpr int WIDTH = 1080;
	inline constexpr int HEIGHT = 1920;

	inline constexpr float WIDTHf = 1080;
	inline constexpr float HEIGHTf = 1920;

	inline constexpr float TRANSITION_TIME = 100;

	inline constexpr float ENEMIES_SPEED = M_PI/(15*60);

	inline constexpr float DEFAULT_PROJECTILES_COOLDOWN = 60;
	inline constexpr float FREEZE_PROJECTILES_COOLDOWN = 180;

	inline constexpr float START_FREEZE = 0.1;
	inline constexpr float MAX_FREEZE = 0.7;
}
