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
	virtual void Draw(Shader shader) override;
	void loadModel(string path);

private:
	vector<STexture> textures_loaded;
	vector<Mesh> meshes;
	string directory;

	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<STexture> loadMaterialTextures(aiMaterial *material, aiTextureType type, string typeName);
};