#pragma once

#include <vector>
#include <math.h>
#include <stdio.h>
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
	SphereSystem(int numSpheres, float radius, Vec3 pos, Vec3 offset, bool useGrid = false);
	~SphereSystem();

	void sortSpheresToGrid();

	std::vector<Sphere> spheres;
	std::vector<std::vector<Sphere>> cdGrid;

private: 
	int m_iGridSize; 
	Real m_fDiameter; 
	int sizeX; 
	int sizeY;
	int sizeZ;

};

