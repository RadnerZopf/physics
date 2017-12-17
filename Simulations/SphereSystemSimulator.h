#ifndef SPHSYSTEMSIMULATOR_h
#define SPHSYSTEMSIMULATOR_h
#include "Simulator.h"
#include "SphereSystem.h"
//#include "spheresystem.h", add your sphere system header file

#define NAIVEACC 0
#define GRIDACC 1
#define KDTREEACC 2


struct Box
{
	Vec3 pos; 
	Vec3 offset; 
};

class SphereSystemSimulator:public Simulator{
public:
	// Construtors
	SphereSystemSimulator();
	// Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void onClick(int x, int y);
	void onMouse(int x, int y);
	
protected:


	inline void simNaiveAcc(float dt); 
	inline void simGridAcc(float dt);
	inline void simKDTreeAcc(float dt);

	inline void detectAndResolveSphereOnSphereCollision(SphereSystem* system, int first, int second, float dt); 
	inline void detectAndResolveSphereOnBoundsCollision(SphereSystem* system, int first,  float dt);


	inline void measureTime(float timeStep);

	// Attributes
	Vec3 externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	float m_fMass;
	float m_fRadius;
	float m_fForceScaling;
	float m_fDamping;
	int   m_iNumSpheres;
	float m_fGravity; 
	
	float m_fRadTimes2;
	float m_fRadTimes2Squared; //squared dist

	bool m_bDrawScene = true; 
	bool m_bDrawGrid = true;

	int	  m_iTimeMeasureSteps = 100;
	bool  m_bMeasureTime = false;

	Box m_boxOuterBounds; 
	
	int   m_iKernel; // index of the m_Kernels[5], more detials in SphereSystemSimulator.cpp
	static std::function<float(float)> m_Kernels[5];

	int   m_iAccelerator; // switch between NAIVEACC and GRIDACC, (optionally, KDTREEACC, 2)
	
	SphereSystem * m_pSphereSystem = nullptr; // add your own sphere system member!
	// for Demo 3 only:
	// you will need multiple SphereSystem objects to do comparisons in Demo 3
	// m_iAccelerator should be ignored.
	SphereSystem * m_pSphereSystemGrid = nullptr;

};

#endif
