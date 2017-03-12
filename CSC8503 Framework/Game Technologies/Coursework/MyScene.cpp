#include "MyScene.h"
#include "Player.h"
#define MSGBOX(x)
#include <windows.h>

#include <nclgl/OBJMesh.h>

#include <ncltech\SimpleMeshObject.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\CommonMeshes.h>
#include <ncltech\NCLDebug.h>
#include<ncltech\CuboidCollisionShape.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\Constraint.h>
#include<fstream>


MyScene::MyScene(Window& window) : Scene(window)
{
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

void MyScene::tardisOpacity(float msec){
	
}

bool MyScene::InitialiseGL()
{
	m_Camera->SetPosition(Vector3(-6.25f, 2.0f, 10.0f));

	PhysicsEngine::Instance()->SetGravity(Vector3(0.0f, -9.81f, 0.0f));		//No Gravity
	PhysicsEngine::Instance()->SetDampingFactor(1.0f);						//No Damping



	//Create Ground
	SimpleMeshObject* ground = new SimpleMeshObject("Ground");
	ground->SetMesh(CommonMeshes::Cube(), false);
	ground->SetLocalTransform(Matrix4::Translation(Vector3(0.0, 0.0, 0.0f)) * Matrix4::Scale(Vector3(20.0f, 0.5f, 20.f))); 
	ground->SetColour(Vector4(0.5f, 0.7f, 1.0f, 1.0f));  
	ground->SetBoundingRadius(80.0f * 80.f);
	ground->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(20.0f, 0.5f, 20.f))));

	this->AddGameObject(ground);



	//Create a Sphere
	for (int i = 0; i < 5; i++){
	SimpleMeshObject* sphere = new SimpleMeshObject("Sphere");
	sphere->SetMesh(CommonMeshes::Sphere(), false);
	sphere->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
	sphere->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
	sphere->SetBoundingRadius(1.0f);
	sphere->Physics()->SetInverseMass(1.f);
	sphere->Physics()->SetCollisionShape(new SphereCollisionShape(0.5f));
	sphere->Physics()->SettoRest(true);

	sphere->Physics()->SetPosition(Vector3(rand() % 20, 5.0f, rand() % 20));
	this->AddGameObject(sphere);
}



	//Create a Cuboid
	for (int i = 0; i < 5; i++){
		SimpleMeshObject* pyramid = new SimpleMeshObject("Cube");
		pyramid->SetMesh(CommonMeshes::pyramid(), false);
		pyramid->SetLocalTransform(Matrix4::Translation(Vector3(0.0f, 0.27f, 0.0f))*Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f))* Matrix4::Rotation(270, Vector3(1.0f, 0.0f, 0.0f)));	
		pyramid->SetColour(Vector4(0.2f, 0.5f, 1.0f, 1.0f));
		
		pyramid->Physics()->SetInverseMass(1.f);
		pyramid->Physics()->SetPosition(Vector3(rand() % 20, 5.0f, rand() % 20));   
		pyramid->Physics()->SetCollisionShape(new PyramidCollisionShape((Vector3(0.25f, 0.25f, 0.25f))));
		pyramid->Physics()->SetInverseInertia(pyramid->Physics()->GetCollisionShape()->BuildInverseInertia(1.4f));
		pyramid->Physics()->SettoRest(true);


		this->AddGameObject(pyramid);
	}

	//Create cube
	SimpleMeshObject* cube1 = new SimpleMeshObject("Cube");
	cube1->SetMesh(CommonMeshes::Cube(), false);
	cube1->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));	//1m x 1m x 1m
	cube1->SetColour(Vector4(0.2f, 0.5f, 1.0f, 1.0f));
	cube1->SetBoundingRadius(1.5f);

	cube1->Physics()->SetPosition(Vector3(10.5f, 1.0f, 0.0f));
	cube1->Physics()->SetAngularVelocity(Vector3(0.0f, 4.0f, 0.0f));
	cube1->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(0.5f, 0.5f, 0.5f))));
	
	this->AddGameObject(cube1);


	
	//Create a 'Player'
	Player* player = new Player("Player1");
	player->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	player->SetBoundingRadius(2.0f);
	player->Physics()->SetInverseMass(6.0f);

	player->Physics()->SetPosition(Vector3(0.0f, 1.0f, -10.0f));

	this->AddGameObject(player);





	//Loop effect
	//Create holder
	SimpleMeshObject* holder = new SimpleMeshObject("holder");
	holder->SetMesh(CommonMeshes::Cube(), false);
	holder->SetTexture(m_TargetTexture, false);
	holder->SetLocalTransform(Matrix4::Scale(Vector3(0.1f, 0.1f, 0.1f)));
	holder->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	holder->SetBoundingRadius(4.0f);
	holder->Physics()->SetAngularVelocity(Vector3(0.0f, 4.0f, 0.0f));

	holder->Physics()->SetPosition(Vector3(10.5f, 5.0f, 0.0f));

	this->AddGameObject(holder);


	
	SimpleMeshObject* hangingBall1 = new SimpleMeshObject("hangingBall");
	hangingBall1->SetMesh(CommonMeshes::Sphere(), false);
	hangingBall1->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
	hangingBall1->SetColour(Vector4(1.0f, 0.2f, 0.5f, 1.0f));
	hangingBall1->SetBoundingRadius(1.0f);
	hangingBall1->Physics()->SetLinearVelocity(Vector3(4.0f, 0.0f, -4.0f));

	hangingBall1->Physics()->SetPosition(Vector3(12.0f, 5.0f, 0.f));
	hangingBall1->Physics()->SetInverseMass(1.f);
	hangingBall1->Physics()->SetCollisionShape(new SphereCollisionShape(0.5f));
	

	this->AddGameObject(hangingBall1);

	DistanceConstraint* pendulum_holder1 = new DistanceConstraint(holder->Physics(), hangingBall1->Physics(), holder->Physics()->GetPosition(), hangingBall1->Physics()->GetPosition());
	PhysicsEngine::Instance()->AddConstraint(pendulum_holder1);




	//quad
	//Create Target
	SimpleMeshObject* target = new SimpleMeshObject("Target");
	target->SetMesh(CommonMeshes::Cube(), false);
	target->SetTexture(m_TargetTexture, false);
	target->SetLocalTransform(Matrix4::Scale(Vector3(0.2f, 5.0f, 5.f)));
	target->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	target->SetBoundingRadius(4.0f);
	target->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(0.2f, 5.0f, 5.f))));

	target->Physics()->SetPosition(Vector3(-20.0f, 5.0f, -16.0f));

	this->AddGameObject(target);


	SimpleMeshObject* target3 = new SimpleMeshObject("Target3");
	target3->SetMesh(CommonMeshes::Cube(), false);
	target3->SetTexture(m_TargetTexture, false);
	target3->SetLocalTransform(Matrix4::Scale(Vector3(0.2f, 5.0f, 5.f)));
	target3->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	target3->SetBoundingRadius(4.0f);
	target3->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(0.2f, 5.0f, 5.f))));

	target3->Physics()->SetPosition(Vector3(-10.0f, 5.0f, -16.0f));

	this->AddGameObject(target3);



	SimpleMeshObject* target1 = new SimpleMeshObject("Target1");
	target1->SetMesh(CommonMeshes::Cube(), false);
	target1->SetTexture(m_TargetTexture, false);
	target1->SetLocalTransform(Matrix4::Scale(Vector3(5.f, 5.0f, 0.2f)));
	target1->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	target1->SetBoundingRadius(4.0f);
	target1->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(5.f, 5.0f, 0.2f))));

	target1->Physics()->SetPosition(Vector3(-15.0f, 5.f, -19.8f));

	this->AddGameObject(target1);


	SimpleMeshObject* target2 = new SimpleMeshObject("Target2");
	target2->SetMesh(CommonMeshes::Cube(), false);
	target2->SetTexture(m_TargetTexture, false);
	target2->SetLocalTransform(Matrix4::Scale(Vector3(5.f, 0.2f, 5.0f)));
	target2->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	target2->SetBoundingRadius(4.0f);
	target2->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(5.f, 0.2f, 5.0f))));

	target2->Physics()->SetPosition(Vector3(-15.0f, 10.0f, -16.0f));

	this->AddGameObject(target2);
	//

	//tardis
	
	
	//time += msec;
	//float alpha = abs(sin(time));
	SimpleMeshObject* tardis = new SimpleMeshObject("taridis");
	tardis->SetMesh(CommonMeshes::tardis(), false);
	tardis->SetTexture(m_TargetTexture, false);
	tardis->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f)));
	tardis->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1));

	tardis->SetBoundingRadius(4.0f);
	tardis->Physics()->SetAngularVelocity(Vector3(0.0f, 4.0f, 0.0f));
	tardis->Physics()->SetPosition(Vector3(-14.0f, 0.5f, -13.f));
	this->AddGameObject(tardis);
	
	//tardis->Physics()->Setscore(true);

	//score
	std::function<bool(PhysicsObject*)> collision_callback = [&](PhysicsObject* otherObj)
	{
		SphereCollisionShape* colSphere = dynamic_cast<SphereCollisionShape*>(otherObj->GetCollisionShape());
		if (colSphere != NULL)
		{
			m_Score += 1000;
		
		}
		
		return true; //Handle the collision
	};

	SimpleMeshObject* tardis1 = new SimpleMeshObject("taridis");
	tardis1->Physics()->SetPosition(Vector3(-14.0f, 1.74f, -13.f));
	tardis1->Physics()->SetAngularVelocity(Vector3(0.0f, 4.0f, 0.0f));
	tardis1->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(0.72f, 1.2f, 0.72f))));
	
	tardis1->Physics()->SetOnCollisionCallback(collision_callback);

	SimpleMeshObject* tardis2 = new SimpleMeshObject("taridis");
	tardis2->Physics()->SetPosition(Vector3(-14.0f, 3.0f, -13.f));
	tardis2->Physics()->SetAngularVelocity(Vector3(0.0f, 4.0f, 0.0f));
	tardis2->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(0.63f, 0.04f, 0.63f))));
	
	tardis2->Physics()->SetOnCollisionCallback(collision_callback);

	SimpleMeshObject* tardis3 = new SimpleMeshObject("taridis");
	tardis3->Physics()->SetPosition(Vector3(-14.0f, 3.13f, -13.f));
	tardis3->Physics()->SetAngularVelocity(Vector3(0.0f, 4.0f, 0.0f));
	tardis3->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(0.59f, 0.08f, 0.59f))));
	
	tardis3->Physics()->SetOnCollisionCallback(collision_callback);

	SimpleMeshObject* tardis5 = new SimpleMeshObject("taridis");
	tardis5->Physics()->SetPosition(Vector3(-14.0f, 0.5f, -13.f));
	tardis5->Physics()->SetAngularVelocity(Vector3(0.0f, 4.0f, 0.0f));
	tardis5->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(0.80f, 0.06f, 0.80f))));
	
	tardis5->Physics()->SetOnCollisionCallback(collision_callback);

	SimpleMeshObject* tardis4 = new SimpleMeshObject("taridis");
	tardis4->Physics()->SetPosition(Vector3(-14.0f, 3.4f, -13.f));
	tardis4->Physics()->SetAngularVelocity(Vector3(0.0f, 4.0f, 0.0f));
	tardis4->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(0.1f, 0.14f, 0.1f))));
	tardis4->Physics()->SetOnCollisionCallback(collision_callback);
	
	

	//AI

	SimpleMeshObject* gk = new SimpleMeshObject("gk");
	gk->SetMesh(CommonMeshes::Cube(), false);
	gk->SetTexture(m_TargetTexture, false);
	gk->SetLocalTransform(Matrix4::Scale(Vector3(1.f, 1.0f, 0.2f)));
	gk->SetColour(Vector4(10.0f, 10.f, 1.0f, 1.0f));
	gk->SetBoundingRadius(4.0f);
	gk->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(1.f, 1.0f, 0.2f))));
	gk->Physics()->SetInverseMass(1.0f);

	gk->Physics()->SetPosition(Vector3(-15.0f, 1.0f, -9.0f));

	this->AddGameObject(gk);

	DistanceConstraint* gk_holder1 = new DistanceConstraint(target2->Physics(), gk->Physics(), target2->Physics()->GetPosition() + Vector3(0.5f, -4.f, -2.f), gk->Physics()->GetPosition());
	PhysicsEngine::Instance()->AddConstraint(gk_holder1);

	return true;
}

