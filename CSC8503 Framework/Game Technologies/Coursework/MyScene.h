
#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>
#include <ncltech\SimpleMeshObject.h>
#include<ncltech\CollisionDetection.h>
#include <ncltech\CollisionShape.h>
#include <ncltech\SphereCollisionShape.h>
#include <ncltech\PyramidCollisionShape.h>
#include "nclgl/SceneNode.h"



class MyScene : public Scene
{
public:
	MyScene(Window& window);
	~MyScene();

	bool InitialiseGL()	override;
	void RenderScene() override;
	void UpdateScene(float dt)  override;
	void goalkeeper();
	void tardisOpacity(float a);

	


		
	float time;	  //tardis
protected:
	GLuint			  m_TargetTexture;
	int					m_Score = 0;
	std::vector<Vector3> m_TrajectoryPoints;
};//