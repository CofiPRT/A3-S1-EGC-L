#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{


	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;

		Mesh* teaMesh = new Mesh("teapot");
		teaMesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[teaMesh->GetMeshID()] = teaMesh;

		Mesh* sphereMesh = new Mesh("sphere");
		sphereMesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[sphereMesh->GetMeshID()] = sphereMesh;

		Mesh* archerMesh = new Mesh("archer");
		archerMesh->LoadMesh(RESOURCE_PATH::MODELS + "Characters\\Archer", "Archer.fbx");
		meshes[archerMesh->GetMeshID()] = archerMesh;
	}
}

void Laborator1::FrameStart()
{
	
}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glm::vec3 currColor = colors[currColorIndex];
	float red = currColor.x;
	float green = currColor.y;
	float blue = currColor.z;

	glClearColor(red, green, blue, 1);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

	UpdateRotatingObjPos(deltaTimeSeconds);

	// render the currently selected object
	RenderMesh(meshes[objectIDs[currObjectIndex]], rotatingObjPos);

	// render the character
	RenderMesh(meshes["archer"], archerPos, glm::vec3(0.005F));

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// do not interfere with the camera
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
		return;

	float distance = deltaTime * SPEED_MODIFIER;

	// X movement
	if (window->KeyHold(GLFW_KEY_A))
		archerPos -= glm::vec3(distance, 0, 0);

	if (window->KeyHold(GLFW_KEY_D))
		archerPos += glm::vec3(distance, 0, 0);

	// Y movement
	if (window->KeyHold(GLFW_KEY_Q))
		archerPos -= glm::vec3(0, distance, 0);

	if (window->KeyHold(GLFW_KEY_E))
		archerPos += glm::vec3(0, distance, 0);

	// Z movement
	if (window->KeyHold(GLFW_KEY_W))
		archerPos -= glm::vec3(0, 0, distance);

	if (window->KeyHold(GLFW_KEY_S))
		archerPos += glm::vec3(0, 0, distance);
};

void Laborator1::OnKeyPress(int key, int mods)
{
	switch (key) {
	case GLFW_KEY_RIGHT_BRACKET:
		currColorIndex = CycleInRange(currColorIndex + 1, MAX_COLORS);
		break;
	case GLFW_KEY_LEFT_BRACKET:
		currColorIndex = CycleInRange(currColorIndex - 1, MAX_COLORS);
		break;
	case GLFW_KEY_PERIOD:
		currObjectIndex = CycleInRange(currObjectIndex + 1, MAX_OBJECTS);
		break;
	case GLFW_KEY_COMMA:
		currObjectIndex = CycleInRange(currObjectIndex - 1, MAX_OBJECTS);
		break;
	}

};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}

float Laborator1::CycleInRange(float number, float end)
{
	return CycleInRange(number, 0, end);
}

// inclusive start, exclusive end
float Laborator1::CycleInRange(float number, float start, float end)
{
	if (number < start)
		return end - (start - number);

	if (number >= end)
		return start + (number - end);

	return number;
}

void Laborator1::UpdateRotatingObjPos(float deltaTime)
{
	currDegrees = CycleInRange(currDegrees + deltaTime * SPEED_MODIFIER, 360);

	glm::vec3 relativePos = glm::vec3(CIRCLE_RANGE * cos(currDegrees), 0, CIRCLE_RANGE * sin(currDegrees));

	rotatingObjPos = archerPos + relativePos;
}