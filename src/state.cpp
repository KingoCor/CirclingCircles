#include "game.hpp"
#include "settings.hpp"
#include <cstdlib>
#include <raylib.h>

using namespace Game;

State::State() {
    InitWindow(Settings::WIDTH, Settings::HEIGHT, "Circling Circles");
    SetTargetFPS(60);
	frame = LoadRenderTexture(Settings::WIDTH, Settings::HEIGHT);
}

State::~State() {
	UnloadRenderTexture(this->frame);
	CloseWindow();
}

void State::Reset() {
	score = 0;
	player = Player();
	enemies.clear();
	needSpecial = 0;
	isShoping = false;
	isUpgrading = false;
	upgradingIndex = -1;
}

void State::Update() {
	switch (screen) {
		case MAIN_MENU: UpdateMainMenu(); break;
		case GAME:      UpdateGame(); break;
	}
}

void State::Draw() {
	float currentScreenWidth = GetScreenWidth();
	float currentScreenHeight = GetScreenHeight();

	BeginTextureMode(frame);
		switch (screen) {
			case MAIN_MENU: DrawMainMenu(); break;
			case GAME:      DrawGame(); break;
		}
	EndTextureMode();

	BeginDrawing();
		DrawTexturePro(
			frame.texture, 
			(Rectangle){0,0,static_cast<float>(Settings::WIDTH),-static_cast<float>(Settings::HEIGHT)}, 
			(Rectangle){0,0,currentScreenWidth,currentScreenHeight}, 
			(Vector2){0,0}, 0, WHITE
		);
	EndDrawing();
}

void State::UpdateMainMenu() {
	if (transition<0 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		transition=0;
	} else if (transition>=0 && transition<Settings::TRANSITION_TIME) {
		transition++;
	} else if (transition>=Settings::TRANSITION_TIME){
		transition=-1;
		Reset();
		screen=GAME;
	}
}

void State::DrawMainMenu() {
	ClearBackground(RAYWHITE);

	std::string scoreText = std::to_string(score);

	float y = Settings::HEIGHTf*0.25;
	if (transition>=0) {
		float p = transition/Settings::TRANSITION_TIME;
		y-=Settings::HEIGHTf*0.3f*(p-sinf(M_PI*p)*0.5);
	}

	DrawText(scoreText.c_str(), Settings::WIDTHf*0.5-MeasureText(scoreText.c_str(), 40), y, 40, GRAY);

	player.Draw();
}

void State::UpdateGame() {
	if (isShoping) {
		UpdateShop();
	} else if (transition<0) {
		player.Update();
		player.DealDamage(this->enemies);
		UpdateEnemies();
	} else {
		transition += 1;
		float p = transition/Settings::TRANSITION_TIME;
		player.rotation *= 1-p;

		for (auto &enemy : enemies) {
			float x = enemy.collider.x-Settings::WIDTHf*0.5;
			float y = enemy.collider.y-Settings::HEIGHT*0.5;
			
			float l = sqrtf(x*x+y*y);
			float lNext = l+(Settings::HEIGHTf-l)*p;

			enemy.collider.x = x*lNext/l+Settings::WIDTHf*0.5;
			enemy.collider.y = y*lNext/l+Settings::HEIGHTf*0.5;
		}

		if (transition>=Settings::TRANSITION_TIME) {
			transition = -1;
			screen = MAIN_MENU;
		}
	}
}

void State::DrawGame() {
	ClearBackground(RAYWHITE);

	for (auto enemy : enemies) enemy.Draw();

	if (transition>=0) {
		float p = 1-transition/Settings::TRANSITION_TIME;
		float y = Settings::HEIGHTf*(0.25-0.3f*(p-sinf(M_PI*p)*0.5));

		std::string scoreText = std::to_string(score);
		DrawText(scoreText.c_str(), Settings::WIDTHf*0.5-MeasureText(scoreText.c_str(), 40), y, 40, GRAY);
	}

	player.Draw();

	if (isShoping) DrawShop();
}

