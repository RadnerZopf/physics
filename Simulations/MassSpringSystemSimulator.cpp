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
	case 0:break; // simple step
	case 1:break; // simple Euler
	case 2:break; // simple Midpoint 
	case 3:break; // Complex sim
	case 5:break; // Leap Frog
	default:break;
	}
}

void MassSpringSystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
	case 0:
		cout << "simple step" << endl;
		break; // simple step
	case 1:
		cout << "simple Euler" << endl;
		break; // simple Euler
	case 2:
		cout << "simple Midpoint" << endl;
		break; // simple Midpoint 
	case 3:
		cout << "Complex sim" << endl;
		break; // Complex sim
	case 5:
		cout << "Leap Frog" << endl;
		break; // Leap Frog
	default:
		cout << "Empty Test!\n";
		break;
	}
}


void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
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
		m_vPoints[s.masspoint1].force += force;
		m_vPoints[s.masspoint2].force += force;
		
	}


	TODO MARKER!!!

	//TODO:
	//integrate Position
	//integrate Velocities
	//collision detection
}



void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	const Vec3 colPoints(1.0f, 0.0f, 0.0f); 
	const Vec3 colLines(0.0f, 1.0f, 0.0f); 
	for (Masspoint m : m_vPoints)
	{
		DUC->setUpLighting(Vec3(), 0.4*Vec3(1, 1, 1), 100, 0.6*Vec3(0.97, 0.86, 1));
		DUC->drawSphere(m.pos, Vec3(1.0f, 1.0f, 1.0f)); 
	}
	for (Spring s : m_vSprings)
	{
		DUC->drawLine(m_vPoints[s.masspoint1].pos, colLines, m_vPoints[s.masspoint2].pos, colLines);
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
