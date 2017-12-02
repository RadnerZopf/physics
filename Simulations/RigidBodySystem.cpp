#include "RigidBodySystem.h"

using namespace GamePhysics;


RigidBodySystem::RigidBodySystem(Vec3 _position, Vec3 _size, float _mass, Quat _orientation, Vec3 _linearVelocity, Vec3 _angularVelocity, float _bounciness) :
	position(_position), mass(_mass), orientation(_orientation), linearVelocity(_linearVelocity), angularVelocity(_angularVelocity), size(_size), bouncieness(_bounciness)
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


	double tensorarr[16] = { size.x * size.x,	-size.x*size.y,		-size.x*size.z,		0,
							-size.x * size.y,	size.y * size.y,	-size.y * size.z,	0,
							-size.x * size.z,	-size.y * size.z,	size.z * size.Z,	0,
								0	,				0		,			0		,		1 };
	inertiaTensor.initFromArray(tensorarr);
	inertiaTensor = inertiaTensor.inverse();

}


Mat4 RigidBodySystem::getInertiaTensor()
{
	Mat4 rot = orientation.getRotMat(); 

	Mat4 rotT = rot; 
	rotT.transpose(); 


	return rot * inertiaTensor * rotT; 
}


RigidBodySystem::~RigidBodySystem()
{
}


void RigidBodySystem::applyForce(Vec3 _loc, Vec3 _force)
{
	force += _force; 

	XMVECTOR xmloc = _loc.toDirectXVector(); 
	XMVECTOR xmforce = _force.toDirectXVector(); 
	XMVECTOR xmtorque = XMVector3Cross(xmloc, xmforce); 
	torque += Vec3(xmtorque); 

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