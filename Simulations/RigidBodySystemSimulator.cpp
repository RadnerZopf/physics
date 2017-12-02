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


	Quat rotate90Z = Quat(sin(XMConvertToRadians(90 / 2)), 0, 0, cos(XMConvertToRadians(90 / 2))); 
	rotate90Z /= rotate90Z.norm(); 

	Quat rotate90YZ = Quat(sin(XMConvertToRadians(90 / 2)), 0, cos(XMConvertToRadians(90 / 2)), cos(XMConvertToRadians(90 / 2)));
	rotate90YZ /= rotate90YZ.norm();

	switch (m_iTestCase)
	{
	case 0: // one step
		addRigidBody(Vec3(), Vec3(1.0f, 0.6f, 0.5f), 2, rotate90Z,  Vec3(), Vec3());
		applyForceOnBody(0, Vec3(0.3, 0.5, 0.25), Vec3(1.0, 1.0, 0.0));

		simulateTimestep(2.0); 

		cout << "Position:\t" << m_vRigidBodies[0].position << "\nLinear Vel:\t " << m_vRigidBodies[0].linearVelocity << "\n Angular vel: \t" << m_vRigidBodies[0].angularVelocity << endl; 

		break;
	case 1:
		addRigidBody(Vec3(), Vec3(1.0f, 0.6f, 0.5f), 2, rotate90Z, Vec3(), Vec3());
		applyForceOnBody(0, Vec3(0.3, 0.5, 0.25), Vec3(1.0, 1.0, 0.0));


		break; //  single Body
	case 2:

		addRigidBody(Vec3(-2,0,0), Vec3(1.0f, 1.0f, 0.5f), 2, rotate90YZ, Vec3(2.0,0.0,0.0), Vec3());
		addRigidBody(Vec3(2,0,0), Vec3(1.0f, 0.6f, 0.5f), 5, Quat(0.0,0.0,0.0,1.0), Vec3( -3.0, 0.0, 0.0), Vec3());


		break; //  two Bodies
	case 3:
		break; // Complex sim
	default:break;
	}
}


void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{

	if (m_iTestCase == 2) {
		cout << "need bp"<< endl; 
	}

	for (int i = 0; i <m_vRigidBodies.size(); i++)
	{
		RigidBodySystem &rigidBody = m_vRigidBodies[i];

		//euler
		rigidBody.position += timeStep * rigidBody.linearVelocity;
		rigidBody.linearVelocity += timeStep / rigidBody.mass * rigidBody.force;


		Quat q = Quat(0.0, rigidBody.angularVelocity.x, rigidBody.angularVelocity.y, rigidBody.angularVelocity.z);

		rigidBody.orientation += timeStep / 2 * q * rigidBody.orientation; 

		rigidBody.angularMomentum += timeStep * rigidBody.torque; 

		rigidBody.angularVelocity = rigidBody.getInertiaTensor() * rigidBody.angularMomentum; 


		//cout << "pos: " << rigidBody.position << endl;
		//cout << "lvel: " << rigidBody.linearVelocity << endl;
		//cout << "avel: " << rigidBody.angularVelocity << endl;

		m_externalForce = Vec3();
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
		float inputScale = 0.1f;
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
