#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	Mesh* tetrahedron = CreateTetrahedron("tetrahedron0", 0.25);

	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
	
	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

glm::mat4 Laborator4::GetFirstBoxPosition()
{
	glm::mat4 matrix = glm::mat4(1);
	
	matrix *= Transform3D::Translate(BOX_INITIAL_X, BOX_INITIAL_Y, BOX_INITIAL_Z);
	matrix *= Transform3D::Translate(translateX, translateY, translateZ);

	return matrix;
}

glm::mat4 Laborator4::GetRotatingPosition(glm::mat4 origin, float range, float speed)
{
	glm::mat4 matrix = origin;
	matrix *= Transform3D::RotateOY(speed * elapsedTime);
	matrix *= Transform3D::Translate(range, 0, 0);

	return matrix;
}

glm::mat4 Laborator4::GetChaoticFloatingPosition()
{
	glm::mat4 matrix = glm::mat4(1);

	float displacementY = sin(TETRAHEDRON_FLOAT_SPEED * elapsedTime);
	
	matrix *= Transform3D::Translate(TETRAHEDRON_INITIAL_X, TETRAHEDRON_INITIAL_Y, TETRAHEDRON_INITIAL_Z);
	matrix *= Transform3D::Translate(0, displacementY, 0);
	matrix *= Transform3D::RotateOX(elapsedTime);
	matrix *= Transform3D::RotateOY(elapsedTime);
	matrix *= Transform3D::RotateOZ(elapsedTime);

	return matrix;
}

Mesh* Laborator4::CreateTetrahedron(const char* name, double scale) {

	vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(scale,  scale,  scale), glm::vec3(0, 1, 0)),//1
		VertexFormat(glm::vec3(scale,  scale,  scale), glm::vec3(0, 0, 1)),//2
		VertexFormat(glm::vec3(scale,  scale,  scale), glm::vec3(1, 0, 1)),//3
		VertexFormat(glm::vec3(-scale,  scale, -scale), glm::vec3(1, 0, 0)),//0
		VertexFormat(glm::vec3(-scale,  scale, -scale), glm::vec3(0, 0, 1)),//2
		VertexFormat(glm::vec3(-scale,  scale, -scale), glm::vec3(1, 0, 1)),//3
		VertexFormat(glm::vec3(scale, -scale, -scale), glm::vec3(1, 0, 0)),//0
		VertexFormat(glm::vec3(scale, -scale, -scale), glm::vec3(0, 1, 0)),//1
		VertexFormat(glm::vec3(scale, -scale, -scale), glm::vec3(1, 0, 1)),//3
		VertexFormat(glm::vec3(-scale, -scale,  scale), glm::vec3(1, 0, 0)),//0
		VertexFormat(glm::vec3(-scale, -scale,  scale), glm::vec3(0, 1, 0)),//1
		VertexFormat(glm::vec3(-scale, -scale,  scale), glm::vec3(0, 0, 1)),//2
	};

	vector<unsigned short> indices = {
		3, 6, 9,
		0, 10, 7,
		1, 4, 11,
		2, 8, 5
	};

	return CreateMesh(name, vertices, indices);

}

Mesh* Laborator4::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO_ID = 0;
	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);

	unsigned int VBO_ID = 0;
	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	unsigned int IBO_ID = 0;
	glGenBuffers(1, &IBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be sent to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO_ID, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	elapsedTime += deltaTimeSeconds;

	glm::mat4 firstBoxPosition = GetFirstBoxPosition();
	glm::mat4 secondBoxPosition = GetRotatingPosition(firstBoxPosition, SECOND_BOX_ORBIT_RANGE, SECOND_BOX_ORBIT_SPEED);
	glm::mat4 thirdBoxPosition = GetRotatingPosition(secondBoxPosition, THIRD_BOX_ORBIT_RANGE, THIRD_BOX_ORBIT_SPEED);
	glm::mat4 tetrahedronPosition = GetChaoticFloatingPosition();

	modelMatrix = firstBoxPosition;
	RenderMesh(meshes["box"], shaders["VertexNormal"], firstBoxPosition);

	modelMatrix = secondBoxPosition * Transform3D::Scale(scaleX, scaleY, scaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = thirdBoxPosition;
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::RotateOY(angularStepOY);
	modelMatrix *= Transform3D::RotateOZ(angularStepOZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = tetrahedronPosition;
	RenderMesh(meshes["tetrahedron0"], shaders["VertexColor"], modelMatrix);

}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	// MOVEMENT (don't interfere with window movement)
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

		// HORIZONTAL
		if (window->KeyHold(GLFW_KEY_W))
			translateZ -= deltaTime;

		if (window->KeyHold(GLFW_KEY_A))
			translateX -= deltaTime;

		if (window->KeyHold(GLFW_KEY_S))
			translateZ += deltaTime;

		if (window->KeyHold(GLFW_KEY_D))
			translateX += deltaTime;

		// VERTICAL
		if (window->KeyHold(GLFW_KEY_R))
			translateY += deltaTime;

		if (window->KeyHold(GLFW_KEY_F))
			translateY -= deltaTime;

	}

	// SCALE
	if (window->KeyHold(GLFW_KEY_1)) {
		scaleX += deltaTime;
		scaleY += deltaTime;
		scaleZ += deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_2)) {
		scaleX -= deltaTime;
		scaleY -= deltaTime;
		scaleZ -= deltaTime;
	}

	// ROTATE
	if (window->KeyHold(GLFW_KEY_3))
		angularStepOX += deltaTime;

	if (window->KeyHold(GLFW_KEY_4))
		angularStepOX -= deltaTime;

	if (window->KeyHold(GLFW_KEY_5))
		angularStepOY += deltaTime;

	if (window->KeyHold(GLFW_KEY_6))
		angularStepOY -= deltaTime;

	if (window->KeyHold(GLFW_KEY_7))
		angularStepOZ += deltaTime;

	if (window->KeyHold(GLFW_KEY_8))
		angularStepOZ -= deltaTime;

}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
