#include "../headers/common.hpp" 

float degToRad(float degree) 
{
    return (degree * M_PI/180.0); 
}  

// ======================================= TEXTURE - START ===================================================

Texture::Texture() 
{
    id = 0; 
    width = 0; 
    height = 0; 
    components = 0; 
} 

Texture::~Texture() 
{
    glDeleteTextures(1, &id); 
    id = 0; 
} 

unsigned int Texture::getID() const 
{
    // code 
    return this->id; 
}

int Texture::getWidth() const 
{
    // code 
    return this->width; 
}

int Texture::getHeight() const 
{
    // code 
    return this->height; 
}

int Texture::getComponents() const 
{
    // code 
    return this->components; 
}

Texture Texture::loadTexture(const char* fileName, BOOL bFlipImage) 
{
    // local variable declarations 
    Texture texture; 
    unsigned char* pData = NULL; 

    // code 
    stbi_set_flip_vertically_on_load((bFlipImage == TRUE) ? 1 : 0); 

    pData = stbi_load(fileName, &texture.width, &texture.height, &texture.components, 0); 
    if(NULL == pData) 
    {
        logFile.log("Load texture failed to load %s\n", fileName); 
        // uninitialize(); 
        exit(EXIT_FAILURE); 
    } 

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    glGenTextures(1, &texture.id); 
    glBindTexture(GL_TEXTURE_2D, texture.id); 
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

        GLenum format; 
        switch(texture.components) 
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
        glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, pData); 
        glGenerateMipmap(GL_TEXTURE_2D); 
    }
    glBindTexture(GL_TEXTURE_2D, 0); 
    
    stbi_image_free(pData); 
    pData = NULL; 

    stbi_set_flip_vertically_on_load(0); 
    return (texture); 
} 

// ======================================= TEXTURE - END ===================================================
