#include "Laborator2.h"

#include <vector>
#include <iostream>
#include <math.h>

#include <Core/Engine.h>

using namespace std;

Laborator2::Laborator2()
{
}

Laborator2::~Laborator2()
{

}

void Laborator2::Init()
{
	cullFace = GL_BACK;
	polygonMode = GL_FILL;

	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a mesh box using custom data
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 0, 0)),
			VertexFormat(glm::vec3( 1, -1,  1), glm::vec3(0, 0, 1)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(0, 1, 0)),
			VertexFormat(glm::vec3( 1,  1,  1), glm::vec3(0, 1, 1)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 0, 0)),
			VertexFormat(glm::vec3( 1, -1, -1), glm::vec3(1, 0, 1)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0)),
			VertexFormat(glm::vec3( 1,  1, -1), glm::vec3(1, 1, 1)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,	// indices for first triangle
			1, 3, 2,	// indices for second triangle
			2, 3, 7,
			2, 7, 6,
			1, 7, 3,
			1, 5, 7,
			6, 7, 4,
			7, 5, 4,
			0, 4, 1,
			1, 4, 5,
			2, 6, 4,
			0, 2, 4
		};

		meshes["cube1"] = new Mesh("generated cube 1");
		meshes["cube1"]->InitFromData(vertices, indices);

		// Create a new mesh from buffer data
		Mesh *cube = CreateMesh("cube3", vertices, indices);
		Mesh* dodecahedron = CreateTetrahedron("tetrahedron0");
		Mesh* halfVisibleSquare = CreateHalfVisibleSquare("halfVisibleSquare0");
	}
}

Mesh* Laborator2::CreateDisk(const char* name) {
	return CreateDisk(name, 1);
}

Mesh* Laborator2::CreateDisk(const char* name, double range) {

	vector<VertexFormat> vertices = { VertexFormat(glm::vec3(0), glm::vec3(1)) };
	vector<unsigned short> indices = { 0 };

	int currIndex = 1;

	for (int i = 0; i < diskTriangleCount; i++) {
		float t = 2*M_PI / diskTriangleCount * i;

		vertices.push_back(VertexFormat(glm::vec3(-range * cos(t), 0, range * sin(t)), glm::vec3(1)));
		indices.push_back(currIndex++);
	}

	indices.push_back(1);

	Mesh* disk = CreateMesh(name, vertices, indices);
	disk->SetDrawMode(GL_TRIANGLE_FAN);

	return disk;

}

Mesh* Laborator2::CreateHalfVisibleSquare(const char* name) {
	return CreateHalfVisibleSquare(name, 1);
}

Mesh* Laborator2::CreateHalfVisibleSquare(const char* name, double scale) {

	vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3( scale,  0,   scale), glm::vec3(0, 0, 1)),
		VertexFormat(glm::vec3(-scale,  0,   scale), glm::vec3(0, 1, 0)),
		VertexFormat(glm::vec3(-scale,  0,  -scale), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3( scale,  0,  -scale), glm::vec3(1, 0, 1)),
	};

	vector<unsigned short> indices = {
		0, 1, 3,
		1, 3, 2
	};

	return CreateMesh(name, vertices, indices);

}

Mesh* Laborator2::CreateTetrahedron(const char* name) {
	return CreateTetrahedron(name, 1);
}

Mesh* Laborator2::CreateTetrahedron(const char* name, double scale) {

	vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3( scale,  scale,  scale), glm::vec3(0, 0, 1)),
		VertexFormat(glm::vec3(-scale,  scale, -scale), glm::vec3(0, 1, 0)),
		VertexFormat(glm::vec3( scale, -scale, -scale), glm::vec3(1, 0, 0)),
		VertexFormat(glm::vec3(-scale, -scale,  scale), glm::vec3(1, 0, 1)),
	};

	vector<unsigned short> indices = {
		1, 2, 3,
		0, 3, 2,
		0, 1, 3,
		0, 2, 1
	};

	return CreateMesh(name, vertices, indices);

}

Mesh* Laborator2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
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

void Laborator2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	glEnable(GL_CULL_FACE);

	glCullFace(cullFace);

	// render an object using face normals for color
	RenderMesh(meshes["box"], shaders["VertexNormal"], glm::vec3(0, 0.5f, -1.5f), glm::vec3(0.75f));

	// render an object using colors from vertex
	RenderMesh(meshes["cube1"], shaders["VertexColor"], glm::vec3(-1.5f, 0.5f, 0), glm::vec3(0.25f));

	RenderMesh(meshes["cube3"], shaders["VertexColor"], glm::vec3(1.5f, -0.5f, 0));
	RenderMesh(meshes["tetrahedron0"], shaders["VertexColor"], glm::vec3(0, 3, 0));
	RenderMesh(meshes["halfVisibleSquare0"], shaders["VertexColor"], glm::vec3(0, 0, -3));

	Mesh* disk = CreateDisk("disk0");
	RenderMesh(meshes["disk0"], shaders["VertexColor"], glm::vec3(0, 0, 3));

	glDisable(GL_CULL_FACE);
}

void Laborator2::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator2::OnInputUpdate(float deltaTime, int mods)
{

}

void Laborator2::OnKeyPress(int key, int mods)
{

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
	else if (key == GLFW_KEY_F2)
	{
		if (cullFace == GL_FRONT)
			cullFace = GL_BACK;
		else
			cullFace = GL_FRONT;
	}
	else if (key == GLFW_KEY_RIGHT_BRACKET)
	{
		if (diskTriangleCount < MAX_DISK_TRIANGLE_COUNT)
			diskTriangleCount++;
	}
	else if (key == GLFW_KEY_LEFT_BRACKET)
	{
		if (diskTriangleCount > MIN_DISK_TRIANGLE_COUNT)
			diskTriangleCount--;
	}
}

void Laborator2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator2::OnWindowResize(int width, int height)
{
}
