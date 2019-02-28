#pragma once
#include <glm/glm.hpp>

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

struct Vertex 
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct STexture
{
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<STexture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<STexture> textures);
	void Draw(Shader shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();


};