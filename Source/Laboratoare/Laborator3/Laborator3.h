#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

#define DEFAULT_SQUARE_SIDE (100)

#define SQUARE_JUMP_AMPLITUDE (300)
#define SQUARE_ROTATION_SPEED (5)
#define SQUARE_BOING_COUNT (4)
#define SQUARE_FIRST_BOING_SCALE (0.4)

#define SPEED_FACTOR (2.5)

#define DEFAULT_Y (250)

#define INITIAL_SQUARE1_X (150)
#define INITIAL_SQUARE1_Y DEFAULT_Y
#define INITIAL_SQUARE2_X (400)
#define INITIAL_SQUARE2_Y DEFAULT_Y
#define INITIAL_SQUARE3_X (650)
#define INITIAL_SQUARE3_Y DEFAULT_Y

#define LEAP_DURATION (2) // deltaTimeSeconds
#define LEAP_DISTANCE (500)
#define LEAP_HEIGHT (800)

class Laborator3 : public SimpleScene
{
	public:
		Laborator3();
		~Laborator3();

		void Init() override;

	private:
		float jumpDelta = 0;

		bool currentlyLeaping = false;
		float leapDelta = 0;
		float leapingSquareCurrX = 0;

		void UpdateDeltas(float deltaTimeSeconds);
		glm::mat3 JumpyJumpMatrix();
		glm::mat3 SpinnySpinMatrix();
		glm::mat3 BoingyBoingMatrix();

		glm::mat3 UpdateFirstSquareMatrix();
		glm::mat3 UpdateSecondSquareMatrix();
		glm::mat3 UpdateThirdSquareMatrix();
		glm::mat3 UpdateLeapingSquareMatrix();

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

	protected:
		glm::mat3 modelMatrix;
		float translateX, translateY;
		float scaleX, scaleY;
		float angularStep;
};
