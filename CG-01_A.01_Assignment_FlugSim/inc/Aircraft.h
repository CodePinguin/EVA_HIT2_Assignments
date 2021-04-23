#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <glm/gtx/euler_angles.hpp>
class Aircraft
{

private:
	glm::mat4 currentTransform;
	glm::vec4 currentPos;
	glm::mat4 GetR(int axes, float angle);
	float delta;
	float velocity;
	int changedAxis = -1;
	unsigned int last_time = 0;
	

public:
	Aircraft();
	void IncreaseAngle(int axes, double value);
	void increaseVel(double value);
	void Reset();
	glm::mat4 GetRot();
	glm::vec4 GetPos();
	void UpdatePhysics(unsigned int sim_time);
};

