
#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>
#include <ncltech\SimpleMeshObject.h>
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

	std::vector<Vector3> m_TrajectoryPoints;
};