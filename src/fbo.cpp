#include "..\headers\fbo.hpp" 

GLuint FBO::getTextureID() const 
{
	return textures[0]; 
} 

bool FBO::createNormalFBO(GLuint textureWidth, GLuint textureHeight) 
{
    // variable declarations 
	GLint maxRenderBufferSize; 

	// code 
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize); 
	if(maxRenderBufferSize < textureWidth || maxRenderBufferSize < textureHeight) 
	{
		logFile.log("FBO::createFloatingPointFBO() > FBO width or height are exceeding max. render buffer size\n"); 
		return false; 
	} 

	// Create new framebuffer 
	glGenFramebuffers(1, &fbo); 

	// Bind newly created framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); 

	glGenRenderbuffers(1, &rbo); 
	glBindRenderbuffer(GL_RENDERBUFFER, rbo); 

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);  

	// Create an empty but full-capable and compatible texture as color buffers 
	glGenTextures(1, &textures[0]);

	glBindTexture(GL_TEXTURE_2D, textures[0]); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureWidth, textureHeight, 0, GL_RGBA, GL_FLOAT, NULL); 
	glGenerateMipmap(GL_TEXTURE_2D);  

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0); 
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		logFile.log("FBO::createFloatingPointFBO > floating point FBO creation incomplete\n"); 
		return (FALSE); 	
	} 
	else
	{
		logFile.log("FBO::createFloatingPointFBO > Floating point FBO created successfully\n"); 
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	return true; 
}  

bool FBO::createFloatingPointFBO(GLuint textureWidth, GLuint textureHeight) 
{
    // variable declarations 
	GLint maxRenderBufferSize; 

	// code 
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize); 
	if(maxRenderBufferSize < textureWidth || maxRenderBufferSize < textureHeight) 
	{
		logFile.log("FBO::createFloatingPointFBO() > FBO width or height are exceeding max. render buffer size\n"); 
		return false; 
	} 

	// Create new framebuffer 
	glGenFramebuffers(1, &fbo); 

	// Bind newly created framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); 

	glGenRenderbuffers(1, &rbo); 
	glBindRenderbuffer(GL_RENDERBUFFER, rbo); 

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);  

	// Create an empty but full-capable and compatible texture as color buffers 
	glGenTextures(1, &textures[0]);

	glBindTexture(GL_TEXTURE_2D, textures[0]); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, textureWidth, textureHeight, 0, GL_RGBA, GL_FLOAT, NULL); 
	glGenerateMipmap(GL_TEXTURE_2D);  

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0); 
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		logFile.log("FBO::createFloatingPointFBO > floating point FBO creation incomplete\n"); 
		return (FALSE); 	
	} 
	else
	{
		logFile.log("FBO::createFloatingPointFBO > Floating point FBO created successfully\n"); 
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	return true; 
} 

bool FBO::createFBOWithoutDepthBuffer(GLuint textureWidth, GLuint textureHeight) 
{
    // variable declarations 
	GLint maxRenderBufferSize; 

	// code 
	// (1) Check wether textureWidth and textureHeight parameters are compatible 
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize); 
	if(maxRenderBufferSize < textureWidth || maxRenderBufferSize < textureHeight) 
	{
		logFile.log("FBO::createFBOWithoutDepthBuffer() > FBO width or height are exceeding max. render buffer size\n"); 
		return (FALSE); 
	} 

	// Create new framebuffer 
	glGenFramebuffers(1, &fbo); 
	glGenTextures(1, &textures[0]);  
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); 
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, textureWidth, textureHeight, 0, GL_RGBA, GL_FLOAT, NULL); 
	glGenerateMipmap(GL_TEXTURE_2D);  
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0); 
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		logFile.log("FBO::createFBOWithoutDepthBuffer() > FBO with no depth buffer creation incomplete\n"); 
		return (FALSE); 	
	} 
	else
	{
		logFile.log("FBO::createFBOWithoutDepthBuffer() > FBO with no depth buffer created successfully\n"); 
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	rbo = 0; 

	return true; 
} 

void FBO::bind() const 
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); 
	glClearColor(0.0, 0.0, 0.0, 1.0); 
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 
	glViewport(0, 0, winWidth, winHeight); 
} 

void FBO::unbind() const 
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
} 

void FBO::destroyFBO() 
{
	// code 
	logFile.log("FBO::destroyFBO() > uninitializing...\n"); 
	
	if(fbo) 
	{
		glDeleteFramebuffers(1, &fbo); 
		fbo = 0; 
	}  
	if(rbo)
	{
		glDeleteRenderbuffers(1, &rbo); 
		rbo = 0; 
	} 
	 
	int i = 0; 
	while(textures[i] != 0) 
	{
		glDeleteTextures(1, textures+i); 
		*(textures+i) = 0; 
	} 
} 
