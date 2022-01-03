#include "Laborator5.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator5::Laborator5()
{
}

Laborator5::~Laborator5()
{
}

void Laborator5::Init()
{
	renderCameraTarget = false;

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	projectionMatrix = glm::perspective(RADIANS(currFOV), window->props.aspectRatio, 0.01f, 200.0f);
}

void Laborator5::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator5::Update(float deltaTimeSeconds)
{

	if (orthoMode) {
		projectionMatrix = glm::ortho<float>(0, currWidth, 0, currHeight, 0.01, 200);
	} else {
		projectionMatrix = glm::perspective(RADIANS(currFOV), window->props.aspectRatio, 0.01f, 200.0f);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

		// Attention! The RenderMesh function overrides the usual RenderMesh that we used until now
		// It uses the viewMatrix from Laborator::Camera instance and the local projectionMatrix
		RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
		RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
		RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
	}

	// Extra custom object 1
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(OBJECT_1_POS_X, OBJECT_1_POS_Y, OBJECT_1_POS_Z));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(OBJECT_1_ROTATION_ANGLE), OBJECT_1_ROTATION_AXIS);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(OBJECT_1_SCALE_X, OBJECT_1_SCALE_Y, OBJECT_1_SCALE_Z));
		RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	}

	// Extra custom object 2
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(OBJECT_2_POS_X, OBJECT_2_POS_Y, OBJECT_2_POS_Z));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(OBJECT_2_ROTATION_ANGLE), OBJECT_2_ROTATION_AXIS);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(OBJECT_2_SCALE_X, OBJECT_2_SCALE_Y, OBJECT_2_SCALE_Z));
		RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	}

	// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
	if (renderCameraTarget)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
	}
}

void Laborator5::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Laborator5::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator5::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sprintMultiplier = window->KeyHold(GLFW_KEY_LEFT_SHIFT) ? SPRINT_MULTIPLIER : 1;

		// MOVEMENT
		if (window->KeyHold(GLFW_KEY_W)) {
			camera->TranslateForward(deltaTime * CAMERA_SPEED * sprintMultiplier);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-deltaTime * CAMERA_SPEED * sprintMultiplier);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			camera->TranslateForward(-deltaTime * CAMERA_SPEED * sprintMultiplier);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			camera->TranslateRight(deltaTime * CAMERA_SPEED * sprintMultiplier);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpword(-deltaTime * CAMERA_SPEED * sprintMultiplier);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpword(deltaTime * CAMERA_SPEED * sprintMultiplier);
		}

	}

	if (orthoMode) {

		// CHANGE ORTHO SIZES
		if (window->KeyHold(GLFW_KEY_UP)) {
			currHeight += ORTHO_CHANGE_SPEED * deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_DOWN)) {
			currHeight -= ORTHO_CHANGE_SPEED * deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_LEFT)) {
			currWidth -= ORTHO_CHANGE_SPEED * deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_RIGHT)) {
			currWidth += ORTHO_CHANGE_SPEED * deltaTime;
		}

	} else {

		// CHANGE FOV
		if (window->KeyHold(GLFW_KEY_RIGHT_BRACKET)) {
			currFOV += FOV_CHANGE_SPEED * deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_LEFT_BRACKET)) {
			currFOV -= FOV_CHANGE_SPEED * deltaTime;
		}

	}
	
}

void Laborator5::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}

	if (key == GLFW_KEY_O)
	{
		orthoMode = true;
	}

	if (key == GLFW_KEY_P)
	{
		orthoMode = false;
	}

}

void Laborator5::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (renderCameraTarget) {

			camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
			camera->RotateThirdPerson_OY(-deltaX * sensivityOY);

		} else {

			camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOY);

		}

	}
}

void Laborator5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator5::OnWindowResize(int width, int height)
{
}
