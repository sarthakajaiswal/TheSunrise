#ifndef ASSIMP_HPP 
#define ASSIMP_HPP 

#include <iostream> 
#include <string> 
#include <vector> 
#include <cassert> 

#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h> 

#include <gl/glew.h> 	
#include <gl/GL.h> 

#include "common.hpp" 
#include "stb_image.h" 
#include "vmath.h" 

extern FILE* gpFile; 

struct ModelTexture 
{
	GLuint id; 
	std::string type; 
	std::string path; 
}; 

class Model 
{
	class Mesh 
	{
		public: 
			std::vector<struct Vertex> vertices; 
			std::vector<unsigned int> indices; 
			std::vector<ModelTexture> textures; 

			mat4 nodeTransform; 

			// opengl objects 
			GLuint vao, vbo, ebo; 
 
			Mesh(); 
	}; 

	private: 
		std::vector<Mesh> meshes; 
		std::string directory; 
		std::vector<ModelTexture> loadedTextures; 

		GLuint modelsShaderProgram = 0; 
		GLuint mvpMatrixUniform_modelsProgram = 0; 
		GLuint modelMatrixUniform = 0; 
		GLuint viewPositionUniform = 0; 
		GLuint isFogUniform = 0; 
		GLuint fogStartUniform = 0; 
		GLuint fogEndUniform = 0; 
		GLuint fogColorUniform = 0; 

		GLuint occlusionProgram = 0; 
		GLuint mvpMatrixUniform_occlusion = 0; 

		std::vector<ModelTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		GLuint loadTextureFromFile(const char* filePath); 

	public: 
		Model(); 
		int initialize(const char* path); 
		void loadModel(const std::string& path); 
		void processNode(aiNode* node, const aiScene* scene, vmath::mat4 parentTransform); 
		Mesh processMesh(aiMesh* mesh, const aiScene* scene); 
		void setupMesh(Mesh& mesh); 
		void render(vmath::mat4 _modelMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix, bool bFog=false, float fogStart=0.0, float fogEnd=0.0, vmath::vec3 fogColor=vec3(0.0), vmath::vec3 viewPosition=vec3(0.0)); 
		void renderOcclusion(vmath::mat4 _modelMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix); 
		// int initOpenGLState(void); 
		int initModelsShaderProgram(void); 
		int initOcclusionShaderProgram(void); 
		~Model(); 
}; 

#endif // _ASSIMP_MODEL_LOADER_H 
