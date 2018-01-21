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
	case 1: // use this for final sim!!!
		addRigidBody(center, Vec3(1, 1, 1), 2, rotate90Z, Vec3(), Vec3(), 0.25);

		break; //  single Body
	case 2:

		addRigidBody(Vec3(-2,0.5,0), Vec3(1.0f, 1.0f, 1.0f), 2, rotate90YZ, Vec3(0.0,0.0,0.0), Vec3());
		addRigidBody(Vec3(2,0,0), Vec3(1.0f, 0.6f, 0.5f), 7, Quat(0.0,0.0,0.0,1.0), Vec3( -15.0, 0.0, 0.0), Vec3());


		break; //  two Bodies
	case 3:

		//add floor and make it fixed
		floorId = 0;
		addRigidBody(Vec3(0.0f, -1.0f, 0.0f), Vec3(10.0f, 1.0f, 10.0f), 2);
		m_vRigidBodies[floorId].fixed = true;


		addRigidBody(Vec3(0, 1.5, 0), Vec3(1.0f, 1.0f, 1.0f), 2, -rotate90YZ, Vec3(0.0, 5.0, 0.0), Vec3());
		addRigidBody(Vec3(0, 4, 0), Vec3(1.0f, 1.0f, 1.0f), 2, rotate90YZ, Vec3(0.0, 0.0, 0.0), Vec3());
		addRigidBody(Vec3(0, 8, 0), Vec3(1.0f, 1.0f, 1.0f), 2, -rotate90YZ, Vec3(0.0, 0.0, 0.0), Vec3(1.0f, 1.0f, 0.0f));
		addRigidBody(Vec3(-1, 6, 0), Vec3(1.0f, 1.0f, 1.0f), 2, rotate90YZ, Vec3(0.0, 0.0, 0.0), Vec3());
		break; // Complex sim
	default:break;
	}
}
 
void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{
	//cout << "new ts" << endl;

	for (int i = 0; i <m_vRigidBodies.size(); i++)
	{


		RigidBodySystem &rigidBody = m_vRigidBodies[i];

		if (!rigidBody.fixed)
		{
			rigidBody.saveState();

			//not sure how to plare cursor meaningful in 3D; this has an "ok" effect
			rigidBody.applyForce(Vec3(m_mouse.x, m_mouse.y, rigidBody.position.z), m_externalForce); 

			//euler
			rigidBody.position += timeStep * rigidBody.linearVelocity;
			rigidBody.linearVelocity += timeStep / rigidBody.mass * rigidBody.force;

			if (m_iTestCase == 3 && gravity) // compex sim -> apply gravity
				rigidBody.linearVelocity += Vec3(0.0f, EARTH_ACCEL, 0.0f) * timeStep;

			//this is prob. borked
			Quat tmp = Quat(0, rigidBody.angularVelocity.x, rigidBody.angularVelocity.y, rigidBody.angularVelocity.z);
			rigidBody.orientation += timeStep / 2 * tmp * rigidBody.orientation;
			rigidBody.orientation = rigidBody.orientation.unit();

			rigidBody.angularMomentum += timeStep * rigidBody.torque;

			rigidBody.angularVelocity = rigidBody.getInertiaTensor() * rigidBody.angularMomentum;


			//cout << "pos: " << rigidBody.position << endl;
			//cout << "lvel: " << rigidBody.linearVelocity << endl;
			//cout << "avel: " << rigidBody.angularVelocity << endl;

		}
	}
	//clear ext force in case input stops (otherwise we would constantly add the last input force) 
	m_externalForce = Vec3();

	//collision detection
	for (int i = 0; i < m_vRigidBodies.size(); ++i)
	{
		for (int j = i + 1; j < m_vRigidBodies.size(); ++j)
		{
			RigidBodySystem& a = m_vRigidBodies[i]; 
			RigidBodySystem& b = m_vRigidBodies[j]; 

			CollisionInfo col = checkCollisionSAT(a.getWorldMat(), b.getWorldMat()); //redundant worldmat calculations here, future me should fix that
			


			if (col.isValid)
			{
				
				Vec3 vrel = col.normalWorld * (a.linearVelocity + cross(a.angularVelocity, col.collisionPointWorld) - b.linearVelocity - cross(b.angularVelocity, col.collisionPointWorld)); 

				//velocity and col- normal have opposite direction
				if (vrel.x >0 & vrel.y >0 & vrel.z > 0)//collision already resolved, do noting
					continue;

				cout << a.linearVelocity << endl;
				cout << b.linearVelocity << endl;
				
				cout << "Valid Collision" << endl;

				Vec3 somethingINeedToDot = cross(a.getInertiaTensor() * cross(a.position, col.normalWorld), a.position)
					+ cross(b.getInertiaTensor() * cross(b.position, col.normalWorld), b.position);
				double denom = 1 / a.mass + 1 / b.mass + (somethingINeedToDot.x * col.normalWorld.x + somethingINeedToDot.y * col.normalWorld.y + somethingINeedToDot.z * col.normalWorld.z);

				somethingINeedToDot = vrel; 
				double impA = (1 + a.bouncieness)*(somethingINeedToDot.x * col.normalWorld.x + somethingINeedToDot.y * col.normalWorld.y + somethingINeedToDot.z * col.normalWorld.z)/denom;
				double impB = (1 + b.bouncieness)*(somethingINeedToDot.x * col.normalWorld.x + somethingINeedToDot.y * col.normalWorld.y + somethingINeedToDot.z * col.normalWorld.z)/denom;

				//if(!a.fixed) a.loadOldState();
				if(!b.fixed) b.loadOldState();

				a.linearVelocity += (impA / a.mass) * col.normalWorld; 
				b.linearVelocity -= (impB / b.mass) * col.normalWorld;

				a.angularMomentum += cross(a.position, impA * col.normalWorld);
				b.angularMomentum -= cross(b.position, impB * col.normalWorld);

				//debug out
				cout << "ImpA: " << impA << " | impB: " << impB << endl; 
				
				cout << a.linearVelocity << endl;
				cout << b.linearVelocity << endl;


			}
		}
	}
}


void RigidBodySystemSimulator::interactWithSystem(int type, Simulator* other)
{
	switch (type)
	{

	case TYPE_MASS_SPRING:
	{

		break; 
	}

	case TYPE_RIGIDBODY: 
	{

		break; 
	}

	case TYPE_SPH:
	{

		break; 
	}
	}


}

void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext, DrawingUtilitiesClass* _DUC)
{
	if (_DUC != nullptr)
		this->DUC = _DUC;
	bool renderAll = true;
	for (RigidBodySystem body : m_vRigidBodies)
	{
		if (!body.fixed || renderAll)
		{
			DUC->setUpLighting(Vec3(0, 0, 0), 0.4*Vec3(1, 1, 1), 2000.0, Vec3(0.5, 0.5, 0.5));
			DUC->drawRigidBody(body.getWorldMat());
		}
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
		float inputScale = 0.001f;
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
