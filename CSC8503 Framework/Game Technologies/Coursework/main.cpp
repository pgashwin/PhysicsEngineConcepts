#include <nclgl\Window.h>
#include "MyScene.h"
#include <ncltech\PhysicsEngine.h>
#include <ncltech\NCLDebug.h>

Scene* scene = NULL;

int Quit(bool pause = false, const string &reason = "") {
	if (scene)
	{
		delete scene;
		scene = NULL;
	}

	Window::Destroy();

	if (pause) {
		std::cout << reason << std::endl;
		system("PAUSE");
	}

	return 0;
}

int main()
{
	//-------------------
	//--- MAIN ENGINE ---
	//-------------------

	//Initialise the Window
	if (!Window::Initialise("Coursework - Ashwin", 1280, 800, false))
	{
		return Quit(true, "Window failed to initialise!");
	}

	//Initialise the PhysicsEngine
	PhysicsEngine::Instance();
	PhysicsEngine::Instance()->SetUpdateTimestep(1.0f / 120.f);

	//Initialise the Scene
	scene = new MyScene(Window::GetWindow());
	if (!scene->HasInitialised())
	{
		return Quit(true, "Renderer failed to initialise!");
	}

	GameTimer engine_timer;

	//Create main game-loop
	while (Window::GetWindow().UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		//Note that for the sake of simplicity, all calculations with time will be done in seconds (ms * 0.001)
		// this is to simplify physics, as I cant visualise how fast 0.02 meters per millisecond is.
		float dt = Window::GetWindow().GetTimer()->GetTimedMS() * 0.001f;	//How many milliseconds since last update?

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
			PhysicsEngine::Instance()->SetPaused(!PhysicsEngine::Instance()->IsPaused());

	


		engine_timer.GetTimedMS();

		PhysicsEngine::Instance()->Update(dt);
		float physics_ms = engine_timer.GetTimedMS();

		scene->UpdateScene(dt);
		float update_ms = engine_timer.GetTimedMS();

		
		
		

		

		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "Physics Engine: %s (Press P to toggle)", PhysicsEngine::Instance()->IsPaused() ? "Paused" : "Enabled");
		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "--------------------------------");
		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "Physics Timestep : %5.2fms (%5.2f FPS)", PhysicsEngine::Instance()->GetUpdateTimestep() * 1000.0f, 1.0f / PhysicsEngine::Instance()->GetUpdateTimestep());
		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "Graphics Timestep: %5.2fms (%5.2f FPS)", dt * 1000.0f, 1.0f / dt);
		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "--------------------------------");
		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "Physics Update: %5.2fms", physics_ms);
		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "Scene Update  : %5.2fms", update_ms);
		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "J: Shoot Balls");
		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "H:Generate cubes"); 
		NCLDebug::AddStatusEntry(Vector4(1.0f, 1.0f, 1.0f, 1.0f), "X:Exit");




		
		
	

		scene->RenderScene();
	}



	//Cleanup
	return Quit();
}//
