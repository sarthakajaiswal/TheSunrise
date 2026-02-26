#include "../headers/spline.hpp" 

Spline3D::Spline3D(std::vector<vmath::vec3> controlPointsArray) 
{
    positions = controlPointsArray; 
} 

float Spline3D::getKnotValue(float t, float alpha, vmath::vec3 p0, vmath::vec3 p1) 
{
    float d = vmath::distance(p0, p1); 
    return (t + pow(d, alpha)); 
} 

vmath::vec3 Spline3D::catmullRom(vmath::vec3 p0, vmath::vec3 p1, vmath::vec3 p2, vmath::vec3 p3, float t, float alpha) 
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

vmath::vec3 Spline3D::getPoint(const std::vector<vmath::vec3>& pts, int index) 
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

void Spline3D::setAlpha(float _alpha) 
{
    this->alpha = _alpha; 
} 

vmath::vec3 Spline3D::evaluatePosition(float globalT) 
{
    int numSegments = positions.size() - 1; 
    float scaled = globalT * numSegments; 
    int segment = vmath::clamp((int)floor(scaled), 0, numSegments - 1); 
    float localT = scaled - segment; 

    vmath::vec3 P0 = getPoint(positions, segment-1); 
    vmath::vec3 P1 = getPoint(positions, segment); 
    vmath::vec3 P2 = getPoint(positions, segment+1); 
    vmath::vec3 P3 = getPoint(positions, segment+2); 

    return (catmullRom(P0, P1, P2, P3, localT, this->alpha)); 
}  

vmath::mat4 Spline3D::getViewMatrix(float t) 
{
    vmath::vec3 camPos = evaluatePosition(t); 

    return (vmath::lookat(camPos, vmath::vec3(0.0), vmath::vec3(0.0, 1.0, 0.0))); 
} 

void Spline3D::addRandomControlPoint() 
{
    vmath::vec3 newControlPoint; 
    size_t positionArraySize = positions.size(); 

    // calculate new point position based last and second last point 
    newControlPoint[0] = positions[positionArraySize-1][0] + fabs(positions[positionArraySize-1][0] - positions[positionArraySize-2][0]); 
    newControlPoint[1] = positions[positionArraySize-1][1] + fabs(positions[positionArraySize-1][1] - positions[positionArraySize-2][1]); 
    newControlPoint[2] = positions[positionArraySize-1][2] + fabs(positions[positionArraySize-1][2] - positions[positionArraySize-2][2]); 
    
    positions.push_back(newControlPoint); 
} 

void Spline3D::addControlPointAtPos(vmath::vec3 newControlPointPosition) 
{
    positions.push_back(newControlPointPosition); 
} 

/* 
std::vector<float> Spline3D::buildArcLengthTable(const std::vector<vmath::vec3>& pts, int samples = 1000) 
{
    std::vector<float> table(samples+1); 
    table[0] = 0.0f; 
    vmath::vec3 prev = evaluatePosition(pts, 0.0f); 

    for(int i = 1; i <= samples; ++i) 
    {
        float t = (float)i / samples; 
        vmath::vec3 curr = evaluatePosition(pts, t); 
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
    float prev = evaluatePosition(pts, 0.0f); 

    for(int i = 1; i <= samples; ++i) 
    {
        float t = (float)i / samples; 
        float curr = evaluatePosition(pts, t); 
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

