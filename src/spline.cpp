#include "../headers/spline.hpp" 

Spline3D::Spline3D() : isOpenGLStateInitialized(false) 
{
    controlPoints.push_back(vmath::vec3(0.0)); 
    controlPoints.push_back(vmath::vec3(4.0)); 
} 

Spline3D::Spline3D(std::vector<vmath::vec3> controlPointsArray) : isOpenGLStateInitialized(false) 
{
    controlPoints = controlPointsArray; 
} 

float 
Spline3D::getKnotValue(float t, float alpha, vmath::vec3 p0, vmath::vec3 p1) 
{
    float d = vmath::distance(p0, p1); 
    return (t + pow(d, alpha)); 
} 

vmath::vec3 
Spline3D::catmullRom(vmath::vec3 p0, vmath::vec3 p1, vmath::vec3 p2, vmath::vec3 p3, float t, float alpha) 
{
    float t0 = 0.0f; 
    float t1 = getKnotValue(t0, alpha, p0, p1); 
    float t2 = getKnotValue(t1, alpha, p1, p2); 
    float t3 = getKnotValue(t2, alpha, p2, p3); 

    // remap input t [0, 1] to [t1, t2] 
    float tt = t1 + t * (t2 - t1); 

    // De Boor like pyramid evaluation 
    vmath::vec3 A1 = (t1-tt)/(t1-t0) * p0 + (tt-t0)/(t1-t0)*p1; 
    vmath::vec3 A2 = (t2-tt)/(t2-t1) * p1 + (tt-t1)/(t2-t1)*p2; 
    vmath::vec3 A3 = (t3-tt)/(t3-t2) * p2 + (tt-t2)/(t3-t2)*p3;
    
    vmath::vec3 B1 = (t2-tt)/(t2-t0) * A1 + (tt-t0)/(t2-t0) * A2; 
    vmath::vec3 B2 = (t3-tt)/(t3-t1) * A2 + (tt-t1)/(t3-t1) * A3; 

    vmath::vec3 C = (t2-tt)/(t2-t1) * B1 + (tt-t1)/(t2-t1) * B2; 
    return (C); 
}  

vmath::vec3 
Spline3D::getPoint(const std::vector<vmath::vec3>& pts, int index) 
{
    if(index < 0) 
    {
        // phantom: reflect first segment direction 
        return (pts[0] + (pts[0] - pts[1]));  
    } 
    if(index >= (int)pts.size()) 
    {
        // phantom: reflect last segment direction 
        int n = pts.size() - 1; 
        return (pts[n] + pts[n] - pts[n-1]); 
    } 

    return (pts[index]); 
} 

void 
Spline3D::setAlpha(float _alpha) 
{
    this->alpha = _alpha; 
} 

vmath::vec3 
Spline3D::evaluatePositionAtT(float globalT) 
{
    int numSegments = controlPoints.size() - 1; 
    float scaled = globalT * numSegments; 
    int segment = vmath::clamp((int)floor(scaled), 0, numSegments - 1); 
    float localT = scaled - segment; 

    vmath::vec3 P0 = getPoint(controlPoints, segment-1); 
    vmath::vec3 P1 = getPoint(controlPoints, segment); 
    vmath::vec3 P2 = getPoint(controlPoints, segment+1); 
    vmath::vec3 P3 = getPoint(controlPoints, segment+2); 

    return (catmullRom(P0, P1, P2, P3, localT, this->alpha)); 
}  

void 
Spline3D::getPositionsOnSpline(std::vector<vmath::vec3>& positionArray, int count) 
{
    float increment;
    assert(count != 0); 

    // adds support for parameter count=1 by returning point at middle on spline 
    if(count == 1) 
    {
        positionArray.push_back(evaluatePositionAtT(0.5)); 
        return; 
    } 
    else 
    {
        increment = 1.0/(count-1); 
    } 

    float t = 0.0; 
    while(t <= 1.0) 
    {
        positionArray.push_back(evaluatePositionAtT(t)); 
        t += increment; 
    } 
} 

vmath::mat4 
Spline3D::getViewMatrix(float t) 
{
    vmath::vec3 camPos = evaluatePositionAtT(t); 

    return (vmath::lookat(camPos, vmath::vec3(0.0), vmath::vec3(0.0, 1.0, 0.0))); 
} 

