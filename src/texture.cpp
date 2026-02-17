#include "..\headers\texture.hpp" 

GLuint loadTexture(const char* pathToImage, BOOL bFlipImage) 
{
	// function prototypes 
	void uninitialize(void); 

	// variable declarations 
	GLuint texture; 
	int width, height, nrComponents; 
	unsigned char* pData = NULL; 

	// code 
	if(bFlipImage == TRUE) 
	{
		stbi_set_flip_vertically_on_load(1); 
	} 

	// get the image data 
	pData = stbi_load(pathToImage, &width, &height, &nrComponents, 0); 
	if(pData == NULL) 
	{
		logFile.log("Texture loading failed for %s\n", pathToImage); 
		uninitialize(); 
		exit(EXIT_FAILURE); 
	}  	
	
	// generate OpenGL texture object 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); 	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLenum format; 
	switch(nrComponents) 
	{
		case 1: 
			format = GL_R; 
			break; 

		case 2: 
			format = GL_RG; 
			break; 

		case 3: 
			format = GL_RGB; 
			break; 

		case 4: 
			format = GL_RGBA; 
			break; 

		default: 
			format = GL_RGBA; 
			break; 
	} 

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		format, 
		width, 
		height, 
		0, 
		format, 
		GL_UNSIGNED_BYTE, 
		pData
	);
	glGenerateMipmap(GL_TEXTURE_2D);  

	glBindTexture(GL_TEXTURE_2D, 0); 
	stbi_image_free(pData); 
	pData = NULL; 

	logFile.log("Texture loaded : %s\n", pathToImage); 

	stbi_set_flip_vertically_on_load(0); 

	return (texture); 
}  