void State::UpdateEnemies() {
	bool canSpawn = true;
	Game::Enemy newEnemy = Game::Enemy(
		1+score/10,
		Settings::ENEMIES_SPEED,
		needSpecial>0
	);

	for (auto enemy = enemies.begin(); enemy!=enemies.end(); ) {
		if (enemy->getHp()<=0) {
			if (enemy->IsSpecial()) OpenShop();

			enemy = this->enemies.erase(enemy);

			this->score+=1;
			if (score%10==0) needSpecial+=1;

			continue;
		}
			
		enemy->Update();
		if (canSpawn && newEnemy.collider.Collide(enemy->collider)) 
			canSpawn = false;


		if (enemy->collider.Collide(player.collider)) 
			transition = 0;
		

		enemy++;
	}

	if (canSpawn) {
		enemies.push_back(newEnemy);
		if (needSpecial>0) needSpecial-=1;
	}
}

void State::OpenShop() {
	isShoping = true;

	if (GetRandomValue(0, 1)==1) {
		this->isUpgrading = true;

		std::vector<int> upgradableIndexes;
		for (int i=0; i<player.ammo.size(); i++) {
			if (player.ammo.at(i)->IsUpgradable())
				upgradableIndexes.push_back(i);
		}

		this->upgradingIndex = upgradableIndexes.at(
			GetRandomValue(0,upgradableIndexes.size()-1)
		);
	} else {
		switch (GetRandomValue(0, 1)) {
			case 0: shopItem = std::make_unique<Game::DefaultProjectile>(0, 0, 5, (Vector2){0,0}, 1); break;
			default: shopItem = std::make_unique<Game::FreezeProjectile>(0, 0, 5, (Vector2){0,0}, Settings::START_FREEZE); break;
		}
		
	}
}

void State::UpdateShop() {
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;

	if (GetMouseY()>Settings::HEIGHTf*0.5) {
		player.maxAmmo += 1;
	} else if (isUpgrading) {
		player.ammo.at(upgradingIndex)->Upgrade();
	} else {
		if (player.maxAmmo<=player.ammo.size()) {
			player.ammo.pop_back();
		}
		player.ammo.push_back(std::move(shopItem));
	}

	isShoping = false;
	isUpgrading = false;
}

void State::DrawShop() {
	DrawRectangle(0, 0, Settings::WIDTH, Settings::HEIGHT, (Color){255,255,255,130});

	float crossSize = Settings::WIDTHf*0.2;
	DrawRectangle(
		Settings::WIDTHf*0.5f-crossSize*0.16, 
		Settings::HEIGHT*0.75f-crossSize*0.5, 
		crossSize*0.33, 
		crossSize,
		(Color){130, 130, 130, 200}
	);
	DrawRectangle(
		Settings::WIDTHf*0.5f-crossSize*0.5, 
		Settings::HEIGHT*0.75f-crossSize*0.16, 
		crossSize, 
		crossSize*0.33,
		(Color){130, 130, 130, 200}
	);

	const char *textAdd = "Add empty slot\0";
	DrawText(
		textAdd, 
		Settings::WIDTHf*0.5f-MeasureText(textAdd, 40)*0.5f, 
		Settings::HEIGHT*0.75f+crossSize, 
		40,
		(Color){130, 130, 130, 200}
	);

	if (isUpgrading) {
		auto &ammoCollider = player.ammo.at(upgradingIndex)->collider;
		ammoCollider.x = Settings::WIDTHf*0.5;
		ammoCollider.y = Settings::HEIGHTf*0.25;
		player.ammo.at(upgradingIndex)->Draw();

		const char *textUpgrade = "Upgrade\0";
		DrawText(
			textUpgrade, 
			Settings::WIDTHf*0.5f-MeasureText(textUpgrade, 40)*0.5f, 
			Settings::HEIGHT*0.25f+ammoCollider.r, 
			40,
			(Color){130, 130, 130, 200}
		);
	} else {
		shopItem->collider.x = Settings::WIDTHf*0.5;
		shopItem->collider.y = Settings::HEIGHTf*0.25;

		shopItem->Draw();

		std::string textBuy = "Buy ammo";
		if (player.maxAmmo<=player.ammo.size()) textBuy = "Replace ammo";
		DrawText(
			textBuy.c_str(), 
			Settings::WIDTHf*0.5f-MeasureText(textBuy.c_str(), 40)*0.5f, 
			Settings::HEIGHT*0.25f+shopItem->collider.r, 
			40,
			(Color){130, 130, 130, 200}
		);
	}
}

