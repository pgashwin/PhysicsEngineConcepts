#include "MyScene.h"

#include <nclgl/OBJMesh.h>

#include <ncltech\SimpleMeshObject.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\CommonMeshes.h>
#include <ncltech\NCLDebug.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\Constraint.h>


MyScene::MyScene(Window& window) : Scene(window)
{
	if (init == true)
		init = InitialiseGL();

	UpdateWorldMatrices(m_RootGameObject, Matrix4());


	//PhysicsEngine::Instance()->SetUpdateTimestep(1.0f / 1.0f);	//1 Update per second

	//PhysicsEngine::Instance()->SetUpdateTimestep(1.0f / 5.0f);	//5 Updates per second

	PhysicsEngine::Instance()->SetUpdateTimestep(1.0f / 60.0f);		//60 Updates per second
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


	m_TargetTexture = SOIL_load_OGL_texture(TEXTUREDIR"target.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glBindTexture(GL_TEXTURE_2D, m_TargetTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); //No linear interpolation to get crisp checkerboard no matter the scalling
	glBindTexture(GL_TEXTURE_2D, 0);


	//Create Ground
	SimpleMeshObject* ground = new SimpleMeshObject("Ground");
	ground->SetMesh(CommonMeshes::Cube(), false);
	ground->SetLocalTransform(Matrix4::Scale(Vector3(80.0f, 0.1f, 2.f)));
	ground->SetColour(Vector4(0.2f, 1.0f, 0.5f, 1.0f));
	ground->SetBoundingRadius(80.0f * 80.f);


	this->AddGameObject(ground);

	//Create Target
	SimpleMeshObject* target = new SimpleMeshObject("Target");
	target->SetMesh(CommonMeshes::Cube(), false);
	target->SetTexture(m_TargetTexture, false);
	target->SetLocalTransform(Matrix4::Scale(Vector3(0.1f, 2.0f, 2.f)));
	target->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	target->SetBoundingRadius(4.0f);

	target->Physics()->SetPosition(Vector3(0.1f, 2.0f, 0.0f));

	this->AddGameObject(target);


	//Create a projectile
	SimpleMeshObject* m_Sphere = new SimpleMeshObject("Sphere");
	m_Sphere->SetMesh(CommonMeshes::Sphere(), false);
	m_Sphere->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
	m_Sphere->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
	m_Sphere->SetBoundingRadius(1.0f);

	m_Sphere->Physics()->SetPosition(Vector3(-12.5f, 2.0f, 0.f));

	m_Sphere->Physics()->SetInverseMass(1.f);

	m_Sphere->Physics()->SetCollisionShape(new SphereCollisionShape(0.5f));


	m_Sphere->Physics()->SetLinearVelocity(Vector3(0.f, 2.5f, 0.0f));
	m_Sphere->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));

	this->AddGameObject(m_Sphere);

	//Create a projectile
	SimpleMeshObject* m_Sphere1 = new SimpleMeshObject("Sphere1");
	m_Sphere1->SetMesh(CommonMeshes::Sphere(), false);
	m_Sphere1->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
	m_Sphere1->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
	m_Sphere1->SetBoundingRadius(1.0f);

	m_Sphere1->Physics()->SetPosition(Vector3(-18.5f, 2.0f, 0.f));

	m_Sphere1->Physics()->SetInverseMass(1.f);

	m_Sphere1->Physics()->SetLinearVelocity(Vector3(0.f, 2.5f, 0.0f));
	m_Sphere1->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));

	this->AddGameObject(m_Sphere1);


	//Create holder
	SimpleMeshObject* holder = new SimpleMeshObject("holder");
	holder->SetMesh(CommonMeshes::Cube(), false);
	holder->SetTexture(m_TargetTexture, false);
	holder->SetLocalTransform(Matrix4::Scale(Vector3(0.1f, 0.1f, 0.1f)));
	holder->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	holder->SetBoundingRadius(4.0f);

	holder->Physics()->SetPosition(Vector3(5.0f, 5.0f, 0.0f));

	this->AddGameObject(holder);

	//Create a pendullum
	pendulumball = new SimpleMeshObject("pendulumball");
	pendulumball->SetMesh(CommonMeshes::Sphere(), false);
	pendulumball->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
	pendulumball->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
	pendulumball->SetBoundingRadius(1.0f);

	pendulumball->Physics()->SetPosition(Vector3(10.0f, 5.0f, 0.f));
	pendulumball->Physics()->SetInverseMass(1.f);
	pendulumball->Physics()->SetCollisionShape(new SphereCollisionShape(0.5f));
	//pendulumball->Physics()->SetLinearVelocity(Vector3(0.f, 2.5f, 0.0f));
	//pendulumball->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));

	this->AddGameObject(pendulumball);



	DistanceConstraint* pendulum_holder = new DistanceConstraint(holder->Physics(), pendulumball->Physics(), holder->Physics()->GetPosition(), pendulumball->Physics()->GetPosition());
	PhysicsEngine::Instance()->AddConstraint(pendulum_holder);
	/*Loading the pendullum an the holder to the distance constraint
	Create holder1*/
	SimpleMeshObject* holder1 = new SimpleMeshObject("holder1");
	holder1->SetMesh(CommonMeshes::Cube(), false);
	holder1->SetTexture(m_TargetTexture, false);
	holder1->SetLocalTransform(Matrix4::Scale(Vector3(0.1f, 0.1f, 0.1f)));
	holder1->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	holder1->SetBoundingRadius(4.0f);

	holder1->Physics()->SetPosition(Vector3(12.0f, 5.0f, 0.0f));

	this->AddGameObject(holder1);

	//Create a pendullum
	pendulumball1 = new SimpleMeshObject("pendulumball1");
	pendulumball1->SetMesh(CommonMeshes::Sphere(), false);
	pendulumball1->SetLocalTransform(Matrix4::Scale(Vector3(.5f, 0.5f, 0.5f)));
	pendulumball1->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
	pendulumball1->SetBoundingRadius(1.0f);
	pendulumball1->Physics()->SetCollisionShape(new SphereCollisionShape(0.5f));
	pendulumball1->Physics()->SetPosition(Vector3(15.0f, 5.0f, 0.f));
	pendulumball1->Physics()->SetInverseMass(1.f);

	//pendulumball->Physics()->SetLinearVelocity(Vector3(0.f, 2.5f, 0.0f));
	//pendulumball->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));

	this->AddGameObject(pendulumball1);


	DistanceConstraint* pendulum_holder1 = new DistanceConstraint(holder1->Physics(), pendulumball1->Physics(), holder1->Physics()->GetPosition(), pendulumball1->Physics()->GetPosition());

	PhysicsEngine::Instance()->AddConstraint(pendulum_holder1);



	return true;
}


