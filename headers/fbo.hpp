#pragma once 

#include "global_header.hpp" 

class FBO 
{
    private: 
        static const int maxColorBuffers = 4; 
        GLuint fbo; 
        GLuint rbo; 
        GLuint textures[maxColorBuffers]; 

    public: 
        GLuint getTextureID() const; 

        bool createNormalFBO(GLuint textureWidth, GLuint textureHeight); 
        bool createFloatingPointFBO(GLuint textureWidth, GLuint textureHeight); 
        bool createFBOWithoutDepthBuffer(GLuint textureWidth, GLuint textureHeight); 

        void bind() const; 
        void unbind() const; 

        void destroyFBO(); 
}; 
