#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <glm/gtx/euler_angles.hpp>
class Aircraft
{

private:
	float delta;
	glm::mat4 currentTransform;
	glm::vec4 currentPos;
	float velocity;
	int changedAxis = -1;
	glm::mat4 GetR(int axes);
public:
	void IncreaseAngle(int axes, double value);
	Aircraft();
	void increaseVel(double value);
	void Reset();
	glm::mat4 GetRot();
	glm::vec4 GetPos();
};

