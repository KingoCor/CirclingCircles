#include "game.hpp"

using namespace Game;

Vector2 RotateVector2(Vector2 v, float a) {
	return (Vector2) {
		v.x*cosf(a)-v.y*sinf(a),
		v.x*sinf(a)+v.y*cosf(a)
	};
}

void Player::Update() {
	LookAt(GetMouseX(), GetMouseY());

	for (const auto &a : ammo) {
		if (a->TickTimer()) {
			this->projectiles.push_back(a->Shoot(rotation));
		}
	}

	for (const auto &p : projectiles) {
		p->Update();
	}
}

void Player::Draw() {
	Vector2 p1, p2, p3;

	p1.x = size*sqrtf(3)/3;
	p1.y = 0;
	p1 = RotateVector2(p1, rotation);		
	p1.x += Settings::WIDTHf*0.5f;
	p1.y += Settings::HEIGHTf*0.5f;

	p2.x = -size*sqrtf(3)/6;
	p2.y = -size*0.5f;
	p2 = RotateVector2(p2, rotation);
	p2.x += Settings::WIDTHf*0.5f;
	p2.y += Settings::HEIGHTf*0.5f;

	p3.x = -size*sqrtf(3)/6;		
	p3.y = size*0.5f;
	p3 = RotateVector2(p3, rotation);
	p3.x += Settings::WIDTHf*0.5f;
	p3.y += Settings::HEIGHTf*0.5f;

	DrawTriangle(p1, p2, p3, GRAY);

	for (const auto& p : projectiles) {
		p->Draw();
	}
}

void Player::LookAt(float x, float y) {
	x-=GetScreenWidth()*0.5f;
	y-=GetScreenHeight()*0.5f;

	if      (x==0 && y>=0) this->rotation=M_PI*0.5f;
	else if (x==0 && y<0)  this->rotation=M_PI*1.5f;
	else this->rotation=atanf(y/x);
	if (x<0) this->rotation+=M_PI;
};

void Player::DealDamage(std::list<Enemy> &enemies) {
	for (auto p=projectiles.begin(); p!=projectiles.end(); ) {
		if ((*p)->DealDamage(enemies)) {
			p = this->projectiles.erase(p);
		} else {
			p++;
		}
	}
}
