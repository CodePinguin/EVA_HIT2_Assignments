#pragma once
class Aircraft
{
public:
	double pitch;
	double roll;
	double yaw;
	double velocity;

private:
	double* Aircraft::getVertices();
	void Aircraft::increaseVel();
};

