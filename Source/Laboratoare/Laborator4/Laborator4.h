#pragma once
#include <Component/SimpleScene.h>

#include <Core/GPU/Mesh.h>

#define BOX_INITIAL_X (0)
#define BOX_INITIAL_Y (0)
#define BOX_INITIAL_Z (0)

#define SECOND_BOX_ORBIT_RANGE (4)
#define THIRD_BOX_ORBIT_RANGE (2)

#define SECOND_BOX_ORBIT_SPEED (1)
#define THIRD_BOX_ORBIT_SPEED (5)

#define TETRAHEDRON_INITIAL_X (0)
#define TETRAHEDRON_INITIAL_Y (2)
#define TETRAHEDRON_INITIAL_Z (0)
#define TETRAHEDRON_FLOAT_SPEED (2)

class Laborator4 : public SimpleScene
{
	public:
		Laborator4();
		~Laborator4();

		void Init() override;

	private:
		float elapsedTime = 0;

		glm::mat4 GetFirstBoxPosition();
		glm::mat4 GetRotatingPosition(glm::mat4 origin, float range, float speed);
		glm::mat4 GetChaoticFloatingPosition();

		Mesh* CreateTetrahedron(const char* name, double scale);
		Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);

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
		glm::mat4 modelMatrix;
		float translateX, translateY, translateZ;
		float scaleX, scaleY, scaleZ;
		float angularStepOX, angularStepOY, angularStepOZ;
		GLenum polygonMode;
};
