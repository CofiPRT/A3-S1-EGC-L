#include "Laborator3.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

void Laborator3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = DEFAULT_SQUARE_SIDE;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;
	
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;
	
	// initialize angularStep
	angularStep = 0;
	

	Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);
	
	Mesh* square2 = Object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
	AddMeshToList(square2);

	Mesh* square3 = Object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
	AddMeshToList(square3);

	Mesh* leapingSquare = Object2D::CreateSquare("leapingSquare", corner, squareSide, glm::vec3(1, 1, 1), true);
	AddMeshToList(leapingSquare);
}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator3::UpdateDeltas(float deltaTimeSeconds)
{
	jumpDelta += SPEED_FACTOR * deltaTimeSeconds;

	if (jumpDelta > M_PI)
		jumpDelta -= M_PI;

	if (currentlyLeaping)
		leapDelta += deltaTimeSeconds;
}

glm::mat3 Laborator3::JumpyJumpMatrix()
{
	float distY = abs(SQUARE_JUMP_AMPLITUDE * sin(jumpDelta));

	return Transform2D::Translate(0, distY);
}

glm::mat3 Laborator3::SpinnySpinMatrix()
{
	// 360 rotation
	// float funcScale = M_PI / atan(M_PI * SQUARE_ROTATION_SPEED / 2);
	// float radians = funcScale * atan(SQUARE_ROTATION_SPEED * (jumpDelta - M_PI_2)) + M_PI;

	// 180 rotation
	float funcScale = M_PI / (2 * atan(M_PI * SQUARE_ROTATION_SPEED / 2));
	float radians = funcScale * atan(SQUARE_ROTATION_SPEED * (jumpDelta - M_PI_2)) + M_PI_2;

	return Transform2D::Translate(DEFAULT_SQUARE_SIDE / 2, DEFAULT_SQUARE_SIDE / 2) *
		Transform2D::Rotate(radians) *
		Transform2D::Translate(-DEFAULT_SQUARE_SIDE / 2, -DEFAULT_SQUARE_SIDE / 2);
}

glm::mat3 Laborator3::BoingyBoingMatrix()
{
	float mainFunc = SQUARE_FIRST_BOING_SCALE * sin(pow(SQUARE_BOING_COUNT, 2) * (jumpDelta + M_PI_2));
	float controlFunc = jumpDelta + M_PI_2;
	float limitFunc = max((double) 0, M_PI_2 - jumpDelta);

	float scaleFactor = mainFunc / controlFunc * limitFunc + 1;

	return Transform2D::Translate(DEFAULT_SQUARE_SIDE / 2, DEFAULT_SQUARE_SIDE / 2) *
		Transform2D::Scale(scaleFactor, scaleFactor) *
		Transform2D::Translate(-DEFAULT_SQUARE_SIDE / 2, -DEFAULT_SQUARE_SIDE / 2);
}

glm::mat3 Laborator3::UpdateFirstSquareMatrix()
{
	glm::mat3 matrix = Transform2D::Translate(INITIAL_SQUARE1_X, INITIAL_SQUARE1_Y);
	
	matrix *= JumpyJumpMatrix();

	return matrix;
}

glm::mat3 Laborator3::UpdateSecondSquareMatrix()
{
	glm::mat3 matrix = Transform2D::Translate(INITIAL_SQUARE2_X, INITIAL_SQUARE2_Y);

	matrix *= JumpyJumpMatrix();
	matrix *= SpinnySpinMatrix();

	return matrix;
}

glm::mat3 Laborator3::UpdateThirdSquareMatrix()
{
	glm::mat3 matrix = Transform2D::Translate(INITIAL_SQUARE3_X, INITIAL_SQUARE3_Y);

	matrix *= JumpyJumpMatrix();
	matrix *= SpinnySpinMatrix();
	matrix *= BoingyBoingMatrix();

	return matrix;
}

glm::mat3 Laborator3::UpdateLeapingSquareMatrix()
{
	glm::mat3 matrix = Transform2D::Translate(leapingSquareCurrX, 0);

	if (!currentlyLeaping)
		return matrix;

	if (LEAP_DURATION == 0) {
		// in an instant
		leapingSquareCurrX += LEAP_DISTANCE;
		return Transform2D::Translate(leapingSquareCurrX, 0);
	}

	if (leapDelta >= LEAP_DURATION) {
		currentlyLeaping = false;
		leapDelta = 0;

		leapingSquareCurrX += LEAP_DISTANCE;
		return Transform2D::Translate(leapingSquareCurrX, 0);
	}

	float translateX = LEAP_DISTANCE / LEAP_DURATION * leapDelta;
	
	float a = 4 * LEAP_HEIGHT / pow(LEAP_DISTANCE, 2);
	float b = LEAP_DISTANCE / 2;
	float c = LEAP_HEIGHT;
	float translateY = -a * pow(translateX - b, 2) + c;

	matrix *= Transform2D::Translate(translateX, translateY);

	float radians = -M_PI / LEAP_DURATION * leapDelta;

	matrix *= Transform2D::Translate(DEFAULT_SQUARE_SIDE / 2, DEFAULT_SQUARE_SIDE / 2) *
		Transform2D::Rotate(radians) *
		Transform2D::Translate(-DEFAULT_SQUARE_SIDE / 2, -DEFAULT_SQUARE_SIDE / 2);

	return matrix;
}

void Laborator3::Update(float deltaTimeSeconds)
{	
	UpdateDeltas(deltaTimeSeconds);

	RenderMesh2D(meshes["square1"], shaders["VertexColor"], UpdateFirstSquareMatrix());
	RenderMesh2D(meshes["square2"], shaders["VertexColor"], UpdateSecondSquareMatrix());
	RenderMesh2D(meshes["square3"], shaders["VertexColor"], UpdateThirdSquareMatrix());
	RenderMesh2D(meshes["leapingSquare"], shaders["VertexColor"], UpdateLeapingSquareMatrix());
}

void Laborator3::FrameEnd()
{

}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
	
}

void Laborator3::OnKeyPress(int key, int mods)
{
	switch (key) {
	case GLFW_KEY_SPACE:
		currentlyLeaping = true;
		break;
	case GLFW_KEY_R:
		currentlyLeaping = false;
		leapDelta = 0;
		leapingSquareCurrX = 0;
		break;
	}
}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}
