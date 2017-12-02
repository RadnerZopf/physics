#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include "util/vectorbase.h"
#include "util/matrixbase.h"
#include "DrawingUtilitiesClass.h"
#include "collisionDetect.h"

using std::vector; 

class RigidBodySystem
{


public:
	 RigidBodySystem(Vec3 _position,Vec3 _size,float _mass, Quat _orientation, Vec3 _linearVelocity, Vec3 _angularVelocity, float _bounciness);
	~RigidBodySystem();

	void applyForce(Vec3 _loc, Vec3 _force);

	Mat4 getInertiaTensor(); 


	Mat4& getWorldMat(); 


	Vec3 position;
	Vec3 size; 
	Quat orientation; 
	Vec3 linearVelocity;
	Vec3 angularVelocity; 
	Vec3 angularMomentum = Vec3();


	Vec3 force = Vec3(), torque = Vec3(); 
	float mass; 
	float bouncieness; 

private: 

	Mat4 inertiaTensor;

	GamePhysics::Mat4 worldMat; 
	GamePhysics::Mat4 scaleMat;
	vector<Vec3> points; 

};

