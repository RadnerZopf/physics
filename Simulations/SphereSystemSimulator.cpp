#include "SphereSystemSimulator.h"
#include "util\timer.h"

std::function<float(float)> SphereSystemSimulator::m_Kernels[5] = {
	[](float x) {return 1.0f; },              // Constant, m_iKernel = 0
	[](float x) {return 1.0f - x; },          // Linear, m_iKernel = 1, as given in the exercise Sheet, x = d/2r
	[](float x) {return (1.0f - x)*(1.0f - x); }, // Quadratic, m_iKernel = 2
	[](float x) {return 1.0f / (x)-1.0f; },     // Weak Electric Charge, m_iKernel = 3
	[](float x) {return 1.0f / (x*x) - 1.0f; },   // Electric Charge, m_iKernel = 4
};

// SphereSystemSimulator member functions

SphereSystemSimulator::SphereSystemSimulator() {

}


const char * SphereSystemSimulator::getTestCasesStr()
{
	return "Demo 1 - NAIVEACC, Demo 2 - GRIDACC, Demo 3 - combined";
}

void SphereSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;


	TwAddVarRW(DUC->g_pTweakBar, "Draw Scene", TW_TYPE_BOOLCPP, &m_bDrawScene, "");
	TwAddVarRW(DUC->g_pTweakBar, "Grid", TW_TYPE_BOOLCPP, &m_bDrawGrid, "");


	TwAddVarRW(DUC->g_pTweakBar, "Kernel", TwDefineEnumFromString("Kernel", "Constant, Linear, Quadratic, Weak Electric Charge, Electric Charge"), &m_iKernel, "");
	TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "min=0.00 step=0.01");
	TwAddVarRW(DUC->g_pTweakBar, "Gravity", TW_TYPE_FLOAT, &m_fGravity, "min=0.00 step=0.01");

	TwAddVarRW(DUC->g_pTweakBar, "Timer Steps", TW_TYPE_INT32, &m_iTimeMeasureSteps, "");
	TwAddVarRW(DUC->g_pTweakBar, "Start Timer", TW_TYPE_BOOLCPP, &m_bMeasureTime, "");


	switch (m_iTestCase)
	{
	case 0: // NAIVEACC
		break;
	case 1:
		break; //  GRIDACC
	case 2:
		break; //  combined

	default:break;
	}
	//TwAddVarRW(DUC->g_pTweakBar, "DrawForce and vel", TW_TYPE_BOOLCPP, &m_bDrawForce, "");
}


void SphereSystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

	this->notifyCaseChanged(m_iTestCase);

}

void SphereSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;

	if (m_pSphereSystem != nullptr)
	{
		delete m_pSphereSystem; 
		m_pSphereSystem = nullptr; 
	}
	if (m_pSphereSystemGrid != nullptr)
	{
		delete m_pSphereSystemGrid;
		m_pSphereSystemGrid = nullptr;
	}

	//values from demo scene
	m_iKernel = 1;
	m_iNumSpheres = 100;
	m_fRadius = 0.05;
	m_fForceScaling = 10.0f; 
	m_fMass = 0.1f;
	m_fDamping = 0.7f; 

	m_fGravity = 0.1f;

	//Debgu values 
	/*
	m_fGravity = 0;
	m_fDamping = 0;
	*/

	// center bb at -0.5, 0.5
	// overlaps sim bb w/ center bb
	Vec3 boxPos = Vec3(-0.5, -0.5, -0.5); 
	Vec3 boxOffset = Vec3(1, 1, 1); 

	m_boxOuterBounds = {boxPos, boxOffset};



	switch (m_iTestCase)
	{
	case 0: // NAIVEACC
		m_pSphereSystem = new SphereSystem(m_iNumSpheres, m_fRadius, boxPos, boxOffset); 
		break;
	case 1: //  GRIDACC
		m_pSphereSystemGrid = new SphereSystem(m_iNumSpheres, m_fRadius, boxPos, boxOffset, true);
		break; 
	case 2:  //  combined
		m_pSphereSystem  = new SphereSystem(m_iNumSpheres, m_fRadius, boxPos, boxOffset);
		m_pSphereSystemGrid = new SphereSystem(m_iNumSpheres, m_fRadius, boxPos, boxOffset, true);

		break;

	default:break;
	}

	m_fRadTimes2 = 2 * m_fRadius; 
	m_fRadTimes2Squared = m_fRadTimes2*m_fRadTimes2; //for ez CD

}

void SphereSystemSimulator::externalForcesCalculations(float timeElapsed)
{
	//dont think we have ext forces
}


