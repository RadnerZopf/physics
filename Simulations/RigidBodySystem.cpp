#include "RigidBodySystem.h"



RigidBodySystem::RigidBodySystem(Vec3 _position, Vec3 _size, float _mass, Quat _orientation = Quat(), Vec3 _linearVelocity = Vec3(), Vec3 _angularVelocity = Vec3()) :
	position(_position), mass(_mass), orientation(_orientation), linearVelocity(_linearVelocity), angularVelocity(_angularVelocity), size(_size)
{
	//scaled quad
	points.push_back(Vec3(size.x, size.y, size.z)); 
	points.push_back(Vec3(-size.x, size.y, size.z));
	points.push_back(Vec3(size.x, -size.y, size.z));
	points.push_back(Vec3(-size.x, -size.y, size.z));

	points.push_back(Vec3(size.x, size.y, -size.z));
	points.push_back(Vec3(-size.x, size.y, -size.z));
	points.push_back(Vec3(size.x, -size.y, -size.z));
	points.push_back(Vec3(-size.x, -size.y, -size.z));


}


RigidBodySystem::~RigidBodySystem()
{
}


void RigidBodySystem::applyForce(Vec3 loc, Vec3 force)
{
	throw "TODO"; 
}
