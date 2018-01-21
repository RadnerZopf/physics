#ifndef RIGIDBODYSYSTEMSIMULATOR_h
#define RIGIDBODYSYSTEMSIMULATOR_h
#include "Simulator.h"
//add your header for your rigid body system, for e.g.,
#include "RigidBodySystem.h" 

#define TESTCASEUSEDTORUNTEST 2

#define EARTH_ACCEL GamePhysics::Real(-9.81)

class RigidBodySystemSimulator:public Simulator{
public:
	// Construtors
	RigidBodySystemSimulator();
	
	// Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext, DrawingUtilitiesClass* DUC = nullptr);
	void notifyCaseChanged(int testCase);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void onClick(int x, int y);
	void onMouse(int x, int y);

	// ExtraFunctions
	int getNumberOfRigidBodies() { return m_vRigidBodies.size(); }
	Vec3 getPositionOfRigidBody(int i) { return m_vRigidBodies[i].position; }
	Vec3 getLinearVelocityOfRigidBody(int i) { return m_vRigidBodies[i].linearVelocity; }
	Vec3 getAngularVelocityOfRigidBody(int i) { return m_vRigidBodies[i].angularVelocity; }
	void applyForceOnBody(int i, Vec3 loc, Vec3 force) { m_vRigidBodies[i].applyForce(loc, force); }
	void addRigidBody(Vec3 position, Vec3 size, int mass, Quat _orientation = Quat(0.0f, 0.0f, 0.0f, 1.0f), Vec3 _linearVelocity = Vec3(), Vec3 _angularVelocity = Vec3(), float bounciness = 0.0f) { m_vRigidBodies.push_back(RigidBodySystem(position, size, mass, _orientation, _linearVelocity, _angularVelocity, bounciness)); }
	void setOrientationOf(int i,Quat orientation) { m_vRigidBodies[i].orientation = orientation; }
	void setVelocityOf(int i, Vec3 velocity) { m_vRigidBodies[i].linearVelocity = velocity; }
	void setAngularVelocityOf(int i, Vec3 velocity) { m_vRigidBodies[i].angularVelocity = velocity; }

	void interactWithSystem(int type, Simulator* other);


	RigidBodySystem* getRigidBodySystem() { return &m_vRigidBodies[0]; } //final sim

private:

	//much lazy
	Vec3 cross(Vec3& a, Vec3& b)
	{
		XMVECTOR xmloc = a.toDirectXVector();
		XMVECTOR xmforce = b.toDirectXVector();
		XMVECTOR xmtorque = XMVector3Cross(xmloc, xmforce);
		return Vec3(xmtorque);
	}


	// Attributes
	// add your RigidBodySystem data members, for e.g.,
	// RigidBodySystem * m_pRigidBodySystem; 
	Vec3 m_externalForce;


	vector<RigidBodySystem> m_vRigidBodies; 

	int floorId;
	bool gravity = false;

	// UI Attributes
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	};
#endif