void SphereSystemSimulator::simulateTimestep(float timeStep)
{
	if (m_bMeasureTime)
	{
		measureTime(timeStep);
		reset(); 
	}

	switch (m_iTestCase)
	{
	case 0: // NAIVEACC
		simNaiveAcc(timeStep);
		break;
	case 1: //  GRIDACC
		simGridAcc(timeStep); 
		break;
	case 2:  //  combined
		simNaiveAcc(timeStep);
		simGridAcc(timeStep);
		break;
	default:break;
	}

}

inline void SphereSystemSimulator::simNaiveAcc(float dt)
{
	float dtDiv2 = dt / 2; 


	for (int i = 0; i < m_iNumSpheres; ++i)
	{
		//cout << m_pSphereSystem->spheres[i].pos << endl;

		detectAndResolveSphereOnBoundsCollision(m_pSphereSystem, i, dtDiv2);
		for (int j = i+1; j < m_iNumSpheres; ++j)
		{
			detectAndResolveSphereOnSphereCollision(m_pSphereSystem, i, j, dtDiv2);
		}
	}

	//midpoint Integration copied from mas spring sim 
	for (int i = 0; i < m_iNumSpheres; ++i)
	{
		Sphere& s = m_pSphereSystem->spheres[i];
		s.midpointPos = s.pos;
		s.midpointVel = s.vel;


		s.pos += dtDiv2 * s.vel;
		s.vel += (Vec3(0,-m_fGravity,0) - s.vel * m_fDamping)*(dtDiv2 / m_fMass);


	}

	for (int i = 0; i < m_iNumSpheres; ++i)
	{
		detectAndResolveSphereOnBoundsCollision(m_pSphereSystem, i, dtDiv2);
		for (int j = i+1; j < m_iNumSpheres; ++j)
		{
			detectAndResolveSphereOnSphereCollision(m_pSphereSystem, i, j, dtDiv2);
		}
	}

	for (Sphere& s : m_pSphereSystem->spheres)
	{
		s.pos = s.midpointPos + dt * s.vel;
		s.vel = s.midpointVel + (Vec3(0, -m_fGravity, 0) - s.vel * m_fDamping)*(dt / m_fMass);
	}
}


inline void SphereSystemSimulator::simGridAcc(float dt)
{
	SphereSystem *ss = m_pSphereSystemGrid;
	for (int i = 0; i < m_iNumSpheres; i++)
	{
		Sphere s = ss->spheres[i].col = false;

	}
	float dtDiv2 = dt / 2;

	

	m_pSphereSystemGrid->sortSpheresToGrid(m_iNumSpheres);

	for (int x = 0; x < ss->m_iGridSize; x++)
	{
		if (ss->cdGrid[x].size() > 0)
		{
			//test for collisions in 1 grid

			vector<int> adj = prepareAdjacentSphereVector(x);

			for (int b = 0; b < ss->cdGrid[x].size(); b++)
			{
				for (int c = 0; c < adj.size(); c++)
				{
					if((ss->cdGrid[x][b] != adj[c]) && !ss->spheres[ss->cdGrid[x][b]].col) detectAndResolveSphereOnSphereCollision(ss, ss->cdGrid[x][b], adj[c], dt);
					//ss->spheres[ss->cdGrid[x][b]].col = true;
					//ss->spheres[adj[c]].col = true;
					//cout << ss->cdGrid[x][b] << ":" << adj[c] << endl;
				}
			}
		}
	}

	for (int i = 0; i < m_iNumSpheres; ++i) detectAndResolveSphereOnBoundsCollision(m_pSphereSystemGrid, i, dtDiv2);

	//midpoint Integration copied from mas spring sim 
	for (int i = 0; i < m_iNumSpheres; ++i)
	{
		Sphere& s = m_pSphereSystemGrid->spheres[i];
		s.midpointPos = s.pos;
		s.midpointVel = s.vel;


		s.pos += dtDiv2 * s.vel;
		s.vel += (Vec3(0, -m_fGravity, 0) - s.vel * m_fDamping)*(dtDiv2 / m_fMass);


	}

	for (Sphere& s : m_pSphereSystemGrid->spheres)
	{
		s.pos = s.midpointPos + dt * s.vel;
		s.vel = s.midpointVel + (Vec3(0, -m_fGravity, 0) - s.vel * m_fDamping)*(dt / m_fMass);
	}

}
inline void SphereSystemSimulator::simKDTreeAcc(float dt)
{
	//Optional
}


inline void SphereSystemSimulator::detectAndResolveSphereOnSphereCollision(SphereSystem* system, int first, int second, float dt)
{
	float dist = system->spheres[first].pos.squaredDistanceTo(system->spheres[second].pos);

	if (dist < m_fRadTimes2Squared) //collision
	{
		//response
		Vec3 response = (system->spheres[first].pos - system->spheres[second].pos) //collision Vect
			* (m_Kernels[m_iKernel](dist / m_fRadTimes2) * m_fForceScaling //force
			/ m_fMass //->acceleration 
			/ sqrtf(dist) // normalisation of collisionVect
			*dt); //-->velocity
		system->spheres[first].vel += response; 
		system->spheres[second].vel -= response; 
	}

}

