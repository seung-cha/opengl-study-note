#ifndef MODEL_IMPORTER_H
#define MODEL_IMPORTER_H

#include <glm.hpp>
#include <string>
#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"



namespace Helper
{
	using namespace std;
	using namespace glm;
	using namespace Assimp;

	struct Vertex
	{
		vec3 Position;
		vec3 Normal;
		vec2 TexCoords;

	};

	struct Texture
	{
		unsigned int id;
		string type;
		string path;
	};


	class Mesh
	{
		public:
			vector<Vertex> vertices;
			vector<unsigned int> indices;
			vector<Texture> textures;

			Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
			void Draw(const GLuint& shaderProgram);

	private:
		GLuint VAO, VBO, EBO;
		void setupMash();

		
	};


	class Model
	{
		public:
			Model(const char* path)
			{
				transformMat = glm::mat4(1);
				loadModel(path);
			}

			void Draw(const GLuint& shaderProgram, const char* modelMatName = "model");

			inline glm::mat4 GetTransform() { return transformMat; }
			inline void SetTransform(glm::mat4 matrix) { transformMat = matrix; }

		private:
			vector<Mesh> meshes;
			string directory;

			void loadModel(string path);
			void processNode(aiNode* node, const aiScene* scene);
			Mesh processMesh(aiMesh* mesh, const aiScene* scene);
			vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
			vector<Texture> loadedTextures;

			glm::mat4 transformMat;


	};

}


#endif