void MyScene::UpdateScene(float msec)
{
	Scene::UpdateScene(msec);

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_N)){
		SimpleMeshObject* m_Sphere1 = new SimpleMeshObject("Sphere3");
		m_Sphere1->SetMesh(CommonMeshes::Sphere(), false);
		m_Sphere1->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
		m_Sphere1->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
		m_Sphere1->SetBoundingRadius(1.0f);
		m_Sphere1->Physics()->SetCollisionShape(new SphereCollisionShape(0.5f));

		m_Sphere1->Physics()->SetPosition(m_Camera->GetPosition());

		/*m_Sphere->Physics()->SetPosition(Vector3(-12.5f, 2.0f, 0.f));*/

		m_Sphere1->Physics()->SetInverseMass(1.f);

		m_Sphere1->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, -10.0f));
		/*	m_Sphere->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));*/

		this->AddGameObject(m_Sphere1);
	}

	GameObject* m_Sphere = Scene::FindGameObject("Sphere");
	if (m_Sphere != NULL)
	{

		if (!PhysicsEngine::Instance()->IsPaused())
		{
			m_TrajectoryPoints.push_back(m_Sphere->Physics()->GetPosition());
		}

		if (m_Sphere->Physics()->GetPosition().y < 0.0f) //Statement used when the projectile ball hits the ground and the scen gets paused.
		{
			PhysicsEngine::Instance()->SetPaused(false);
		}

		///-------COllision detection CHeck between two spheres------///
		GameObject* m_Sphere1 = Scene::FindGameObject("Sphere1");//finds the game object from the given name
		SphereCollisionShape *a = new SphereCollisionShape;
		a->SetRadius(0.5);
		CollisionDetection coll;
		if (coll.CheckSphereSphereCollision(pendulumball->Physics(), pendulumball1->Physics(), a, a))//calling the function to check for the collisions
		{
			pendulumball->Physics()->SetLinearVelocity(-pendulumball->Physics()->GetLinearVelocity());
			pendulumball1->Physics()->SetLinearVelocity(-pendulumball1->Physics()->GetLinearVelocity());
			//PhysicsEngine::Instance()->SetPaused(true);
		}
		delete a;

		/////-----COLLISION DETECTION ON SPHERE AND A PLANE-------////
		Vector3 normal = Vector3(0, 1, 0);
		float distance = 0.0f;// Vector3::Dot(normal, Vector3(0, 0, 0));
		if (abs(Vector3::Dot(m_Sphere->Physics()->GetPosition(), normal)) + distance <= 0.5f) {
			m_Sphere->Physics()->SetLinearVelocity(-m_Sphere->Physics()->GetLinearVelocity());

		}

	}
}

