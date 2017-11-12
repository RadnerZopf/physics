#ifndef MASSSPRINGSYSTEMSIMULATOR_h
#define MASSSPRINGSYSTEMSIMULATOR_h
#include "Simulator.h"

// Do Not Change
#define EULER 0
#define LEAPFROG 1
#define MIDPOINT 2
// Do Not Change

#define EARTH_ACCEL GamePhysics::Real(-9.81)
#define FLOOR_Y -0.5f

struct Masspoint
{
	Masspoint(Vec3 _pos, Vec3 _velocity, bool _fixed, float _mass, int _gameObject = 0) : pos(_pos), velocity(_velocity), force(0.0f, 0.0f, 0.0f), fixed(_fixed), mass(_mass), gameObject(_gameObject)
	{

	}

	Vec3 pos, velocity, force; 
	Vec3 tmpPos, tmpVel; 
	float mass;
	bool fixed; 

	int gameObject; //indicates to which gameobject the MP belongs ( collision detection ) 

} ;

struct Spring
{
	Spring(int _masspoint1, int _masspoint2, float _initialLength, float _stiffness) : masspoint1(_masspoint1), masspoint2(_masspoint2), initialLength(_initialLength), stiffness(_stiffness)
	{

	}

	int masspoint1, masspoint2;
	float initialLength; 

	float stiffness;
	float currentLength; 
	
};

struct GameObject // might need that at some point, dunno
{
	int id = 0; 
	int pointOffset = 0; 
	int springOffset = 0; 
	int pointCount = 0; 
	int springCount = 0;
};

class MassSpringSystemSimulator:public Simulator{
public:
	// Construtors
	MassSpringSystemSimulator();
	
	// UI Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void onClick(int x, int y);
	void onMouse(int x, int y);

	// Specific Functions
	void setMass(float mass) {m_fMass = mass;}
	void setStiffness(float stiffness) { m_fStiffness = stiffness; }
	void setDampingFactor(float damping) { m_fDamping = damping; }
	int addMassPoint(Vec3 position, Vec3 velocity, bool isFixed, float mass = -1, int gameObject = 0); // mass == -1 -> use m_fMass
	void addSpring(int masspoint1, int masspoint2, float initialLength, float stiffness = -1); // stiffness == -1 -> use m_fStiffness
	int getNumberOfMassPoints() { return m_vPoints.size(); }
	int getNumberOfSprings() { return m_vSprings.size(); }
	Vec3 getPositionOfMassPoint(int index) { return m_vPoints[index].pos; }
	Vec3 getVelocityOfMassPoint(int index) { return m_vPoints[index].velocity; }
	void applyExternalForce(Vec3 force) { m_externalForce += force; } // apply means add, right <--> += vs = 
	
	// Do Not Change
	void setIntegrator(int integrator) {
		m_iIntegrator = integrator;
	}


private:


	void computeElasticForces(); 

	// Data Attributes
	float m_fMass;
	float m_fStiffness;
	float m_fDamping = 0;

	float m_fFixedTimestep = -1; 

	int m_iIntegrator = EULER;

	// UI Attributes
	Vec3 m_externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;

	bool m_bDrawForce = false; 

	vector<Masspoint> m_vPoints;
	vector<Spring> m_vSprings; 
	vector<GameObject> m_vGameObjects;

};
#endif