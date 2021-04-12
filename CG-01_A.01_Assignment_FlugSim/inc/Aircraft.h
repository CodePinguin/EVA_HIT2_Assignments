#pragma once
class Aircraft
{
public:
	float PITCH;
	float ROLL;
	float YAW;
	float velocity;

private:
	double* Aircraft::getVertices();
	void Aircraft::increaseVel();
};

