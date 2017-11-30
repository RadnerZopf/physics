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
		addRigidBody(Vec3(), Vec3(1.0f, 0.6f, 0.5f), 2);
		this->setVelocityOf(0, Vec3(0.05f, 0.05f, 0.2f));
		this->setAngularVelocityOf(0, Vec3(1.1f, 1.2f, 1.3f));

		break; //  single Body
	case 2:
		break; //  two Bodies
	case 3:
		break; // Complex sim
	default:break;
	}
}


void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{


	for (int i = 0; i <m_vRigidBodies.size(); i++)
	{
		RigidBodySystem &rigidBody = m_vRigidBodies[i];

		//euler
		rigidBody.position += timeStep * rigidBody.linearVelocity;
		rigidBody.linearVelocity += timeStep / rigidBody.mass * m_externalForce;
		
		//rigidBody.orientation.x += timeStep / 2 * rigidBody.angularVelocity.x * rigidBody.orientation.x;
		//rigidBody.orientation.y += timeStep / 2 * rigidBody.angularVelocity.y * rigidBody.orientation.y;
		//rigidBody.orientation.z += timeStep / 2 * rigidBody.angularVelocity.z * rigidBody.orientation.z;
		//rigidBody.orientation /= rigidBody.orientation.norm();

		//cout << "pos: " << rigidBody.position << endl;
		//cout << "lvel: " << rigidBody.linearVelocity << endl;
		//cout << "avel: " << rigidBody.angularVelocity << endl;



		
	}
}

void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	for (RigidBodySystem body : m_vRigidBodies)
	{
		
		DUC->setUpLighting(Vec3(0, 0, 0), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
		DUC->drawRigidBody(body.getWorldMat());
	}
}

//copied form MassSpring
void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed)
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

		m_externalForce = inputWorld;
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
