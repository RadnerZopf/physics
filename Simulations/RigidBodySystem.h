#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include "util/vectorbase.h"
#include "DrawingUtilitiesClass.h"

using std::vector; 

class RigidBodySystem
{


public:
	RigidBodySystem(Vec3 _position,Vec3 _size,float _mass, Quat _orientation = Quat(), Vec3 _linearVelocity = Vec3(), Vec3 _angularVelocity = Vec3());
	~RigidBodySystem();

	void applyForce(Vec3 loc, Vec3 force);




	Vec3 position;
	Vec3 size; 
	Quat orientation; 
	Vec3 linearVelocity;
	Vec3 angularVelocity; 
	float mass; 

private: 

	vector<Vec3> points; 

};

