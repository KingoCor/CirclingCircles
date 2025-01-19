#include "game.hpp"
#include <raylib.h>

using namespace Game;

Enemy::Enemy(float hp, float speed, bool isSpecial) {
	this->collider = Collider(Settings::WIDTHf*0.5, -Settings::WIDTHf*0.05, Settings::WIDTHf*0.05);
	this->hp = hp;
	this->speed = speed;
	this->isSpecial = isSpecial;
}

void Enemy::Update() {
	float x0 = Settings::WIDTHf*0.5;
	float y0 = Settings::HEIGHTf*0.5;

	float x = collider.x-x0;
	float y = collider.y-y0;

	float r = sqrtf(x*x+y*y);
	float a = 0;

	if      (x==0 && y>=0) a = M_PI*0.5f;
	else if (x==0 && y<0)  a = M_PI*1.5f;
	else                   a = atanf(y/x);
	if (x<0)a+=M_PI;

	float currentSpeed = speed*(1-freeze);

	a-=currentSpeed;
	r-=collider.r*currentSpeed/M_PI;

	collider.x = r*cosf(a)+x0;
	collider.y = r*sinf(a)+y0;
}

void Enemy::Draw() {
	Color start = GRAY;
	Color half = RED;
	Color end = BLACK;
	Color current;

	int th1 = 100;
	int th2 = 200;

	if (isSpecial)   current = YELLOW;
	else if (hp<th1) current = ColorLerp(start, half, hp/th1);
	else if (hp<th2) current = ColorLerp(half, end, (hp-th1)/(th2-th1));
	else             current = BLACK;

	if (freeze>0.0) current = ColorLerp(current, BLUE, freeze);

	DrawCircle(collider.x, collider.y, collider.r, current);
}

void Enemy::TakeDamage(float damage) {
	this->hp-=damage;
}

void Enemy::Freeze(float freeze) {
	if (freeze>this->freeze) this->freeze = freeze;
}