void MyScene::UpdateScene(float msec)
{
	Scene::UpdateScene(msec);
	
	NCLDebug::AddStatusEntry(Vector4(20.0f, 20.0f, 1.0f, 1.0f), "New SCORE: %d", m_Score);
	/*
	Game Update Logic
	*/
	//Shooting spheres from the camera
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J)){
		
			SimpleMeshObject* m_Sphere1 = new SimpleMeshObject("Sphere");
		
			m_Sphere1->SetMesh(CommonMeshes::texturedSpehre(), false);
			m_Sphere1->SetLocalTransform(Matrix4::Scale(Vector3(0.18f, 0.18f, 0.18f)));
			m_Sphere1->SetColour(Vector4(1.0f, 0.9f, 0.8f, 1.0f));
			m_Sphere1->SetBoundingRadius(1.0f);
			m_Sphere1->Physics()->SetCollisionShape(new SphereCollisionShape(0.18f));

			m_Sphere1->Physics()->SetPosition(m_Camera->GetPosition());

			m_Sphere1->Physics()->SetInverseMass(1.f);

			Matrix3 viewRotation = Matrix3(viewMatrix);
			viewRotation = Matrix3::Inverse(viewRotation); //Only needed for viewmatrix as viewmatrix is an inverse rotation.

			Vector3 forward = viewRotation * Vector3(0, 0, -1);

			m_Sphere1->Physics()->SetLinearVelocity(forward * 30.0f);
					
			//AI
			GameObject* temp = this->FindGameObject("gk");			

			//if (m_Sphere1->Physics()->GetLinearVelocity != Vector3(0, 0, 0))
			/*temp->Physics()->SetPosition(Vector3(-15.0f, 5.0f, -11.0f));*/
				forward.Normalise();
				
				temp->Physics()->SetLinearVelocity(-forward * 10);
				
				this->AddGameObject(m_Sphere1);
				//

	}

		

	
	//Shooting cube from the camera
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_H)){
		for (int i = 0; i < 5; i++){
			SimpleMeshObject* cube = new SimpleMeshObject("Pyramid");
			cube->SetMesh(CommonMeshes::Cube(), false);
			cube->SetLocalTransform(Matrix4::Scale(Vector3(0.5f, 0.5f, 0.5f))/** Matrix4::Rotation(90, Vector3(4.0f, 4.0f, 4.0f))*/);	//1m x 1m x 1m
			cube->SetColour(Vector4(0.2f, 0.5f, 1.0f, 1.0f));
			cube->SetBoundingRadius(1.5f);
			cube->Physics()->SetInverseMass(1.f);


			/*cube->Physics()->SetAngularVelocity(Vector3(2.0f, 4.0f, 2.0f));*/

			cube->Physics()->SetPosition(Vector3(rand()%20,5.0f,rand()%20));   //11.0f, 5.0f, 0.0f)
			cube->Physics()->SetCollisionShape(new CuboidCollisionShape((Vector3(0.5f, 0.5f, 0.5f))));
			cube->Physics()->SetInverseInertia(cube->Physics()->GetCollisionShape()->BuildInverseInertia(1.4f));
			cube->Physics()->SettoRest(true);

			Matrix3 viewRotation = Matrix3(viewMatrix);
			viewRotation = Matrix3::Inverse(viewRotation); //Only needed for viewmatrix as viewmatrix is an inverse rotation.

			Vector3 forward = viewRotation * Vector3(0, 0, -1);
			cube->Physics()->SetLinearVelocity(forward * 30.0f);


			this->AddGameObject(cube);
		}
	}

	//Quit the program
	std::stringstream buffer;
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_X)){
		string x;
		ofstream myfile("Input.txt");
		if (myfile.is_open())
			cout << "Enter Your name";
		cin >> x;
		myfile << "1:" << x << "=" << m_Score;
		

		
	
	std::ifstream file("Input.txt");
	std::stringstream buffer;

	if (file)
	{
		

		buffer << file.rdbuf();

		file.close();



		}
	
		MessageBox(NULL, /*buffer.str().c_str()*/"High score added to \"Input.txt\"", "Thanks for playing", MB_OK);
		exit(0);
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
}

