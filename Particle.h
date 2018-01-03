#pragma once

#include "ofMain.h"

class Particle {
public:
	Particle();
	Particle(ofVec3f location, ofColor body_color);
	~Particle();

	void apply_force(ofVec3f force);
	void seek(ofVec3f target);
	void seek_r(ofVec3f target);

	void update();
	void draw();

	ofVec3f get_location();
	ofVec3f get_start_location();

private:
	float size;
	ofVec3f location;
	ofVec3f start_location;
	ofVec3f velocity;
	ofVec3f acceleration;
	float max_force;
	float max_speed;
	float radius;
	ofColor body_color;
};