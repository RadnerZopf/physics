#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include "util/vectorbase.h"
#include "util/matrixbase.h"
#include "DrawingUtilitiesClass.h"

using std::vector; 

class RigidBodySystem
{


public:
	 RigidBodySystem(Vec3 _position,Vec3 _size,float _mass, Quat _orientation = Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3 _linearVelocity = Vec3(), Vec3 _angularVelocity = Vec3());
	~RigidBodySystem();

	void applyForce(Vec3 loc, Vec3 force);

	Mat4& getWorldMat(); 


	Vec3 position;
	Vec3 size; 
	Quat orientation; 
	Vec3 linearVelocity;
	Vec3 angularVelocity; 
	float mass; 

private: 
	GamePhysics::Mat4 worldMat; 
	GamePhysics::Mat4 scaleMat;
	vector<Vec3> points; 

};

