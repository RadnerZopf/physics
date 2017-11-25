#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{

}

const char * MassSpringSystemSimulator::getTestCasesStr()
{
	return "Demo 1 - simple step, Demo 2 - simple Euler, Demo 3 - simple Midpoint, Demo 4 - Complex sim, Demo 5 - Leap Frog";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

	switch (m_iTestCase)
	{
	case 0: // simple step
		TwAddVarRW(DUC->g_pTweakBar, "Method", TwDefineEnumFromString("Method", "Euler, Leapfrog, Midpoint"), &m_iIntegrator, "");
		break;
	case 1:
		break; // simple Euler
	case 2:
		break; // simple Midpoint 
	case 3:
		TwAddVarRW(DUC->g_pTweakBar, "Method", TwDefineEnumFromString("Method", "Euler, Leapfrog, Midpoint"), &m_iIntegrator, "");
		break; // Complex sim
	case 4:break; // Leap Frog
	default:break;
	}
	TwAddVarRW(DUC->g_pTweakBar, "DrawForce and vel", TW_TYPE_BOOLCPP, &m_bDrawForce, "");

}

void MassSpringSystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

	this->notifyCaseChanged(m_iTestCase); 

}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	m_fFixedTimestep = -1;

	m_externalForce = Vec3(); 

	m_vPoints.clear(); 
	m_vSprings.clear(); 

	switch (m_iTestCase)
	{
	case 0:// single step
		cout << "simple step - integrator : " + m_iIntegrator << endl;

		this->setMass(10.0);
		this->setStiffness(40.0);
		this->setDampingFactor(0.0);

		this->addMassPoint(Vec3(), Vec3(-1.0, 0.0, 0.0), false);
		this->addMassPoint(Vec3(0.0, 2.0, 0.0), Vec3(1.0, 0.0, 0.0), false);

		this->addSpring(0, 1, 1.0);


		this->simulateTimestep(0.1);

		for (int i = 0; i < m_vPoints.size(); ++i)
		{
			cout << "Point " << i << ":\n\tPosition:\t" << m_vPoints[i].pos << "\n\tVelocity:\t" << m_vPoints[i].velocity << endl;
		}

		break;
	case 1:// simple Euler
		cout << "simple Euler" << endl;

		m_iIntegrator = EULER;
		m_fFixedTimestep = 0.005;


		this->setMass(10.0);
		this->setStiffness(40.0);
		this->setDampingFactor(0.0);

		this->addMassPoint(Vec3(), Vec3(-1.0, 0.0, 0.0), false);
		this->addMassPoint(Vec3(0.0, 2.0, 0.0), Vec3(1.0, 0.0, 0.0), false);

		this->addSpring(0, 1, 1.0);


		break;
	case 2:// simple Midpoint 
		cout << "simple Midpoint" << endl;

		m_iIntegrator = MIDPOINT;
		m_fFixedTimestep = 0.005;

		this->setMass(10.0);
		this->setStiffness(40.0);
		this->setDampingFactor(0.0);

		this->addMassPoint(Vec3(), Vec3(-1.0, 0.0, 0.0), false);
		this->addMassPoint(Vec3(0.0, 2.0, 0.0), Vec3(1.0, 0.0, 0.0), false);

		this->addSpring(0, 1, 1.0);

		break;
	case 3:// Complex sim
	{
		cout << "Complex sim" << endl;


		this->m_iIntegrator = MIDPOINT; 
		this->m_fDamping = 0.05;

		int pointOffset = getNumberOfMassPoints();
		int springOffset = getNumberOfSprings();

		float startpos = -0.7f; 
		float startY = 0.4f; 
		int pointcount = 15;
		float totalMass = 5;
		float springLength = 0.1;
		float diagLength = sqrt(2 * springLength* springLength);
		float stiffness = 50;



		GameObject weirdClothyThingamabob;
		weirdClothyThingamabob.id = 1;
		weirdClothyThingamabob.pointOffset = pointOffset; 
		weirdClothyThingamabob.springOffset = springOffset; 

		for (int i = 0; i < pointcount; ++i)
		{
			addMassPoint(Vec3(startpos, startY, startpos + i*springLength), Vec3(), false, totalMass / (pointcount * pointcount), 1);
			weirdClothyThingamabob.pointCount++; // YES, I AM THAT LAZY 
		}

		for (int i = 1; i < pointcount; ++i) // make a pointcoountXpointcoount MP grid for some cloth like stuff
		{
			addMassPoint(Vec3(startpos + i*springLength, startY, startpos), Vec3(), false, totalMass / (pointcount * pointcount), 1);
			weirdClothyThingamabob.pointCount++; // YES, I AM THAT LAZY 


			for (int j = 1; j < pointcount; ++j)
			{
				
				addMassPoint(Vec3(startpos + i*springLength, startY, startpos + j*springLength), Vec3(), false, totalMass / (pointcount * pointcount), 1);
				weirdClothyThingamabob.pointCount++; // YES, I AM THAT LAZY 

				int q0 = pointOffset + (i - 1) * pointcount + j - 1;
				int q1 = q0 + 1;
				int q2 = q0 + pointcount;
				int q3 = q2 + 1;
				
				addSpring(q0, q1, springLength, stiffness);
				addSpring(q0, q2, springLength, stiffness);
				addSpring(q0, q3, diagLength, stiffness);
				addSpring(q1, q2, diagLength, stiffness);

				weirdClothyThingamabob.springCount += 4; // YES, I AM THAT LAZY

			}


			int e0 = pointOffset + (i) * pointcount - 1;
			int e1 = e0 + pointcount;

			addSpring(e0, e1, springLength, stiffness);
			weirdClothyThingamabob.springCount++; // YES, I AM THAT LAZY

		}

		for (int j = 0; j < pointcount-1; ++j)
		{
			int q0 = pointOffset + (pointcount-1) * pointcount + j;
			int q1 = q0 + 1;
			addSpring(q0, q1, springLength, stiffness);
			weirdClothyThingamabob.springCount++; // YES, I AM THAT LAZY

		}

		m_vPoints[pointOffset].fixed = true;
		m_vPoints[pointOffset + pointcount - 1].fixed = true;
		m_vPoints[pointOffset + pointcount * (pointcount-1)].fixed = true;
		m_vPoints[pointOffset + pointcount * pointcount - 1].fixed = true;

		//setup ball
		Vec3 ballPos = Vec3(0, 0, 0);
		float ballRadius = 0.2f;
		float ballMass = 5.0f;

		GameObject rndBall;
		rndBall.id = 2;
		rndBall.pointOffset = getNumberOfMassPoints();

		rndBall.pointCount = 1;
		int tmp = addMassPoint(ballPos, Vec3(0, 0, 0), false, ballMass, 2);

		rndBall.sphereOffset = addSphere(tmp , ballRadius);

		

		break;
	}
	case 5:// Leap Frog
		cout << "Leap Frog" << endl;

		m_iIntegrator = LEAPFROG; 

		break; 
	default:
		cout << "Empty Test!\n";
		break;
	}
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
	//calculate and apply mouse force to all m_vPoints
	Point2D mouseDiff;
	mouseDiff.x = m_trackmouse.x - m_oldtrackmouse.x;
	mouseDiff.y = m_trackmouse.y - m_oldtrackmouse.y;

	if (mouseDiff.x != 0 || mouseDiff.y != 0)
	{
		Mat4 worldViewInv = Mat4(DUC->g_camera.GetWorldMatrix() * DUC->g_camera.GetViewMatrix());
		worldViewInv = worldViewInv.inverse();
		Vec3 inputView = Vec3((float)mouseDiff.x, (float)-mouseDiff.y, 0);
		Vec3 inputWorld = worldViewInv.transformVectorNormal(inputView);
		// find a proper scale!
		float inputScale = 0.005f;
		inputWorld = inputWorld * inputScale;

		/*
		for (int i = 0; i < m_vPoints.size(); ++i)
		{
			m_vPoints[i].velocity += inputWorld;
		}
		*/
		m_externalForce = inputWorld; 
	}
}

