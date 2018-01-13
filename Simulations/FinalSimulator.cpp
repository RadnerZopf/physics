#include "FinalSimulator.h"



FinalSimulator::FinalSimulator()
{
}


FinalSimulator::~FinalSimulator()
{
}


const char * FinalSimulator::getTestCasesStr()
{
	return "Demo 1";
}

void FinalSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

	/*
	TwAddVarRW(DUC->g_pTweakBar, "Draw Scene", TW_TYPE_BOOLCPP, &m_bDrawScene, "");
	TwAddVarRW(DUC->g_pTweakBar, "Grid", TW_TYPE_BOOLCPP, &m_bDrawGrid, "");


	TwAddVarRW(DUC->g_pTweakBar, "Kernel", TwDefineEnumFromString("Kernel", "Constant, Linear, Quadratic, Weak Electric Charge, Electric Charge"), &m_iKernel, "");
	TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "min=0.00 step=0.01");
	TwAddVarRW(DUC->g_pTweakBar, "Gravity", TW_TYPE_FLOAT, &m_fGravity, "min=0.00 step=0.01");

	TwAddVarRW(DUC->g_pTweakBar, "Timer Steps", TW_TYPE_INT32, &m_iTimeMeasureSteps, "");
	TwAddVarRW(DUC->g_pTweakBar, "Start Timer", TW_TYPE_BOOLCPP, &m_bMeasureTime, "");
*/

	switch (m_iTestCase)
	{
	case 0: // 
		break;
	case 1:
		break; //  
	case 2:
		break; //  

	default:break;
	}
	//TwAddVarRW(DUC->g_pTweakBar, "DrawForce and vel", TW_TYPE_BOOLCPP, &m_bDrawForce, "");
}

void FinalSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;



	m_fGravity = 0;
	m_fDamping = 0;
	




	switch (m_iTestCase)
	{
	case 0: // 
		break;
	case 1: //  
		break;
	case 2:  //  

		break;

	default:break;
	}


}
void FinalSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;

	this->notifyCaseChanged(m_iTestCase);

}

void FinalSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void FinalSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
