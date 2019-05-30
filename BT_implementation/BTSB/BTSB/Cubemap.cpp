#include "Cubemap.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Renderer.h"
#include <glm/ext.hpp>
#include "stb_image/stb_image.h"

Cubemap::Cubemap()
{
	float vertices[] = {        
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	glGenBuffers(1, &vbo_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Source skybox textures
	vector<std::string> faces
	{
		"resources/cubemap/right.jpg",
		"resources/cubemap/left.jpg",
		"resources/cubemap/top.jpg",
		"resources/cubemap/bottom.jpg",
		"resources/cubemap/front.jpg",
		"resources/cubemap/back.jpg"
	};

	// Load textures from file
	cubemap_texture_ = loadCubemap(faces);
	cubemap_shader_ = new Shader("shaders/cubemap.vs", "shaders/cubemap.fs");
}

void Cubemap::Draw(Shader shader)
{
	// Disable depth mask, so skybox is always in background
	glDepthMask(GL_FALSE);
	
	cubemap_shader_->use();
	cubemap_shader_->setInt("cubemapTexture", 0);

	// camera/view transformation
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(Renderer::instance().window_width) / (float)(Renderer::instance().window_height), 0.1f, 100.0f);
	cubemap_shader_->setMat4("projection", projection);
	glm::mat4 view = glm::mat4(glm::mat3(Renderer::instance().GetCamera()->GetViewMatrix())); // Ignore skybox translation
	cubemap_shader_->setMat4("view", view);

	glBindVertexArray(vao_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture_);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE); // Enable depth mask again
	glBindVertexArray(0);
}

unsigned int Cubemap::loadCubemap(std::vector<std::string> faces)
{
	unsigned int texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	int width, height, nr_channels;

	// Load all sides
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nr_channels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;	
		}
		stbi_image_free(data);
	}

	// Set filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texture_id;
}

Cubemap::~Cubemap()
{
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
}