void MassSpringSystemSimulator::computeElasticForces()
{
	for (Spring s : m_vSprings)
	{
		s.currentLength = sqrt(m_vPoints[s.masspoint1].pos.squaredDistanceTo(m_vPoints[s.masspoint2].pos));
		Vec3 force = s.stiffness * (s.currentLength - s.initialLength) *  ((m_vPoints[s.masspoint1].pos - m_vPoints[s.masspoint2].pos) / s.currentLength);
		m_vPoints[s.masspoint1].force -= force;
		m_vPoints[s.masspoint2].force += force;
	}

//#define ISUCK
#ifdef ISUCK
	Vec3 clearforce(0.0f, m_iTestCase > 2 ? EARTH_ACCEL : 0.0f, 0.0f);
	clearforce += m_externalForce;

	Vec3 internalForce; 
	for (Masspoint p : m_vPoints)
		internalForce += p.force - clearforce; 

	cout << "internal force after computeElasticForces (should be 0) : " << internalForce << endl; 

#endif //  Deb


}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	if (m_fFixedTimestep > 0) timeStep = m_fFixedTimestep; // fixed timestep for demo 1-3
	

	Vec3 clearforce(0.0f, 0.0f, 0.0f);
	clearforce += m_externalForce;

	m_externalForce = Vec3(); 

	for(Masspoint point : m_vPoints) // clear forces
	{
		point.force = clearforce; 
	}

	computeElasticForces(); 

	switch (m_iIntegrator)
	{
	case EULER:
		for (int i = 0; i< m_vPoints.size(); ++i)
		{
			Masspoint& p = m_vPoints[i];
			if (p.fixed) continue; 

			

			p.pos += timeStep * p.velocity;
			
			//p.force = internal + external 
			p.velocity += (p.force - p.velocity * m_fDamping)*(timeStep / p.mass);
			if (m_iTestCase == 3) // compex sim -> apply gravity
				p.velocity += Vec3(0.0f, EARTH_ACCEL, 0.0f) * timeStep;

		}
		break;
	case MIDPOINT:
	{
		float timeDiv2 = timeStep / 2.0;

		//midpoint calc, save original pos + vel in p.tmp...
		for (int i = 0; i < m_vPoints.size(); ++i)
		{
			Masspoint& p = m_vPoints[i];
			if (p.fixed) continue;

			p.tmpPos = p.pos;
			p.tmpVel = p.velocity;


			p.pos += timeDiv2 * p.velocity;
			//p.force = internal + external force
			p.velocity += (p.force - p.velocity * m_fDamping)*(timeDiv2 / p.mass);

			if (m_iTestCase == 3) // compex sim -> apply gravity
				p.velocity += Vec3(0.0f, EARTH_ACCEL, 0.0f) * timeDiv2;

			p.force = clearforce;

		}

		//recalc elastic forces w/ midpoint values

		computeElasticForces();

		//final step
		for (int i = 0; i < m_vPoints.size(); ++i)
		{
			Masspoint& p = m_vPoints[i];
			if (p.fixed) continue;
			

			p.pos = p.tmpPos + timeStep * p.velocity;
			//p.force = internal + external force
			p.velocity = p.tmpVel + (p.force - p.velocity * m_fDamping)*(timeStep / p.mass);
			if (m_iTestCase == 3) // compex sim -> apply gravity
			{
				p.velocity += Vec3(0.0f, EARTH_ACCEL, 0.0f) * timeStep;

				//check if point collides with sphere in complex scene
				if (checkPointAgainstSphere(m_vPoints[i], m_vSpheres[0]))
				{
					p.pos = p.tmpPos;
					//p.velocity = p.tmpVel;
				}
			}
		}


		break; 
	}		
	case LEAPFROG: 
		break; 
	}


	//collision detection for complex sim
	if (m_iTestCase == 3)
	{
		for (Masspoint& p : m_vPoints)
		{
			if (p.pos.y < FLOOR_Y) // Floor //<- useful comment
				p.pos.y = FLOOR_Y; 
		}

		//check if spheres are out of bounds
		for (Sphere s : m_vSpheres)
		{
			if ((m_vPoints[s.masspoint].pos.y - s.radius) < FLOOR_Y) m_vPoints[s.masspoint].pos.y = FLOOR_Y + s.radius;
		}

		
	}


	//TODO MARKER!!!

	//TODO:
	//collision detection
	//user interaction
}

