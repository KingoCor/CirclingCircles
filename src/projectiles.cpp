#include "game.hpp"
#include "settings.hpp"

using namespace Game;

bool Projectile::TickTimer() {
	timer += 1;
	if (timer>=getCooldown()) {
		timer = 0;
		return true;
	}

	return false;
}

DefaultProjectile::DefaultProjectile(
	float x, float y, 
	float speed, Vector2 velocity, 
	float damage
) {
	this->collider.x = x;
	this->collider.y = y;
	this->speed = speed;
	this->velocity = velocity;
	this->damage = damage;
}

void DefaultProjectile::Update() {
	this->collider.x += velocity.x;
	this->collider.y += velocity.y;
}

void DefaultProjectile::Draw() {
	DrawCircle(collider.x, collider.y, collider.r, GRAY);
}

void DefaultProjectile::Upgrade() {
	this->damage+=2;
}

std::unique_ptr<Projectile> DefaultProjectile::Shoot(float rotation) {
	Vector2 v = {
		speed*cosf(rotation),
		speed*sinf(rotation)
	};

	return std::make_unique<DefaultProjectile>(Settings::WIDTHf*0.5, Settings::HEIGHTf*0.5, speed, v, damage);
}

bool DefaultProjectile::DealDamage(std::list<Enemy> &enemies) {
	bool isHit = false;

	for (auto &enemy : enemies) {
		if (collider.Collide(enemy.collider)) {
			enemy.TakeDamage(damage);
			isHit = true;
		}
	}

	return isHit;
}

FreezeProjectile::FreezeProjectile(
	float x, float y, 
	float speed, Vector2 velocity,
	float freeze
) {
	this->collider.x = x;
	this->collider.y = y;
	this->speed = speed;
	this->velocity = velocity;
	this->freeze = freeze;
}

void FreezeProjectile::Update() {
	this->collider.x += velocity.x;
	this->collider.y += velocity.y;
}

void FreezeProjectile::Draw() {
	DrawCircle(collider.x, collider.y, collider.r, BLUE);
}

void FreezeProjectile::Upgrade() {
	freeze*=1.20;
	if (freeze>Settings::MAX_FREEZE) {
		freeze = Settings::MAX_FREEZE;
	}
}

bool FreezeProjectile::IsUpgradable() {
	return freeze<Settings::MAX_FREEZE;
}

std::unique_ptr<Projectile> FreezeProjectile::Shoot(float rotation) {
	Vector2 v = {
		speed*cosf(rotation),
		speed*sinf(rotation)
	};

	return std::make_unique<FreezeProjectile>(Settings::WIDTHf*0.5, Settings::HEIGHTf*0.5, speed, v, freeze);
}

bool FreezeProjectile::DealDamage(std::list<Enemy> &enemies) {
	bool isHit = false;

	for (auto &enemy : enemies) {
		if (collider.Collide(enemy.collider)) {
			enemy.Freeze(freeze);
			isHit = true;
		}
	}

	return isHit;
}
