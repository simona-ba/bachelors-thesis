#include "Model.h"
#include <glm/ext.hpp>
#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Renderer.h"
#include <glad/glad.h>

Model::Model()
{
	basic_lighting_ = new Shader("shaders/main.vs", "shaders/main.fs");
	normals_visualization_ = new Shader("shaders/normal.vs", "shaders/normal.fs", "shaders/normal.gs");
}

void Model::Draw(Shader shader)
{
	basic_lighting_->use();

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	basic_lighting_->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	basic_lighting_->setVec3("lightPos", Renderer::instance().GetLight()->GetLocation());
	basic_lighting_->setVec3("viewPos", Renderer::instance().GetCamera()->GetLocation());
	basic_lighting_->setInt("viewMode", Renderer::instance().current_view_mode);

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(Renderer::instance().window_width) / (float)(Renderer::instance().window_height), 0.1f, 100.0f);

	basic_lighting_->setMat4("projection", projection);
	basic_lighting_->setMat4("view", Renderer::instance().GetCamera()->GetViewMatrix());
	basic_lighting_->setInt("specularEnabled", Renderer::instance().IsSpecularEnabled());
	basic_lighting_->setInt("blinnEnabled", Renderer::instance().IsBlinnEnabled());
	basic_lighting_->setInt("normalEnabled", Renderer::instance().IsNormalEnabled());

	// world transformation
	basic_lighting_->setMat4("model", model_matrix);

	for (unsigned int i = 0; i < meshes_.size(); i++)
	{
		meshes_[i].Draw(*basic_lighting_);
	}

	if (Renderer::instance().current_view_mode == NORMALS)
	{
		normals_visualization_->use();
		normals_visualization_->setMat4("projection", projection);
		normals_visualization_->setMat4("view", Renderer::instance().GetCamera()->GetViewMatrix());
		normals_visualization_->setMat4("model", model_matrix);

		for (unsigned int i = 0; i < meshes_.size(); i++)
		{
			meshes_[i].Draw(*normals_visualization_);
		}
	}
}

void Model::loadModel(string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	directory_ = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

	cout << "Loaded model has " << vertex_count_ << " vertices and " << face_count_ << " polygons.";
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes_.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	vertex_count_ += mesh->mNumVertices;
	face_count_ += mesh->mNumFaces;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. diffuse maps
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return Mesh(vertices, indices, textures);
}


vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded_.size(); j++)
		{
			if (std::strcmp(textures_loaded_[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded_[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory_);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded_.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}