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
		TwAddVarRW(DUC->g_pTweakBar, "Method", TwDefineEnumFromString("Method", "Euler, Midpoint, Leapfrog"), &m_iIntegrator, "");
		break;
	case 1:
		break; // simple Euler
	case 2:
		break; // simple Midpoint 
	case 3:
		TwAddVarRW(DUC->g_pTweakBar, "Method", TwDefineEnumFromString("Method", "Euler, Midpoint, Leapfrog"), &m_iIntegrator, "");
		break; // Complex sim
	case 4:break; // Leap Frog
	default:break;
	}
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
	case 0:
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
			cout << "Point " << i <<":\n\tPosition:\t" << m_vPoints[i].pos << "\n\tVelocity:\t" << m_vPoints[i].velocity << endl; 
		}

		break; // simple step
	case 1:
		cout << "simple Euler" << endl;

		m_iIntegrator = EULER;
		m_fFixedTimestep = 0.005;


		this->setMass(10.0);
		this->setStiffness(40.0);
		this->setDampingFactor(0.0);

		this->addMassPoint(Vec3(), Vec3(-1.0, 0.0, 0.0), false);
		this->addMassPoint(Vec3(0.0, 2.0, 0.0), Vec3(1.0, 0.0, 0.0), false);

		this->addSpring(0, 1, 1.0);


		break; // simple Euler
	case 2:
		cout << "simple Midpoint" << endl;

		m_iIntegrator = MIDPOINT;
		m_fFixedTimestep = 0.005;

		this->setMass(10.0);
		this->setStiffness(40.0);
		this->setDampingFactor(0.0);

		this->addMassPoint(Vec3(), Vec3(-1.0, 0.0, 0.0), false);
		this->addMassPoint(Vec3(0.0, 2.0, 0.0), Vec3(1.0, 0.0, 0.0), false);

		this->addSpring(0, 1, 1.0);

		break; // simple Midpoint 
	case 3:
		cout << "Complex sim" << endl;
		break; // Complex sim
	case 5:
		cout << "Leap Frog" << endl;

		m_iIntegrator = LEAPFROG; 

		break; // Leap Frog
	default:
		cout << "Empty Test!\n";
		break;
	}
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}


void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	if (m_fFixedTimestep > 0) timeStep = m_fFixedTimestep; // fixed timestep for demo 1-3

	Vec3 clearforce(0.0f, m_iTestCase > 2 ? EARTH_ACCEL : 0.0f, 0.0f);
	clearforce += m_externalForce;

	for(Masspoint point : m_vPoints) // clear forces
	{
		point.force = clearforce; 
	}

	for (Spring s : m_vSprings)
	{
		s.currentLength = sqrt(m_vPoints[s.masspoint1].pos.squaredDistanceTo(m_vPoints[s.masspoint2].pos));
		Vec3 force = s.stiffness * (s.currentLength - s.initialLength) *  ((m_vPoints[s.masspoint1].pos - m_vPoints[s.masspoint2].pos) / s.currentLength);
		m_vPoints[s.masspoint1].force -= force;
		m_vPoints[s.masspoint2].force -= force;
		
	}


	for (int i = 0; i< m_vPoints.size(); ++i)
	{
		(this->*m_fpIntegrators[m_iIntegrator])(timeStep, i); 

	}

	//TODO MARKER!!!

	//TODO:
	//collision detection
}



void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	const Vec3 colPoints(1.0f, 0.0f, 0.0f); 
	const Vec3 colLines(0.0f, 1.0f, 0.0f); 
	for (Masspoint m : m_vPoints)
	{
		DUC->setUpLighting(colPoints, colPoints, 100, colPoints);
		DUC->drawSphere(m.pos, Vec3(0.01f, 0.01f, 0.01f)); 
	}
	for (Spring s : m_vSprings)
	{
		DUC->beginLine();
		DUC->drawLine(m_vPoints[s.masspoint1].pos, colLines, m_vPoints[s.masspoint2].pos, colLines);
		DUC->endLine();
	}

}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 velocity, bool isFixed, float mass) // mass == -1 -> use m_fMass
{
	m_vPoints.push_back(Masspoint(position, velocity, isFixed, mass == -1 ? m_fMass : mass));
	return m_vPoints.size() - 1; 
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength, float stiffness) // stiffness == -1 -> use m_fStiffness
{
	m_vSprings.push_back(Spring(masspoint1, masspoint2, initialLength, stiffness == -1 ? m_fStiffness : stiffness));
}

void MassSpringSystemSimulator::integrateEuler(const float& deltaTime, const int& point)
{
	Masspoint& p = m_vPoints[point]; 

	p.pos += deltaTime * p.velocity;

	p.velocity += (p.force - p.velocity * m_fDamping) / p.mass * deltaTime;

}

void MassSpringSystemSimulator::integrateMidpoint(const float& deltaTime, const int& point)
{
	throw "Not Implemented";
}

void MassSpringSystemSimulator::integrateLeapFrog(const float& deltaTime, const int& point)
{
	throw "Not Implemented";
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