vector<int> SphereSystemSimulator::prepareAdjacentSphereVector(int gridID)
{
	vector<int> aS; //list of all adjacent spheres of gridID
	SphereSystem *ss = m_pSphereSystemGrid;

	aS.insert(aS.end(), ss->cdGrid[gridID].begin(), ss->cdGrid[gridID].end());
	
	if(gridID-1 >=0) aS.insert(aS.end(), ss->cdGrid[gridID - 1].begin(), ss->cdGrid[gridID - 1].end());
	if (gridID + 1 < 1000)aS.insert(aS.end(), ss->cdGrid[gridID + 1].begin(), ss->cdGrid[gridID + 1].end());

	if (gridID - ss->sizeX >= 0)aS.insert(aS.end(), ss->cdGrid[gridID - ss->sizeX].begin(), ss->cdGrid[gridID - ss->sizeX].end());
	if (gridID + ss->sizeX < 1000)aS.insert(aS.end(), ss->cdGrid[gridID + ss->sizeX].begin(), ss->cdGrid[gridID + ss->sizeX].end());

	//aS.insert(aS.end(), ss->cdGrid[gridID - ss->sizeX - 1].begin(), ss->cdGrid[gridID - ss->sizeX - 1].end());
	//aS.insert(aS.end(), ss->cdGrid[gridID + ss->sizeX + 1].begin(), ss->cdGrid[gridID + ss->sizeX + 1].end());

	if (gridID - ss->sizeX * ss->sizeY >= 0)aS.insert(aS.end(), ss->cdGrid[gridID - ss->sizeX * ss->sizeY].begin(), ss->cdGrid[gridID - ss->sizeX * ss->sizeY].end());
	if (gridID + ss->sizeX * ss->sizeY < 1000)aS.insert(aS.end(), ss->cdGrid[gridID + ss->sizeX * ss->sizeY].begin(), ss->cdGrid[gridID + ss->sizeX * ss->sizeY].end());

	return aS;
}

inline void SphereSystemSimulator::detectAndResolveSphereOnBoundsCollision(SphereSystem* system, int first, float dt)
{
	Sphere& s = system->spheres[first]; 

	if (m_boxOuterBounds.pos.x > s.pos.x - m_fRadius)
	{
		s.vel.x =  -m_Kernels[m_iKernel](abs(m_boxOuterBounds.pos.x - s.pos.x) / m_fRadTimes2) * m_fForceScaling //force
			/ m_fMass //->acceleration 
			*dt; //-->velocity

		s.pos.x = m_boxOuterBounds.pos.x + m_fRadius;

	}
	else if(m_boxOuterBounds.pos.x + m_boxOuterBounds.offset.x < s.pos.x + m_fRadius)
	{
		s.vel.x = m_Kernels[m_iKernel](abs(m_boxOuterBounds.pos.x - s.pos.x) / m_fRadTimes2) * m_fForceScaling //force
			/ m_fMass //->acceleration 
			*dt; //-->velocity

		s.pos.x = m_boxOuterBounds.pos.x + m_boxOuterBounds.offset.x - m_fRadius;
	}

	if (m_boxOuterBounds.pos.y > s.pos.y - m_fRadius)
	{
		s.vel.y =  -m_Kernels[m_iKernel](abs(m_boxOuterBounds.pos.y - s.pos.y) / m_fRadTimes2) * m_fForceScaling //force
			/ m_fMass //->acceleration 
			*dt; //-->velocity

		//s.vel.y += m_fGravity / m_fMass *dt; //disable gravity when on ground??
		s.pos.y = m_boxOuterBounds.pos.y + m_fRadius;

	}
	else if (m_boxOuterBounds.pos.y + m_boxOuterBounds.offset.y < s.pos.y + m_fRadius)
	{
		s.vel.y = m_Kernels[m_iKernel](abs(m_boxOuterBounds.pos.y - s.pos.y) / m_fRadTimes2) * m_fForceScaling //force
			/ m_fMass //->acceleration 
			*dt; //-->velocity
		s.pos.y = m_boxOuterBounds.pos.y + m_boxOuterBounds.offset.y - m_fRadius;
	}


	if (m_boxOuterBounds.pos.z > s.pos.z - m_fRadius)
	{
		s.vel.z = -m_Kernels[m_iKernel](abs(m_boxOuterBounds.pos.z - s.pos.z) / m_fRadTimes2) * m_fForceScaling //force
			/ m_fMass //->acceleration 
			*dt; //-->velocity

		s.pos.z = m_boxOuterBounds.pos.z + m_fRadius;

	}
	else if (m_boxOuterBounds.pos.z + m_boxOuterBounds.offset.z < s.pos.z + m_fRadius)
	{
		s.vel.z = m_Kernels[m_iKernel](abs(m_boxOuterBounds.pos.z - s.pos.z) / m_fRadTimes2) * m_fForceScaling //force
			/ m_fMass //->acceleration 
			*dt; //-->velocity
		s.pos.z = m_boxOuterBounds.pos.z + m_boxOuterBounds.offset.z - m_fRadius;
	}

}

