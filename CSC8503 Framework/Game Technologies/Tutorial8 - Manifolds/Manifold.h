#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>
#include <ncltech\SimpleMeshObject.h>
#include <ncltech\Constraint.h>
#include <ncltech\DistanceConstraint.h>
#include<ncltech\CollisionDetection.h>
#include <ncltech\CollisionShape.h>
#include <ncltech\SphereCollisionShape.h>
class Manifold
{ 
public:
	Manifold(PhysicsObject* nodeA, PhysicsObject* nodeB);
	~Manifold(); //Called whenever a new collision contact between A & B are found 
	void AddContact(const Vector3& globalOnA, const Vector3& globalOnB, const Vector3 & normal, const float  & penetration); //Sequentially solves each contact constraint 
	void ApplyImpulse();
	void PreSolverStep(float dt);	//Debug draws the manifold surface area 
	void DebugDraw() const; 		//Get the physics  objects
	PhysicsObject* NodeA() {
		return m_NodeA; 
	}
	PhysicsObject* NodeB() {
		return m_NodeB;
	}
	protected:
		void UpdateConstraint(Contact& c);
	protected:
			PhysicsObject* m_NodeA;
			PhysicsObject* m_NodeB;
			std::vector <Contact > m_Contacts;
};