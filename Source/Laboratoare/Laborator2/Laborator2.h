#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>

#define MIN_DISK_TRIANGLE_COUNT (4)
#define DEFAULT_DISK_TRIANGLE_COUNT (24)
#define MAX_DISK_TRIANGLE_COUNT (INT_MAX)

class Laborator2 : public SimpleScene
{
	public:
		Laborator2();
		~Laborator2();

		void Init() override;

	private:
		unsigned int diskTriangleCount = DEFAULT_DISK_TRIANGLE_COUNT;

		Mesh* CreateDisk(const char* name);
		Mesh* CreateDisk(const char* name, double range);
		Mesh* CreateHalfVisibleSquare(const char* name, double scale);
		Mesh* CreateHalfVisibleSquare(const char* name);
		Mesh* CreateTetrahedron(const char* name, double scale);
		Mesh* CreateTetrahedron(const char* name);
		Mesh* CreateMesh(const char *name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);

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
		GLenum cullFace;
		GLenum polygonMode;
};
