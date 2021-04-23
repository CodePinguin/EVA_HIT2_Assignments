#include "../inc/Aircraft.h"

Aircraft::Aircraft() {
    Reset();
}


void Aircraft::increaseVel(double value)
{
	velocity += value;
}

void Aircraft::Reset() {
	velocity = 0;
	
	currentTransform = glm::mat4(1.0f);
	currentPos = glm::vec4(0, 0.0f, 0, 0);
}

glm::mat4 Aircraft::GetRot() {

    // setup modelview matrix

	if (changedAxis >= 0) {
		currentTransform = currentTransform * GetR(changedAxis, delta);
		changedAxis = -1;
	}


    return currentTransform;
}

glm::vec4 Aircraft::GetPos() {
	return currentPos;
}

glm::mat4 Aircraft::GetR(int axes, float angle) {


	double c1 = cos(angle), s1 = sin(angle);

	switch (axes) {
	case 0:
		//x
		glm::mat4 Rx(1.0f);
		Rx[1][1] = c1;
		Rx[2][1] = -s1;
		Rx[2][2] = c1;
		Rx[1][2] = s1;
		return Rx;
	case 1:
		//y
		glm::mat4 Ry(1.0f);
		Ry[0][0] = c1;
		Ry[2][0] = s1;
		Ry[0][2] = -s1;
		Ry[2][2] = c1;
		return Ry;
	case 2:
		//z
		glm::mat4 Rz(1.0f);
		Rz[0][0] = c1;
		Rz[1][0] = -s1;
		Rz[0][1] = s1;
		Rz[1][1] = c1;
		return Rz;
	}
}

void Aircraft::IncreaseAngle(int axes, double value) {
	changedAxis = axes;
	delta = value;
}

void Aircraft::UpdatePhysics(unsigned int sim_time) {
	unsigned int delta_t = sim_time - last_time;
	float delta_t_sec = delta_t / 1000.0;

	//Update position
	glm::vec4 translate(velocity * delta_t_sec, 0, 0, 1);
	translate = currentTransform * translate;
	currentPos += translate;

	last_time = sim_time;
}