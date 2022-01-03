#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

#define COLOR_WHITE				(glm::vec3(1))
#define COLOR_GRAY				(glm::vec3(0.5))
#define COLOR_LIGHT_GRAY		(glm::vec3(0.75))
#define COLOR_DARK_GRAY			(glm::vec3(0.25))
#define COLOR_BLACK				(glm::vec3(0))
#define COLOR_RED				(glm::vec3(1, 0, 0))
#define COLOR_GREEN				(glm::vec3(0, 1, 0))
#define COLOR_BLUE				(glm::vec3(0, 0, 1))
#define COLOR_YELLOW			(glm::vec3(1, 1, 0))
#define COLOR_BROWN				(glm::vec3(0.6, 0.4, 0.2))
#define COLOR_DARK_BROWN		(glm::vec3(0.8, 0.6, 0.4))
#define COLOR_WASHED_GREEN		(glm::vec3(0.55, 0.7, 0.5))
#define COLOR_DARK_RED			(glm::vec3(0.55, 0, 0))
#define COLOR_MUSTARD_YELLOW	(glm::vec3(0.7, 0.7, 0))

class Laborator7 : public SimpleScene
{
	public:
		Laborator7();
		~Laborator7();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		glm::vec3 lightPosition;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
};
