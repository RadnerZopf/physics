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
