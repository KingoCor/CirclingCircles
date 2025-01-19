#include "game.hpp"
#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

Game::State gameState;

void gameLoop() {
	gameState.Update();
	gameState.Draw();
}

int main() {

	#if defined(PLATFORM_WEB)
		emscripten_set_main_loop(gameLoop, 0, 1);
	#else
		while (!WindowShouldClose()) {
			gameLoop();
		}
	#endif
	

    return 0;
}