void SphereSystemSimulator::measureTime(float timeStep)
{
	MuTime timer;
	m_bMeasureTime = false; // DO NOT [RE]MOVE THIS

	


	timer.get(); 

	for (size_t i = 0; i < m_iTimeMeasureSteps; ++i)
	{
		simulateTimestep(timeStep); 
	}
	
	timer = timer.update();


	cout << "-------------------------------\n"<< 
		"TimeMeasurement for:\t" << (m_iTestCase == 0 ? "NAIVEACC" : "GRIDACC") 
		<< "\nNumber of steps: \t" << m_iTimeMeasureSteps 
		<< "\nTimeStep: \t\t" << timeStep 
		<< "\nTimePassed: \t\t" << timer.time
		<<"\n--------------------------------"<< endl; 

		
}

void SphereSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{

	if (!m_bDrawScene) return; 

	if (m_bDrawGrid) // drawGrid 
	{
		Vec3 gridCol = (1, 1, 1);

		for (float y = -0.5f; y <= 0.5f; y += m_fRadTimes2)
		{
			for (float x = -0.5f; x <= 0.5f; x += m_fRadTimes2)
			{
				DUC->beginLine();
				DUC->drawLine(Vec3(x, y, -0.5f), gridCol, Vec3(x, y, 0.5f), gridCol);
				DUC->endLine();

				DUC->beginLine();
				DUC->drawLine(Vec3(-0.5f, x, y), gridCol, Vec3(0.5f, x, y), gridCol);
				DUC->endLine();

				DUC->beginLine();
				DUC->drawLine(Vec3(y, -0.5f, x), gridCol, Vec3(y, 0.5f, x), gridCol);
				DUC->endLine();
			}
		}
		



	}

	Vec3 scaleRad = Vec3(m_fRadius, m_fRadius, m_fRadius); 


	switch (m_iTestCase)
	{
	case 0: // NAIVEACC

		for (Sphere s : m_pSphereSystem->spheres)
		{
			DUC->setUpLighting(Vec3(1.0, 0, 0), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
			DUC->drawSphere(s.pos, scaleRad); 
		}
		break;
	case 1: //  GRIDACC
		for (Sphere s : m_pSphereSystemGrid->spheres)
		{
			DUC->setUpLighting(Vec3(0, 0, 1.0), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
			DUC->drawSphere(s.pos, scaleRad);
		}		break;
	case 2:  //  combined
		for (int i = 0; i < m_iNumSpheres; ++i)
		{
			DUC->setUpLighting(Vec3(1.0, 0, 0), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
			DUC->drawSphere(m_pSphereSystem->spheres[i].pos, scaleRad);

			DUC->setUpLighting(Vec3(0, 0, 1.0), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
			DUC->drawSphere(m_pSphereSystemGrid->spheres[i].pos, scaleRad);
		}
		break;
	default:break;
	}

	//draw Bounds

	Vec3 col = Vec3(0, 0, 1); 
	Vec3 X = Vec3(1, 0, 0);
	Vec3 Y = Vec3(0, 1, 0);
	Vec3 Z = Vec3(0, 0,1);


	DUC->beginLine();
	DUC->drawLine(m_boxOuterBounds.pos, col, m_boxOuterBounds.pos + X*(m_boxOuterBounds.pos.x + m_boxOuterBounds.offset.x), col);
	DUC->endLine();

	DUC->beginLine();
	DUC->drawLine(m_boxOuterBounds.pos, col, m_boxOuterBounds.pos + Y*(m_boxOuterBounds.pos.y + m_boxOuterBounds.offset.y), col);
	DUC->endLine();

	DUC->beginLine();
	DUC->drawLine(m_boxOuterBounds.pos, col, m_boxOuterBounds.pos + Z*(m_boxOuterBounds.pos.z + m_boxOuterBounds.offset.z), col);
	DUC->endLine();



	//Debug



	DUC->beginLine();
	DUC->drawLine(Vec3(-1,- 0.59899999943532711,1), col,Vec3(1, - 0.59899999943532711,-1), col);
	DUC->endLine();
}

void SphereSystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void SphereSystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