bool MassSpringSystemSimulator::checkPointAgainstSphere(Masspoint mp, Sphere s)
{
	double dist = sqrt(pow(mp.pos.x - m_vPoints[s.masspoint].pos.x, 2.0) + pow(mp.pos.y - m_vPoints[s.masspoint].pos.y, 2.0) + pow(mp.pos.z - m_vPoints[s.masspoint].pos.z, 2.0));

	if (dist < s.radius && dist !=0)
	{
		cout << dist << endl;
		//cout << m_vPoints[s.masspoint].pos << endl;
		return true;

	}

	return false;
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	const Vec3 colPoints(1.0f, 0.0f, 0.0f); 
	const Vec3 colFixPoints(0.0f, 0.0f, 1.0f);

	const Vec3 colSpheres(1.0f, 1.0f, 0.5f);

	const Vec3 colLines(0.0f, 1.0f, 0.0f); 
	const Vec3 colForce(1.0f, 105.0/255.0, 180/255.0);

	for (Masspoint m : m_vPoints)
	{
		if(m.fixed)
			DUC->setUpLighting(colFixPoints, colFixPoints, 100, colFixPoints);
		else
			DUC->setUpLighting(colPoints, colPoints, 100, colPoints);

		DUC->drawSphere(m.pos, Vec3(0.01f, 0.01f, 0.01f)); 


		if (m_bDrawForce)
		{
			DUC->beginLine();
			DUC->drawLine(m.pos, colForce, m.pos+m.force, colForce);
			DUC->endLine();
			DUC->beginLine();
			DUC->drawLine(m.pos, colForce, m.pos + m.velocity, colFixPoints);
			DUC->endLine();
		}

	}
	for (Spring s : m_vSprings)
	{
		DUC->beginLine();
		DUC->drawLine(m_vPoints[s.masspoint1].pos, colLines, m_vPoints[s.masspoint2].pos, colLines);
		DUC->endLine();
	}

	for (Sphere s : m_vSpheres)
	{
		DUC->setUpLighting(colSpheres, colSpheres, 100, colSpheres);

		DUC->drawSphere(m_vPoints[s.masspoint].pos, Vec3(s.radius, s.radius, s.radius));
	}

}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 velocity, bool isFixed, float mass, int gameObject) // mass == -1 -> use m_fMass
{
	m_vPoints.push_back(Masspoint(position, velocity, isFixed, mass == -1 ? m_fMass : mass, gameObject));
	return m_vPoints.size() - 1; 
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength, float stiffness) // stiffness == -1 -> use m_fStiffness
{
	m_vSprings.push_back(Spring(masspoint1, masspoint2, initialLength, stiffness == -1 ? m_fStiffness : stiffness));
}

int MassSpringSystemSimulator::addSphere(int masspoint, float radius) // mass == -1 -> use m_fMass
{
	m_vSpheres.push_back(Sphere(masspoint, radius));
	return m_vPoints.size() - 1;
}


void MassSpringSystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void MassSpringSystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
