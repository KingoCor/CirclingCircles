#include "game.hpp"

using namespace Game;

Collider::Collider(float x, float y, float r) {
	this->x = x;
	this->y = y;
	this->r = r;
}

bool Collider::Collide(Collider c) {
	return powf(x-c.x, 2.0)+powf(y-c.y, 2.0)<=powf(r+c.r,2.0);
}
