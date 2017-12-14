#pragma once

<<<<<<< HEAD
#include <vector>
#include "util\vectorbase.h"

using namespace GamePhysics; 

struct Sphere
{
	Sphere(Vec3 _pos, Vec3 _vel = Vec3(0,0,0)) : pos(_pos), vel(_vel) {}

	Vec3 pos; 
	Vec3 vel; 
	Vec3 force = Vec3(); 

	Vec3 midpointPos, midpointVel; 

} ;

class SphereSystem
{
public:
	SphereSystem(int numSpheres, float radius, Vec3 firstPos, float x_max, float z_max);
	~SphereSystem();

	std::vector<Sphere> spheres;
=======
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


	void saveState();
	void loadOldState();

	Mat4 getInertiaTensor(); 


	Mat4& getWorldMat(); 


	Vec3 position;
	Vec3 size; 
	Quat orientation; 
	Vec3 linearVelocity;
	Vec3 angularVelocity; 
	Vec3 angularMomentum = Vec3();

	Vec3 old_position;
	Vec3 old_size;
	Quat old_orientation;
	Vec3 old_linearVelocity;
	Vec3 old_angularVelocity;
	Vec3 old_angularMomentum = Vec3();




	Vec3 force = Vec3(), torque = Vec3(); 
	float mass; 
	float bouncieness; 
	bool fixed = false;

private: 

	Mat4 inertiaTensor;

	GamePhysics::Mat4 worldMat; 
	GamePhysics::Mat4 scaleMat;
	vector<Vec3> points;
>>>>>>> cd08e57063624ad0a5fa43cd9ec88b9f5a0c3097

};

