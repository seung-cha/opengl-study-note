#include "ModelImporter.h"


unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

Helper::Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMash();
}

void Helper::Mesh::Draw(const GLuint& shaderProgram)
{
	glUseProgram(shaderProgram);
	unsigned int diffNo = 0;
	unsigned int specNo = 0;

	//Bind textures

	for(unsigned int i = 0; i < textures.size(); i++)
	{

		glActiveTexture(GL_TEXTURE0 + i);

		string no;
		string name = textures[i].type;

		if(name == "textures_diffuse")
		{
			no = to_string(diffNo++);
		}
		else if(name == "textures_specular")
		{
			no = to_string(specNo++);
		}

		GLuint texPos = glGetUniformLocation(shaderProgram, ("material." + name + no).c_str());

		glUniform1i(texPos, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);

	}

	glBindVertexArray(VAO);
	

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);


}

void Helper::Mesh::setupMash()
{
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Bind VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FLOAT, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Buffer EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);


	glBindVertexArray(0);



}

void Helper::Model::Draw(const GLuint& shaderProgram, const char* modelMatName)
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, modelMatName)
		, 1, GL_FALSE, &transformMat[0][0]);

	for(unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shaderProgram);
	}

	glUseProgram(0);
}

void Helper::Model::loadModel(string path)
{
	Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "Error while loading in a model: " << importer.GetErrorString() << endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

}

void Helper::Model::processNode(aiNode* node, const aiScene* scene)
{

	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

Helper::Mesh Helper::Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;


	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;

		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = vec2(0);
		}

		vertices.push_back(vertex);
	}


	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<Texture> diffuseMaps = loadMaterialTextures(material,
		aiTextureType_DIFFUSE, "textures_diffuse");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "textures_specular");

		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	}



	return Mesh(vertices, indices, textures);
}

std::vector<Helper::Texture> Helper::Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < loadedTextures.size(); j++)
		{
			if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture); // add to loaded textures
		}
	}


	return textures;
}


unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	
	stbi_set_flip_vertically_on_load(1);
	
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	
	stbi_set_flip_vertically_on_load(0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}