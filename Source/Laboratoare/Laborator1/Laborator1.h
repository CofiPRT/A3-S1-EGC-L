#pragma once

#include <Component/SimpleScene.h>
#include <string>

#define MAX_COLORS 4
#define MAX_OBJECTS 3
#define SPEED_MODIFIER 1
#define CIRCLE_RANGE 2

class Laborator1 : public SimpleScene
{
	public:
		Laborator1();
		~Laborator1();

		void Init() override;

	private:
		int currColorIndex = 0;
		int currObjectIndex = 0;
		float currDegrees = 0;

		glm::vec3 colors[MAX_COLORS] = {
			glm::vec3(0, 0, 0),		// BLACK
			glm::vec3(255, 0, 0),	// RED
			glm::vec3(0, 255, 0),	// BLUE
			glm::vec3(0, 0, 255)	// GREEN
		};

		std::string objectIDs[MAX_OBJECTS] = {
			"box", "teapot", "sphere"
		};

		glm::vec3 rotatingObjPos = glm::vec3(0, 0, 0);
		glm::vec3 archerPos = glm::vec3(0, 0, 0);

		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		float CycleInRange(float number, float end);
		float CycleInRange(float number, float start, float end);

		void UpdateRotatingObjPos(float deltaTime);
};
