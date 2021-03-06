#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "Object.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Model : public Object
{
public:
	//Model(char *path)
	//{
	//	loadModel(path);
	//}

	Model();
	//~Model();

	virtual void Draw(Shader shader) override;
	void loadModel(string path);

private:
	vector<Texture> textures_loaded_;
	vector<Mesh> meshes_;
	string directory_;

	int vertex_count_ = 0;
	int face_count_ = 0;

	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName);

	Shader* basic_lighting_;

	Shader* normals_visualization_;
};