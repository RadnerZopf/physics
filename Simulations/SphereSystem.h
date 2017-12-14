#pragma once

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

};

