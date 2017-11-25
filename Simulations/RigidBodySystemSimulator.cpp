#include "RigidBodySystemSimulator.h"



RigidBodySystemSimulator::RigidBodySystemSimulator()
{

}

const char * RigidBodySystemSimulator::getTestCasesStr()
{
	return "Demo 1 - one step, Demo 2 - single Body, Demo 3 - two Bodies, Demo 4 - Complex sim";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

	switch (m_iTestCase)
	{
	case 0: // one step
		break;
	case 1:
		break; //  single Body
	case 2:
		break; //  two Bodies
	case 3:
		break; // Complex sim
	default:break;
	}
	//TwAddVarRW(DUC->g_pTweakBar, "DrawForce and vel", TW_TYPE_BOOLCPP, &m_bDrawForce, "");
}

void RigidBodySystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

	this->notifyCaseChanged(m_iTestCase);

}

void RigidBodySystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;

	m_externalForce = Vec3();

	m_vRigidBodies.clear(); 

	switch (m_iTestCase)
	{
	case 0: // one step
		addRigidBody(Vec3(), Vec3(1.0f, 0.6f, 0.5f), 2); 
		break;
	case 1:
		break; //  single Body
	case 2:
		break; //  two Bodies
	case 3:
		break; // Complex sim
	default:break;
	}
}




void RigidBodySystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void RigidBodySystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