void
Spline3D::show(vmath::mat4 _mvpMatrix) 
{
    if(isOpenGLStateInitialized == false) 
        initOpenGLState(); 

    shaderProgram.use(); 
    glBindVertexArray(vao); 
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, _mvpMatrix); 
    glPointSize(4.0); 
    glDrawArrays(GL_LINE_STRIP, 0, positionsOnSpline.size()); 
    shaderProgram.unuse(); 
} 

void 
Spline3D::addRandomControlPoint() 
{
    vmath::vec3 newControlPoint; 
    size_t positionArraySize = controlPoints.size(); 

    // calculate new point position based last and second last point 
    newControlPoint[0] = controlPoints[positionArraySize-1][0] + fabs(controlPoints[positionArraySize-1][0] - controlPoints[positionArraySize-2][0]); 
    newControlPoint[1] = controlPoints[positionArraySize-1][1] + fabs(controlPoints[positionArraySize-1][1] - controlPoints[positionArraySize-2][1]); 
    newControlPoint[2] = controlPoints[positionArraySize-1][2] + fabs(controlPoints[positionArraySize-1][2] - controlPoints[positionArraySize-2][2]); 
    
    controlPoints.push_back(newControlPoint); 
    
    // update buffer 
    positionsOnSpline.clear(); 
    getPositionsOnSpline(positionsOnSpline, controlPoints.size()*25); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    glBufferData(GL_ARRAY_BUFFER, positionsOnSpline.size()*sizeof(vmath::vec3), positionsOnSpline.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
} 

void 
Spline3D::addControlPointAtPos(vmath::vec3 newControlPointPosition) 
{
    controlPoints.push_back(newControlPointPosition); 

    // update buffer 
    positionsOnSpline.clear(); 
    getPositionsOnSpline(positionsOnSpline, controlPoints.size()*25); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    glBufferData(GL_ARRAY_BUFFER, positionsOnSpline.size()*sizeof(vmath::vec3), positionsOnSpline.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
} 

/* 
std::vector<float> Spline3D::buildArcLengthTable(const std::vector<vmath::vec3>& pts, int samples = 1000) 
{
    std::vector<float> table(samples+1); 
    table[0] = 0.0f; 
    vmath::vec3 prev = evaluatePositionAtT(pts, 0.0f); 

    for(int i = 1; i <= samples; ++i) 
    {
        float t = (float)i / samples; 
        vmath::vec3 curr = evaluatePositionAtT(pts, t); 
        table[i] = table[i-1] + vmath::distance(prev, curr); 
        prev = curr; 
    } 

    return (table); 
} 

float Spline3D::arcLengthToT(const std::vector<float>& table, float targetLen) 
{
    int samples = table.size() - 1; 
    float totalLen = table.back(); 
    targetLen = vmath::clamp(targetLen, 0.0f, totalLen); 
    
    // binary search 
    int lo = 0, hi = samples; 
    while(lo < hi) 
    {
        int mid = (lo+hi) / 2; 
        if(table[mid] < targetLen) 
            lo = mid+1; 
        else 
            hi = mid; 
    } 

    float frac = (targetLen - table[lo-1]) / (table[lo] - table[lo-1]); 
    return ((lo-1) + frac) / samples; 
} 
*/ 

void 
Spline3D::initOpenGLState() 
{
	// code 
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/bw.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/bw.fs"); 
    // if(vertexShaderSourceCode == NULL || fragmentShaderSourceCode == NULL) 
    //     return false; 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_COLOR, "aColor")); 
	
    shaderProgram.create(shaders, attributes); 

    // get uniform locations 
    mvpMatrixUniform = shaderProgram.getUniformLocation("uMVPMatrix"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 

    // --------- FILL BUFFERS -------- 
    getPositionsOnSpline(positionsOnSpline, controlPoints.size()*25);  // fill array with nr of positions based on number of control points 
    glGenVertexArrays(1, &vao); 
    glBindVertexArray(vao);     
    
    glGenBuffers(1, &vbo); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
        glBufferData(GL_ARRAY_BUFFER, positionsOnSpline.size()*sizeof(vmath::vec3), positionsOnSpline.data(), GL_DYNAMIC_DRAW); 
        glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL); 
        glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION); 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glVertexAttrib3fv(AMC_ATTRIBUTE_COLOR, vec3(1.0, 1.0, 1.0)); 
    glBindVertexArray(0); 

    isOpenGLStateInitialized = true; 
} 

// =================================================================================================================================

Spline1D::Spline1D(std::vector<float> valuesArray) 
{
    values = valuesArray; 
} 

float Spline1D::getKnotValue(float t, float alpha, float v0, float v1) 
{
    float d = fabs(v0-v1); 
    return (t + pow(d, alpha)); 
} 

float Spline1D::catmullRom(float p0, float p1, float p2, float p3, float t, float alpha) 
{
    float t0 = 0.0f; 
    float t1 = getKnotValue(t0, alpha, p0, p1); 
    float t2 = getKnotValue(t1, alpha, p1, p2); 
    float t3 = getKnotValue(t2, alpha, p2, p3); 

    // remap input t [0, 1] to [t1, t2] 
    float tt = t1 + t * (t2 - t1); 

    // De Boor like pyramid evaluation 
    float A1 = (t1-tt)/(t1-t0) * p0 + (tt-t0)/(t1-t0)*p1; 
    float A2 = (t2-tt)/(t2-t1) * p1 + (tt-t1)/(t2-t1)*p2; 
    float A3 = (t3-tt)/(t3-t2) * p2 + (tt-t2)/(t3-t2)*p3;
    
    float B1 = (t2-tt)/(t2-t0) * A1 + (tt-t0)/(t2-t0) * A2; 
    float B2 = (t3-tt)/(t3-t1) * A2 + (tt-t1)/(t3-t1) * A3; 

    float C = (t2-tt)/(t2-t1) * B1 + (tt-t1)/(t2-t1) * B2; 
    return (C); 
}  

float Spline1D::getValue(const std::vector<float>& pts, int index) 
{
    if(index < 0) 
    {
        // phantom: reflect first segment direction 
        return (pts[0] + (pts[0] - pts[1]));  
    } 
    if(index >= (int)pts.size()) 
    {
        // phantom: reflect last segment direction 
        int n = pts.size() - 1; 
        return (pts[n] + pts[n] - pts[n-1]); 
    } 

    return (pts[index]); 
} 

void Spline1D::setAlpha(float _alpha) 
{
    this->alpha = _alpha; 
} 

float Spline1D::evaluateValue(float globalT) 
{
    int numSegments = values.size() - 1; 
    float scaled = globalT * numSegments; 
    int segment = vmath::clamp((int)floor(scaled), 0, numSegments - 1); 
    float localT = scaled - segment; 

    float V0 = getValue(values, segment-1); 
    float V1 = getValue(values, segment); 
    float V2 = getValue(values, segment+1); 
    float V3 = getValue(values, segment+2); 

    return (catmullRom(V0, V1, V2, V3, localT, this->alpha)); 
}  

void Spline1D::addRandomValue() 
{
    float newValue; 
    size_t valuesArraySize = values.size(); 

    // calculate new point position based last and second last point 
    newValue = values[valuesArraySize-1] + fabs(values[valuesArraySize-1] - values[valuesArraySize-2]); 
    
    values.push_back(newValue); 
} 

void Spline1D::addGivenValue(float newValue) 
{
    values.push_back(newValue); 
} 

/* 
std::vector<float> Spline1D::buildArcLengthTable(const std::vector<float>& pts, int samples = 1000) 
{
    std::vector<float> table(samples+1); 
    table[0] = 0.0f; 
    float prev = evaluatePositionAtT(pts, 0.0f); 

    for(int i = 1; i <= samples; ++i) 
    {
        float t = (float)i / samples; 
        float curr = evaluatePositionAtT(pts, t); 
        table[i] = table[i-1] + vmath::distance(prev, curr); 
        prev = curr; 
    } 

    return (table); 
} 

float Spline1D::arcLengthToT(const std::vector<float>& table, float targetLen) 
{
    int samples = table.size() - 1; 
    float totalLen = table.back(); 
    targetLen = vmath::clamp(targetLen, 0.0f, totalLen); 
    
    // binary search 
    int lo = 0, hi = samples; 
    while(lo < hi) 
    {
        int mid = (lo+hi) / 2; 
        if(table[mid] < targetLen) 
            lo = mid+1; 
        else 
            hi = mid; 
    } 

    float frac = (targetLen - table[lo-1]) / (table[lo] - table[lo-1]); 
    return ((lo-1) + frac) / samples; 
} 
*/ 

