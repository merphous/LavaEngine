#include "Application.h"
#include "WindowManager.h"
#include "../Platforms/OpenGL/GLRenderer.h"
#include "../Platforms/OpenGL/GLBatchedRenderer.h"
#include "../Platforms/OpenGL/GLRenderObject.h"
#include "../Renderer/Mesh.h"
#include "../Platforms/OpenGL/GLShader.h"
#include "../Platforms/OpenGL/GLShaderBank.h"
#include "../Core/AssetDatabase.h"
#include "../Components/Entity.h"
#include "../Components/Camera.h"
#include <iterator>
#include <vector>
#include <iostream>
#include "../Utils/ObjImporter.h"
#include "../Utils/AssetImporter.h"
#include "LavaTime.h"
#include <time.h>
namespace Lava {
	int GetRandom() {
		return rand() % 100;
	}

	void Application::Run() {
		WindowManager manager;
		if (manager.GenerateWindow() == -1)
			return;

		//OpenGL::GLRenderer renderer = OpenGL::GLRenderer(std::vector<Lava::OpenGL::GLShader*>());
		OpenGL::GLBatchedRenderer _renderer = OpenGL::GLBatchedRenderer(std::vector<Lava::OpenGL::GLShader*>());

		std::vector<VertexBufferElement> bufferElements(3);
		bufferElements[0].uniform_name = "position";
		bufferElements[0].uniform_count = 3;
		bufferElements[1].uniform_name = "texCoord";
		bufferElements[1].uniform_count = 2;
		bufferElements[2].uniform_name = "normal";
		bufferElements[2].uniform_count = 3;

		auto pack = Lava::Importers::AssetImporter::Load("Assets/Zombie9.obj");
		auto texture = AssetDatabase::LoadTexture("Assets/Zombie9_CT.jpg");

		std::vector<Entity*> entities;
		MeshRenderer* batchableRenderer = nullptr;
		for (int a = 0; a < 100; a++) {
			Entity* entity = new Entity(glm::vec3(0., -.5, 0), pack);
			entity->SetBufferLayout(bufferElements);
			entity->material->m_mainTexture = &texture;
			if (!batchableRenderer)
				batchableRenderer = entity->GetMeshRenderer(Platform::OpenGL);
			else
				entity->meshRenderer = batchableRenderer;
			entities.push_back(entity);
		}

		srand(time(0));
		Camera camera;

		Light light = Light(glm::vec3(1, 1, 1));
		bool onStartExecuted = false;

		while (!manager.IsWindowClosed()) {
			Time::CalculateDeltaTime();
			if (!onStartExecuted) {
				for (auto& eachEntity : entities) {
					for (int i = 0; i < 3; i++) {
						
						float randPos = GetRandom() / 50.f;
						eachEntity->transform->Position[i] = randPos;
					}
				}
				onStartExecuted = true;
			}
			for (auto& entity : entities) {
				_renderer.AddToBatch(entity);
			}

			if (glfwGetKey(manager.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
			{
				camera.transform.Position.z -= Time::deltaTime;
				std::cout << Time::deltaTime << std::endl;
			}

			if (glfwGetKey(manager.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
			{
				camera.transform.Position.z +=  Time::deltaTime;
			}

			if (glfwGetKey(manager.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) {
				camera.transform.Position.x -= Time::deltaTime;
			}

			if (glfwGetKey(manager.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) {
				camera.transform.Position.x +=  Time::deltaTime;
			}

			if (glfwGetKey(manager.GetWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
				camera.transform.Position.y -=  Time::deltaTime;
			}

			if (glfwGetKey(manager.GetWindow(), GLFW_KEY_E) == GLFW_PRESS) {
				camera.transform.Position.y +=  Time::deltaTime;
			}

			if (glfwGetKey(manager.GetWindow(), GLFW_KEY_KP_4) == GLFW_PRESS) {
				light.Position.x -=  Time::deltaTime;
			}


			if (glfwGetKey(manager.GetWindow(), GLFW_KEY_KP_6) == GLFW_PRESS) {
				light.Position.x +=  Time::deltaTime;
			}


			if (glfwGetKey(manager.GetWindow(), GLFW_KEY_KP_8) == GLFW_PRESS) {
				light.Position.y +=  Time::deltaTime;
			}


			if (glfwGetKey(manager.GetWindow(), GLFW_KEY_KP_2) == GLFW_PRESS) {
				light.Position.y -=  Time::deltaTime;
			}

			_renderer.Update(camera, light);

			manager.UpdateWindow();
		}
		manager.DestroyWindow();
		//_renderer.~GLBatchedRenderer();
	}
}