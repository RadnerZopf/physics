#ifndef SPHSYSTEMSIMULATOR_h
#define SPHSYSTEMSIMULATOR_h
#include "Simulator.h"
<<<<<<< HEAD
#include "SphereSystem.h"
=======
>>>>>>> cd08e57063624ad0a5fa43cd9ec88b9f5a0c3097
//#include "spheresystem.h", add your sphere system header file

#define NAIVEACC 0
#define GRIDACC 1
<<<<<<< HEAD
#define KDTREEACC 2

#define EARTH_ACCEL GamePhysics::Real(-9.81)


struct Box
{
	Vec3 pos; 
	Vec3 posPlusoffset; 
};
=======
>>>>>>> cd08e57063624ad0a5fa43cd9ec88b9f5a0c3097

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
<<<<<<< HEAD


	inline void simNaiveAcc(float dt); 
	inline void simGridAcc(float dt);
	inline void simKDTreeAcc(float dt);

	inline void detectAndResolveSphereOnSphereCollision(SphereSystem* system, int first, int second, float dt); 
	inline void detectAndResolveSphereOnBoundsCollision(SphereSystem* system, int first,  float dt);


=======
>>>>>>> cd08e57063624ad0a5fa43cd9ec88b9f5a0c3097
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
<<<<<<< HEAD

	float m_fRadTimes2;
	float m_fRadTimes2Squared; //squared dist

	Box m_boxOuterBounds; 
	Box m_boxRadiusBounds;

=======
>>>>>>> cd08e57063624ad0a5fa43cd9ec88b9f5a0c3097
	
	int   m_iKernel; // index of the m_Kernels[5], more detials in SphereSystemSimulator.cpp
	static std::function<float(float)> m_Kernels[5];
	
	int   m_iAccelerator; // switch between NAIVEACC and GRIDACC, (optionally, KDTREEACC, 2)
	
<<<<<<< HEAD
	SphereSystem * m_pSphereSystem = nullptr; // add your own sphere system member!
	// for Demo 3 only:
	// you will need multiple SphereSystem objects to do comparisons in Demo 3
	// m_iAccelerator should be ignored.
	SphereSystem * m_pSphereSystemGrid = nullptr;
=======
	//SphereSystem * m_pSphereSystem; // add your own sphere system member!
	// for Demo 3 only:
	// you will need multiple SphereSystem objects to do comparisons in Demo 3
	// m_iAccelerator should be ignored.
	// SphereSystem * m_pSphereSystemGrid; 
>>>>>>> cd08e57063624ad0a5fa43cd9ec88b9f5a0c3097

};

#endif