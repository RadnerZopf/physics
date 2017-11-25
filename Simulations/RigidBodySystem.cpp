#include "RigidBodySystem.h"

using namespace GamePhysics;


RigidBodySystem::RigidBodySystem(Vec3 _position, Vec3 _size, float _mass, Quat _orientation, Vec3 _linearVelocity, Vec3 _angularVelocity) :
	position(_position), mass(_mass), orientation(_orientation), linearVelocity(_linearVelocity), angularVelocity(_angularVelocity), size(_size)
{
	//scaled quad // prob not gona need dis
	points.push_back(Vec3(size.x, size.y, size.z)); 
	points.push_back(Vec3(-size.x, size.y, size.z));
	points.push_back(Vec3(size.x, -size.y, size.z));
	points.push_back(Vec3(-size.x, -size.y, size.z));

	points.push_back(Vec3(size.x, size.y, -size.z));
	points.push_back(Vec3(-size.x, size.y, -size.z));
	points.push_back(Vec3(size.x, -size.y, -size.z));
	points.push_back(Vec3(-size.x, -size.y, -size.z));

	// this we need


	scaleMat.initScaling(size.x, size.y, size.z); 

}


RigidBodySystem::~RigidBodySystem()
{
}


void RigidBodySystem::applyForce(Vec3 loc, Vec3 force)
{
	throw "TODO"; 
}


Mat4& RigidBodySystem::getWorldMat()
{
	Mat4 translation; 
	translation.initTranslation(position.x, position.y, position.z); 


	worldMat = scaleMat; 
	worldMat *= orientation.getRotMat();
	worldMat *= translation; 

	return worldMat; 
}