#include "../../headers/effects/terrain.hpp" 

TriangleList::TriangleList() 
{} 

void TriangleList::createTriangleList(int width, int depth, const Terrain* pTerrain) 
{
    m_width = width; 
    m_depth = depth; 

    CreateGLState(); 
    PopulateBuffers(pTerrain); 

    glBindVertexArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
} 

void TriangleList::CreateGLState() 
{
    glGenVertexArrays(1, &m_vao); 
    glBindVertexArray(m_vao); 

    glGenBuffers(1, &m_vbo_position); 
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_position); 

    glGenBuffers(1, &m_vbo_texCoord); 
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_texCoord); 

    glGenBuffers(1, &m_ib); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib); 

    size_t numFloats = 0; 
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION); 
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(numFloats * sizeof(float))); 
    numFloats += 3;

    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD); 
    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(numFloats * sizeof(float))); 
    numFloats += 2;

    glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL); 
    glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(numFloats * sizeof(float))); 
    numFloats += 3;
} 

void TriangleList::PopulateBuffers(const Terrain* pTerrain) 
{
    std::vector<Vertex> vertices; 
    vertices.resize(m_width * m_depth); 

    InitVertices(pTerrain, vertices); 

    std::vector<unsigned int> indices; 
    int numQuads = (m_width - 1) * (m_depth - 1); 
    indices.resize(numQuads * 6); 
    InitIndices(indices); 

    calculateNormals(vertices, indices); 

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*vertices.size(), &vertices[0], GL_STATIC_DRAW); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*indices.size(), &indices[0], GL_STATIC_DRAW); 
} 

void TriangleList::InitVertices(const Terrain* pTerrain, std::vector<Vertex>& vertices) 
{
    int index = 0; 

    for(int z = 0; z < m_depth; z++) 
    {
        for(int x = 0; x < m_width; x++) 
        {
            assert(index < vertices.size()); 
            vertices[index].InitVertex(pTerrain, x, z); 
            index++; 
        } 
    } 
} 

void TriangleList::InitIndices(std::vector<unsigned int>& indices) 
{
    int index = 0; 

    for(int z = 0; z < m_depth-1; z++) 
    {
        for(int x = 0; x < m_width-1; x++) 
        {
            unsigned int indexBottomLeft = z * m_width + x; 
            unsigned int indexTopLeft = (z+1) * m_width + x; 
            unsigned int indexTopRight = (z+1) * m_width + x+1; 
            unsigned int indexBottomRight = z * m_width + x+1; 

            // add top left triangle 
            indices[index++] = indexBottomLeft;     //  ______
            indices[index++] = indexTopLeft;        //  |   / | 
            indices[index++] = indexTopRight;       //  |  /  | 
                                                    //  | /   | 
            // add bottom triangle                  //   ------  
            indices[index++] = indexBottomLeft; 
            indices[index++] = indexTopRight; 
            indices[index++] = indexBottomRight; 
        } 
    } 
} 

void TriangleList::calculateNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) 
{
    unsigned int index = 0; 

    // accumulate each triangle normal into each of the triangle vertices 
    for(unsigned int i = 0; i < indices.size(); i+=3) 
    {
        unsigned int index0 = indices[i]; 
        unsigned int index1 = indices[i+1]; 
        unsigned int index2 = indices[i+2]; 
        
        vec3 v1 = vertices[index1].position - vertices[index0].position; 
        vec3 v2 = vertices[index2].position - vertices[index0].position; 
        vec3 normal = cross(v1, v2); 
        normalize(normal); 

        vertices[index0].normal += normal; 
        vertices[index1].normal += normal; 
        vertices[index2].normal += normal; 
    } 

    // normalize all vertex normals 
    for(unsigned int i = 0; i < vertices.size(); ++i) 
        normalize(vertices[i].normal); 
} 

void TriangleList::Render() 
{
    glBindVertexArray(m_vao); 
    glDrawElements(GL_TRIANGLES, (m_depth-1) * (m_width-1) * 6, GL_UNSIGNED_INT, NULL); 
    glBindVertexArray(0); 
}









void Vertex::InitVertex(const Terrain* pTerrain, int x, int z) 
{
    float y = pTerrain->GetHeight(x, z); 
    float worldScale = pTerrain->GetScale(); 
    float heightScale = pTerrain->GetHeightScaleFactor(); 
    position = vec3(x*worldScale, y*heightScale, z*worldScale); 

    int textureScale = pTerrain->GetTextureScale(); 
    float size = (float)pTerrain->GetSize(); 
    texCoord = vec2(
                    (float)x/size * textureScale, 
                    (float)z/size * textureScale 
                ); 

    normal = vec3(0.0f, 0.0f, 0.0f); 
} 

float Vertex::getX() const {return position[0];}; 
float Vertex::getY() const {return position[1];}; 
float Vertex::getZ() const {return position[2];}; 




















