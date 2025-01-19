#pragma once

#include <cmath>
#include <list>
#include <vector>
#include <memory>
#include <string>

#include "raylib.h"

#include "settings.hpp"

namespace Game {
	class Collider {
		public:
			float x, y;
			float r;

			Collider(float x=0, float y=0, float r=1);

			bool Collide(Collider collider);
	};
	
	class Enemy {
		private:
			float speed;
			float hp;
			float freeze = 0;
			bool isSpecial;

		public:
			Collider collider;

		public:
			Enemy(float hp, float speed, bool isSpecial);

			void Update();
			void Draw();
			
			bool IsSpecial() { return isSpecial; }
			float getHp() { return hp; }
			void TakeDamage(float damage);
			void Freeze(float freeze);
	};


	class Projectile {
		protected:
			float speed = 0;
			Vector2 velocity = (Vector2){0,0};
			float damage = 0;
			float timer = 0;
			bool isShot = false;

		public:
			Collider collider = Collider(0,0,Settings::WIDTHf*0.02);

		public:
			virtual void Update() {}
			virtual void Draw() {}

			virtual float getCooldown() { return 0; }
			bool TickTimer();
			virtual void Upgrade() {}
			virtual std::unique_ptr<Projectile> Shoot(float rotation) { return std::make_unique<Projectile>(); }
			virtual bool DealDamage(std::list<Enemy> &enemies) { return false; }
	};

	class DefaultProjectile : public Projectile {
		public:
			DefaultProjectile(
				float x, float y, 
				float speed, Vector2 velocity, 
				float damage
			);

			void Update() override;
			void Draw() override;

			float getCooldown() override { return Settings::DEFAULT_PROJECTILES_COOLDOWN; }
			void Upgrade() override;
			std::unique_ptr<Projectile> Shoot(float rotation) override;
			bool DealDamage(std::list<Enemy> &enemies) override;
	};

	class FreezeProjectile : public Projectile {
		protected:
			float freeze;

		public:
			FreezeProjectile(
				float x, float y, 
				float speed, Vector2 velocity,
				float freeze
			);

			void Update() override;
			void Draw() override;

			float getCooldown() override { return Settings::FREEZE_PROJECTILES_COOLDOWN; }
			void Upgrade() override;
			std::unique_ptr<Projectile> Shoot(float rotation) override;
			bool DealDamage(std::list<Enemy> &enemies) override;
	};

	class Player {
		private:
			std::list<std::unique_ptr<Projectile>> projectiles;
			
			float size = Settings::WIDTHf*0.1;

		public:
			Collider collider = Collider(Settings::WIDTHf*0.5,Settings::HEIGHT*0.5,size*0.5);
			float rotation = 0;

			int maxAmmo = 1;
			std::vector<std::unique_ptr<Projectile>> ammo;

		public:
			Player() {
				this->ammo.push_back(std::make_unique<DefaultProjectile>(0, 0, 5, (Vector2){0,0}, 1));
			}

			void Update();
			void Draw();

			void LookAt(float x, float y);

			void DealDamage(std::list<Enemy> &enemies);
	};

	enum Screen {
		MAIN_MENU,
		GAME
	};

	class State {
		private:
			RenderTexture2D frame;

			Screen screen = MAIN_MENU;
			float transition = -1;

			int score = 0;
			Game::Player player;
			std::list<Game::Enemy> enemies;
			int needSpecial = 0;

			bool isShoping = false;
			bool isUpgrading = false;
			int upgradingIndex = -1;
			std::unique_ptr<Game::Projectile> shopItem;

		private:
			void UpdateMainMenu();
			void DrawMainMenu();

			void UpdateGame();
			void DrawGame();

			void UpdateEnemies();

			void OpenShop();
			void UpdateShop();
			void DrawShop();

		public:
			State();
			~State();

			void Reset();
			
			void Update();
			void Draw();
	};
}

