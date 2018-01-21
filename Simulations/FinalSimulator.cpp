#include "FinalSimulator.h"



FinalSimulator::FinalSimulator()
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

	simulators.clear(); 

	// setup test scene
	m_iTestCase = testCase;



	m_fGravity = 0;
	m_fDamping = 0.1;
/*
#define SIM_BOUNCYNET 0
#define SIM_cube 1
#define SIM_POOL 2
*/


	MassSpringSystemSimulator* bouncyNet = new MassSpringSystemSimulator(); 
	simulators.push_back(bouncyNet); 
	//needs to be centered on a point w/ x=z cuz MP.complex sim is setup poorly-> so i guess this is going center scene
	bouncyNet->center = Vec3(0, 1, 0); 
	bouncyNet->DUC = DUC; 
	bouncyNet->m_iTestCase = 3; // clothy net thing
	bouncyNet->setDampingFactor(m_fDamping); 
	


	bouncyNet->notifyCaseChanged(3); 

	RigidBodySystemSimulator* cube = new RigidBodySystemSimulator(); 
	simulators.push_back(cube);
	cube->center = Vec3(0, 2.5, 0); 
	cube->DUC = DUC;
	cube->m_iTestCase = 1; // cube that will be thrown
	

	cube->notifyCaseChanged(1);


/* this is fucked
	SphereSystemSimulator* pool = new SphereSystemSimulator(); 
	simulators.push_back(pool);

	pool->center = Vec3(0, 0.5, 2);
	pool->DUC = DUC;

	pool->m_iTestCase = 1; // cube that will be thrown


	pool->notifyCaseChanged(1);


	/**/



}

void FinalSimulator::simulateTimestep(float timeStep)
{

	timeStep *= 2;  // speedup

	//TODO
	//input --> push cube 

	RigidBodySystemSimulator * cubesim = dynamic_cast<RigidBodySystemSimulator*>(simulators[SIM_CUBE]);

	//detect collision between systems

	for (int i = 0; i < simulators.size(); ++i)
	{
		for (int j = i; j < simulators.size(); ++j)
		{
			Simulator *a = simulators[i];
			Simulator *b = simulators[j];
			if ((a->center - b->center) * (a->center - b->center) < a->collisionRadius + b->collisionRadius)
			{
				a->interactWithSystem(simType(b), b); // mass spring vs rigidbody
			}
		}
	}

	//update systems

	for (Simulator* sim : simulators)
	{
		sim->simulateTimestep(timeStep);
	}

}

void FinalSimulator::externalForcesCalculations(float timeElapsed)
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
		m_inputForce = inputWorld * inputScale;
	}
}

void FinalSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext, DrawingUtilitiesClass* _DUC)
{
	for (Simulator* sim : simulators)
	{
		sim->drawFrame(pd3dImmediateContext, this->DUC); 
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


int FinalSimulator::simType(Simulator* sim)
{
	if (dynamic_cast<MassSpringSystemSimulator*>(sim))
	{
		return TYPE_MASS_SPRING; 
	}
	if (dynamic_cast<RigidBodySystemSimulator*>(sim))
	{
		return TYPE_RIGIDBODY;
	}
	if (dynamic_cast<SphereSystemSimulator*>(sim))
	{
		return TYPE_SPH;
	}
}
