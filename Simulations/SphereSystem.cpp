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

		for (int i = 0; i < 1000; i++) cdGrid.push_back(*new std::vector<int>());
	}

}


SphereSystem::~SphereSystem()
{
}

void SphereSystem::sortSpheresToGrid(int num)
{
	for (int i = 0; i < 1000; i++) cdGrid[i].clear();
	

	int posX = 0, posY = 0, posZ = 0; 
	
	for (int i = 0; i < num; i++)
	{
		Sphere s = spheres[i];

		posX = (int)((s.pos.x + 0.5f) / m_fDiameter);
		posY = (int)((s.pos.y + 0.5f) / m_fDiameter);
		posZ = (int)((s.pos.z + 0.5f) / m_fDiameter);

		if(posX * sizeX * sizeY + posY * sizeY + posZ >= 0) cdGrid[posX * sizeX * sizeY + posY * sizeY + posZ].push_back(i);
		else std::cout << "Dropped" << std::endl; 
	}
}
