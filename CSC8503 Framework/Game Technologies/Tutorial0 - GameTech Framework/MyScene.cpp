#include "MyScene.h"
#include "Player.h"
#include <Tardis.h>

#include <nclgl/OBJMesh.h>

#include <ncltech\SimpleMeshObject.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\CommonMeshes.h>
#include <ncltech\NCLDebug.h>
#include<ncltech\CuboidCollisionShape.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\Constraint.h>


MyScene::MyScene(Window& window) : Scene(window)
{

	//CubeRobot::CreateCube();
	if (init == true)
		init = InitialiseGL();

	UpdateWorldMatrices(m_RootGameObject, Matrix4());
}

MyScene::~MyScene()
{
	if (m_TargetTexture)
	{
		glDeleteTextures(1, &m_TargetTexture);
		m_TargetTexture = NULL;
	}
}

bool MyScene::InitialiseGL()
{
	m_Camera->SetPosition(Vector3(-6.25f, 2.0f, 10.0f));

	PhysicsEngine::Instance()->SetGravity(Vector3(0.0f, -9.81f, 0.0f));		//No Gravity
	PhysicsEngine::Instance()->SetDampingFactor(1.0f);						//No Damping



	//Create Ground
	SimpleMeshObject* ground = new SimpleMeshObject("Ground");
	ground->SetMesh(CommonMeshes::Cube(), false);
	ground->SetLocalTransform(Matrix4::Translation(Vector3(0.0, 0.0, 0.0f)) * Matrix4::Scale(Vector3(20.0f, 0.5f, 20.f))); //80m width, 1m height, 80m depth
	ground->SetColour(Vector4(0.5f, 0.7f, 1.0f, 1.0f));  //0.2f, 1.0f, 0.5f, 1.0f
	ground->SetBoundingRadius(80.0f * 80.f);
	ground->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(20.0f, 0.5f, 20.f))));

	this->AddGameObject(ground);



	//Create a Sphere
	SimpleMeshObject* sphere = new SimpleMeshObject("Sphere");
	sphere->SetMesh(CommonMeshes::Sphere(), false);
	sphere->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));//1m radius
	sphere->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
	sphere->SetBoundingRadius(1.0f);
	sphere->Physics()->SetInverseMass(1.f);
	sphere->Physics()->SetCollisionShape(new SphereCollisionShape(0.5f));


	sphere->Physics()->SetPosition(Vector3(-12.5f, 1.0f, 0.f));
	this->AddGameObject(sphere);



	//Create a Cuboid
	SimpleMeshObject* cube = new SimpleMeshObject("Cube");
	cube->SetMesh(CommonMeshes::Cube(), false);
	cube->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));	//1m x 1m x 1m
	cube->SetColour(Vector4(0.2f, 0.5f, 1.0f, 1.0f));
	cube->SetBoundingRadius(1.5f);

	cube->Physics()->SetPosition(Vector3(12.5f, 1.0f, 0.0f));

	this->AddGameObject(cube);



	//Create a 'Player'
	Player* player = new Player("Player1");
	player->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	player->SetBoundingRadius(2.0f);

	player->Physics()->SetPosition(Vector3(0.0f, 1.0f, 0.0f));

	this->AddGameObject(player);



	return true;
}

void MyScene::UpdateScene(float msec)
{
	Scene::UpdateScene(msec);

	/*
	Game Update Logic
	*/
	//Shooting spheres from the camera
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J)){
		SimpleMeshObject* m_Sphere1 = new SimpleMeshObject("Sphere");
		m_Sphere1->SetMesh(CommonMeshes::Sphere(), false);
		m_Sphere1->SetLocalTransform(Matrix4::Scale(Vector3(0.3f, 0.3f, 0.3f)));
		m_Sphere1->SetColour(Vector4(1.0f, 0.9f, 0.8f, 1.0f));
		m_Sphere1->SetBoundingRadius(1.0f);
		m_Sphere1->Physics()->SetCollisionShape(new SphereCollisionShape(0.3f));

		m_Sphere1->Physics()->SetPosition(m_Camera->GetPosition());

		/*m_Sphere->Physics()->SetPosition(Vector3(-12.5f, 2.0f, 0.f));*/

		m_Sphere1->Physics()->SetInverseMass(1.f);

		Matrix3 viewRotation = Matrix3(viewMatrix);
		viewRotation = Matrix3::Inverse(viewRotation); //Only needed for viewmatrix as viewmatrix is an inverse rotation.

		Vector3 forward = viewRotation * Vector3(0, 0, -1);

		m_Sphere1->Physics()->SetLinearVelocity(forward * 20.0f);
		/*	m_Sphere->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));*/

		this->AddGameObject(m_Sphere1);


		//SimpleMeshObject* m_Sphere2 = new SimpleMeshObject("Sphere2");
		//m_Sphere2->SetMesh(CommonMeshes::Sphere(), false);
		//m_Sphere2->SetLocalTransform(Matrix4::Scale(Vector3(0.3f, 0.3f, 0.3f)));
		//m_Sphere2->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
		//m_Sphere2->SetBoundingRadius(1.0f);
		//m_Sphere2->Physics()->SetCollisionShape(new SphereCollisionShape(0.3f));

		//m_Sphere2->Physics()->SetPosition(m_Camera->GetPosition());

		///*m_Sphere->Physics()->SetPosition(Vector3(-12.5f, 2.0f, 0.f));*/

		//m_Sphere2->Physics()->SetInverseMass(1.f);

		//m_Sphere2->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, -20.0f));
		///*	m_Sphere->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));*/

		//this->AddGameObject(m_Sphere1);

		//DistanceConstraint* pendulum_holder = new DistanceConstraint(m_Sphere1->Physics(), m_Sphere2->Physics(), m_Sphere1->Physics()->GetPosition(), m_Sphere2->Physics()->GetPosition());
		//PhysicsEngine::Instance()->AddConstraint(pendulum_holder);
	}
	//
	
	//Quit the program
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_X)){
		exit(1);
	}
	//

	GameObject* obj = Scene::FindGameObject("Player1");
	if (obj != NULL)
	{
		Vector3 player_pos = obj->Physics()->GetPosition();
		NCLDebug::Log(Vector3(1.0f, 0.0f, 0.0f), "Player Position - %5.2fm, %5.2fm, %5.2fm\n", player_pos.x, player_pos.y, player_pos.z);
	}

}

void MyScene::RenderScene()
{
	Scene::RenderScene();

	/* 
	Special Rendering Cases (Scene will call all render() functions on GameObjects's is holds)
	*/
}