void MyScene::RenderScene()
{
	for (size_t i = 1; i < m_TrajectoryPoints.size(); i++)
	{
		NCLDebug::DrawHairLine(m_TrajectoryPoints[i - 1], m_TrajectoryPoints[i], Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	Scene::RenderScene();
}

//#include "MyScene.h"
//
//#include <nclgl/OBJMesh.h>
//
//#include <ncltech\SimpleMeshObject.h>
//#include <ncltech\PhysicsEngine.h>
//#include <ncltech\CommonMeshes.h>
//#include <ncltech\NCLDebug.h>
//
//
//MyScene::MyScene(Window& window) : Scene(window)
//{
//	if (init == true)
//		init = InitialiseGL();
//
//	UpdateWorldMatrices(m_RootGameObject, Matrix4());
//
//
//	//PhysicsEngine::Instance()->SetUpdateTimestep(1.0f / 1.0f);	//1 Update per second
//
//	//PhysicsEngine::Instance()->SetUpdateTimestep(1.0f / 5.0f);	//5 Updates per second
//
//	PhysicsEngine::Instance()->SetUpdateTimestep(1.0f / 60.0f);		//60 Updates per second
//}
//
//MyScene::~MyScene()
//{
//	if (m_TargetTexture)
//	{
//		glDeleteTextures(1, &m_TargetTexture);
//		m_TargetTexture = NULL;
//	}
//}
//
//bool MyScene::InitialiseGL()
//{
//	m_Camera->SetPosition(Vector3(-6.25f, 2.0f, 10.0f));
//
//	PhysicsEngine::Instance()->SetGravity(Vector3(0.0f, -9.81f, 0.0f));		//No Gravity
//	PhysicsEngine::Instance()->SetDampingFactor(1.0f);						//No Damping
//
//
//	m_TargetTexture = SOIL_load_OGL_texture(TEXTUREDIR"target.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
//	glBindTexture(GL_TEXTURE_2D, m_TargetTexture);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); //No linear interpolation to get crisp checkerboard no matter the scalling
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//
//	//Create Ground
//	SimpleMeshObject* ground = new SimpleMeshObject("Ground");
//	ground->SetMesh(CommonMeshes::Cube(), false);
//	ground->SetLocalTransform(Matrix4::Scale(Vector3(80.0f, 0.1f, 2.f)));
//	ground->SetColour(Vector4(0.2f, 1.0f, 0.5f, 1.0f));
//	ground->SetBoundingRadius(80.0f * 80.f);
//
//	ground->Physics()->SetPosition(Vector3(-6.25f, -0.2f, 0.0f));
//
//	this->AddGameObject(ground);
//
//
//
//
//	//Create Target
//	SimpleMeshObject* target = new SimpleMeshObject("Target");
//	target->SetMesh(CommonMeshes::Cube(), false);
//	target->SetTexture(m_TargetTexture, false);
//	target->SetLocalTransform(Matrix4::Scale(Vector3(0.1f, 2.0f, 2.f)));
//	target->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
//	target->SetBoundingRadius(4.0f);
//
//	target->Physics()->SetPosition(Vector3(0.1f, 2.0f, 0.0f));
//
//	this->AddGameObject(target);
//
//
//	//Create a projectile
//	SimpleMeshObject* m_Sphere = new SimpleMeshObject("Sphere");
//	m_Sphere->SetMesh(CommonMeshes::Sphere(), false);
//	m_Sphere->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
//	m_Sphere->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
//	m_Sphere->SetBoundingRadius(1.0f);
//
//	m_Sphere->Physics()->SetPosition(Vector3(-12.5f, 2.0f, 0.f));
//
//	m_Sphere->Physics()->SetInverseMass(1.f);
//
//	m_Sphere->Physics()->SetLinearVelocity(Vector3(0.f, 2.5f, 0.0f));
//	m_Sphere->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));
//
//	this->AddGameObject(m_Sphere);
//
//	//creating sphere 2 
//	SimpleMeshObject* Sphere = new SimpleMeshObject("Sphere1");
//	Sphere->SetMesh(CommonMeshes::Sphere(), false);
//	Sphere->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
//	Sphere->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
//	Sphere->SetBoundingRadius(1.0f);
//	
//	Sphere->Physics()->SetPosition(Vector3(-9.5f, 1.0f, 0.5f));
//
//	Sphere->Physics()->SetInverseMass(1.f);
//
//	Sphere->Physics()->SetLinearVelocity(Vector3(0.f, 2.5f, 0.0f));
//	Sphere->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));
//
//	this->AddGameObject(Sphere);
//
//	//cube 
//	//Create a Cuboid
//	SimpleMeshObject* cube = new SimpleMeshObject("Cube");
//	cube->SetMesh(CommonMeshes::Cube(), false);
//	cube->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));	//1m x 1m x 1m
//	cube->SetColour(Vector4(0.2f, 0.5f, 1.0f, 1.0f));
//	cube->SetBoundingRadius(1.5f);
//
//	cube->Physics()->SetPosition(Vector3(-9.5f, 3.0f, 0.3f));
//
//	this->AddGameObject(cube);
//
//	return true;
//}
//
//void MyScene::UpdateScene(float msec)
//{
//	Scene::UpdateScene(msec);
//
//
//	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_N)){
//		SimpleMeshObject* m_Sphere = new SimpleMeshObject("Sphere");
//		m_Sphere->SetMesh(CommonMeshes::Sphere(), false);
//		m_Sphere->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
//		m_Sphere->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
//		m_Sphere->SetBoundingRadius(1.0f);
//
//		m_Sphere->Physics()->SetPosition(m_Camera->GetPosition());
//
//		/*m_Sphere->Physics()->SetPosition(Vector3(-12.5f, 2.0f, 0.f));*/
//
//		m_Sphere->Physics()->SetInverseMass(1.f);
//
//		m_Sphere->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, -10.0f));
//	/*	m_Sphere->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));*/
//
//		this->AddGameObject(m_Sphere);
//
//	}
//
//	GameObject* m_Sphere = Scene::FindGameObject("Sphere");
//	if (m_Sphere != NULL)
//	{
//
//		if (!PhysicsEngine::Instance()->IsPaused())
//		{
//			m_TrajectoryPoints.push_back(m_Sphere->Physics()->GetPosition());
//		}
//
//		if (m_Sphere->Physics()->GetPosition().y < 0.0f)
//		{
//			PhysicsEngine::Instance()->SetPaused(true);
//		}
//	}
//}
//
//void MyScene::RenderScene()
//{
//	for (size_t i = 1; i < m_TrajectoryPoints.size(); i++)
//	{
//		NCLDebug::DrawHairLine(m_TrajectoryPoints[i - 1], m_TrajectoryPoints[i], Vector4(1.0f, 0.0f, 0.0f, 1.0f));
//	}
//
//	Scene::RenderScene();
//}
//#include "MyScene.h"
//
//#include <nclgl/OBJMesh.h>
//
//#include <ncltech\SimpleMeshObject.h>
//#include <ncltech\PhysicsEngine.h>
//#include <ncltech\CommonMeshes.h>
//#include <ncltech\NCLDebug.h>
//
//
//MyScene::MyScene(Window& window) : Scene(window)
//{
//	if (init == true)
//		init = InitialiseGL();
//
//	UpdateWorldMatrices(m_RootGameObject, Matrix4());
//
//
//	//PhysicsEngine::Instance()->SetUpdateTimestep(1.0f / 1.0f);	//1 Update per second
//
//	//PhysicsEngine::Instance()->SetUpdateTimestep(1.0f / 5.0f);	//5 Updates per second
//
//	PhysicsEngine::Instance()->SetUpdateTimestep(1.0f / 60.0f);		//60 Updates per second
//}
//
//MyScene::~MyScene()
//{
//	if (m_TargetTexture)
//	{
//		glDeleteTextures(1, &m_TargetTexture);
//		m_TargetTexture = NULL;
//	}
//}
//
//bool MyScene::InitialiseGL()
//{
//	m_Camera->SetPosition(Vector3(-6.25f, 2.0f, 10.0f));
//
//	PhysicsEngine::Instance()->SetGravity(Vector3(0.0f, 0.0f, 0.0f));		//No Gravity
//	PhysicsEngine::Instance()->SetDampingFactor(1.0f);						//No Damping
//	
//
//	m_TargetTexture = SOIL_load_OGL_texture(TEXTUREDIR"target.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
//	glBindTexture(GL_TEXTURE_2D, m_TargetTexture);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); //No linear interpolation to get crisp checkerboard no matter the scalling
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//		
//	//Create Ground
//	SimpleMeshObject* ground  = new SimpleMeshObject("Ground");
//	ground->SetMesh(CommonMeshes::Cube(), false);
//	ground->SetLocalTransform(Matrix4::Scale(Vector3(80.0f, 0.1f, 2.f)));
//	ground->SetColour(Vector4(0.2f, 1.0f, 0.5f, 1.0f));
//	ground->SetBoundingRadius(80.0f * 80.f);
//
//	ground->Physics()->SetPosition(Vector3(-6.25f, -0.2f, 0.0f));
//
//	this->AddGameObject(ground);
//	
//
//
//
//	//Create Target
//	SimpleMeshObject* target = new SimpleMeshObject("Target");
//	target->SetMesh(CommonMeshes::Cube(), false);
//	target->SetTexture(m_TargetTexture, false);
//	target->SetLocalTransform(Matrix4::Scale(Vector3(0.1f, 2.0f, 2.f)));
//	target->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
//	target->SetBoundingRadius(4.0f);
//
//	target->Physics()->SetPosition(Vector3(0.1f, 2.0f, 0.0f));
//
//	this->AddGameObject(target);
//	
//
//	//Create a projectile
//	SimpleMeshObject* m_Sphere = new SimpleMeshObject("Sphere");
//	m_Sphere->SetMesh(CommonMeshes::Sphere(), false);
//	m_Sphere->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
//	m_Sphere->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
//	m_Sphere->SetBoundingRadius(1.0f);
//
//	m_Sphere->Physics()->SetPosition(Vector3(-12.5f, 2.0f, 0.f));
//
//	m_Sphere->Physics()->SetInverseMass(1.f);
//
//	m_Sphere->Physics()->SetLinearVelocity(Vector3(0.f, 2.5f, 0.0f));
//	m_Sphere->Physics()->SetForce(Vector3(1.f, -1.f, 0.0f));
//
//	this->AddGameObject(m_Sphere);
//
//	return true;
//}
//
//void MyScene::UpdateScene(float msec)
//{
//	Scene::UpdateScene(msec);
//
//	if (!PhysicsEngine::Instance()->IsPaused())
//	{
//		m_TrajectoryPoints.push_back(m_Sphere->Physics()->GetPosition());
//	}
//
//	if (m_Sphere->Physics()->GetPosition().y < 0.0f)
//	{
//		PhysicsEngine::Instance()->SetPaused(true);
//	}
//}
//
//void MyScene::RenderScene()
//{
//	for (size_t i = 1; i < m_TrajectoryPoints.size(); i++)
//	{
//		NCLDebug::DrawHairLine(m_TrajectoryPoints[i - 1], m_TrajectoryPoints[i], Vector4(1.0f, 0.0f, 0.0f, 1.0f));
//	}
//
//	Scene::RenderScene();
//}
//
