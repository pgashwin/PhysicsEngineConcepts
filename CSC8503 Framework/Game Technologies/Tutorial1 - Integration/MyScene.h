
#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>
#include <ncltech\SimpleMeshObject.h>
#include <ncltech\Constraint.h>
#include <ncltech\DistanceConstraint.h>
#include<ncltech\CollisionDetection.h>
#include <ncltech\CollisionShape.h>
#include <ncltech\SphereCollisionShape.h>



class MyScene : public Scene
{
public:
	MyScene(Window& window);
	~MyScene();

	bool InitialiseGL()	override;
	void RenderScene() override;
	void UpdateScene(float dt)  override;
protected:
	GLuint			  m_TargetTexture;
	SimpleMeshObject* pendulumball;
	SimpleMeshObject* pendulumball1;

	std::vector<Vector3> m_TrajectoryPoints;
};