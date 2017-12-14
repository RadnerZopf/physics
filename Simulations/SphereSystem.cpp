<<<<<<< HEAD
#include "SphereSystem.h"



SphereSystem::SphereSystem(int numSpheres, float radius, Vec3 firstPos, float x_max, float z_max)
{
	float d = 2 * radius; 

	Vec3 curPos = firstPos; 
	spheres.reserve(numSpheres); 

	for (int i = 0; i < numSpheres; ++i)
	{
		spheres.push_back(Sphere(curPos, Vec3(0.0f, 0.0f, 0.0f)));

		curPos.x += d; 
		if (curPos.x > x_max)
		{
			curPos.x = firstPos.x; 
			curPos.z += d;

			if (curPos.z > z_max)
			{
				curPos.z = firstPos.z;
				curPos.y -= d; // just dont give me too much balls :X
			}

		}
	}
}


SphereSystem::~SphereSystem()
{
}
=======
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

	Mat4 rotT(rot); 
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

void RigidBodySystem::saveState()
{
	this->old_position = position;
	//this->old_linearVelocity = linearVelocity;
	this->old_orientation = orientation;
	//this->old_angularMomentum = angularMomentum;
	//this->old_angularVelocity = angularVelocity;
}

void RigidBodySystem::loadOldState()
{
	//this->angularMomentum = old_angularMomentum;
	//this->angularVelocity = old_angularVelocity;
	//this->linearVelocity = old_linearVelocity;
	this->position = old_position;
	this->orientation = old_orientation;
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
>>>>>>> cd08e57063624ad0a5fa43cd9ec88b9f5a0c3097
