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
