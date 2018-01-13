#pragma once
#include "Simulator.h"
#include "MassSpringSystemSimulator.h"
#include "RigidBodySystemSimulator.h"
#include "SphereSystemSimulator.h"

class FinalSimulator :
	public Simulator
{
public:
	FinalSimulator();
	~FinalSimulator();

	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void onClick(int x, int y);
	void onMouse(int x, int y);

	void interactWithSystem(int type, Simulator* other) {} //called only for subsystems

	float m_fGravity; 
	float m_fDamping; 
	Vec3 externalForce;

private:
	//
	MassSpringSystemSimulator* bouncyNet; 
	RigidBodySystemSimulator* objectThrown; 
	SphereSystemSimulator* landingPool; 

	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
};

