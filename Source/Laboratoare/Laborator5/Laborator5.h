#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"

#define CAMERA_SPEED (2)
#define SPRINT_MULTIPLIER (4)

#define FOV_CHANGE_SPEED (50)
#define ORTHO_CHANGE_SPEED (4)

#define DEFAULT_FOV (60)
#define DEFAULT_WIDTH (10)
#define DEFAULT_HEIGHT (10)

#define OBJECT_1_POS_X (2)
#define OBJECT_1_POS_Y (2)
#define OBJECT_1_POS_Z (4)
#define OBJECT_1_ROTATION_ANGLE (30)
#define OBJECT_1_ROTATION_AXIS (OY_AXIS)
#define OBJECT_1_SCALE_X (1)
#define OBJECT_1_SCALE_Y (2)
#define OBJECT_1_SCALE_Z (2)

#define OBJECT_2_POS_X (2)
#define OBJECT_2_POS_Y (2)
#define OBJECT_2_POS_Z (-4)
#define OBJECT_2_ROTATION_ANGLE (-30)
#define OBJECT_2_ROTATION_AXIS (OZ_AXIS)
#define OBJECT_2_SCALE_X (0.5)
#define OBJECT_2_SCALE_Y (4)
#define OBJECT_2_SCALE_Z (0.5)

class Laborator5 : public SimpleScene
{
	public:
		Laborator5();
		~Laborator5();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Laborator::Camera *camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;

		float currFOV = DEFAULT_FOV;
		float currWidth = DEFAULT_WIDTH;
		float currHeight = DEFAULT_HEIGHT;

		bool orthoMode = false;
};
