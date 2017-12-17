#include "SphereSystem.h"



SphereSystem::SphereSystem(int numSpheres, float radius, Vec3 pos, Vec3 offset, bool useGrid)
{
	float d = 2 * radius; 
	m_fDiameter = d; 
	Vec3 firstPos = pos + Vec3(radius, offset.y - radius, radius);
	Vec3 posPlusOffset = pos + offset;
	Vec3 curPos = firstPos; 
	spheres.reserve(numSpheres); 

	for (int i = 0; i < numSpheres; ++i)
	{
		//inital vel set here for testing
		spheres.push_back(Sphere(curPos, Vec3(0.0f, 0.0f, 0.0f)));

		curPos.x += d; 
		if (curPos.x + radius> posPlusOffset.x)
		{
			curPos.x = firstPos.x + 0.05f; // slight offset to cause x/z movement on SphereOnSphere col
			curPos.z += d;

			if (curPos.z + radius> posPlusOffset.z)
			{
				curPos.z = firstPos.z +0.05f; // slight offset to cause x/z movement on SphereOnSphere col
				curPos.y -= d; // just dont give me too much balls :X
			}

		}
	}


	if (useGrid) // setup grid
	{
		sizeX = static_cast<int>(ceil(offset.x / d)); 
		sizeY = static_cast<int>(ceil(offset.y / d));
		sizeZ = static_cast<int>(ceil(offset.z / d));

		m_iGridSize = sizeX * sizeY * sizeZ;
		cdGrid.reserve(m_iGridSize);
	}

}


SphereSystem::~SphereSystem()
{
}

inline void SphereSystem::sortSpheresToGrid()
{
	cdGrid.clear(); 
	cdGrid.reserve(m_iGridSize);


	int posX = 0, posY = 0, posZ = 0; 

	for (Sphere& s : spheres)
	{
		cdGrid[((int)s.pos.x / m_fDiameter) * sizeX * sizeY + ((int)s.pos.y / m_fDiameter) * sizeY + ((int)s.pos.z / m_fDiameter)].push_back(s);
	}
}
