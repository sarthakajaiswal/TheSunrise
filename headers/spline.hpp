#pragma once 

#include <iostream> 
#include <vector> 
#include <cstdio> 

#include "vmath.h"  

// requires at least 4 points 
class Spline3D 
{
    private: 
        std::vector<vmath::vec3> positions; 
        float alpha = 0.5f; // 0.5 -> centripetal catmull-rom spline 

        float getKnotValue(float t, float alpha, vmath::vec3 p0, vmath::vec3 p1); 
        vmath::vec3 catmullRom(vmath::vec3 p0, vmath::vec3 p1, vmath::vec3 p2, vmath::vec3 p3, float t, float alpha);
        vmath::vec3 getPoint(const std::vector<vmath::vec3>& pts, int index);

    public: 
        Spline3D(std::vector<vmath::vec3> controlPointsArray); 

        void setAlpha(float _alpha); 
        vmath::mat4 getViewMatrix(float t); 
        
        vmath::vec3 evaluatePosition(float globalT); 
        void addRandomControlPoint(); 
        void addControlPointAtPos(vmath::vec3 newControlPointPosition); 

        /* 
        std::vector<float> buildArcLengthTable(const std::vector<vmath::vec3>& pts, int samples = 1000); 
        float arcLengthToT(const std::vector<float>& table, float targetLen); 
        */ 
}; 

class Spline1D 
{
    private: 
        std::vector<float> values; 
        float alpha = 0.5f; // 0.5 -> centripetal catmull-rom spline 

        float getKnotValue(float t, float alpha, float v0, float v1); 
        float catmullRom(float v0, float v1, float v2, float v3, float t, float alpha);
        float getValue(const std::vector<float>& values, int index);

    public: 
        Spline1D(std::vector<float> valuesArray); 

        void setAlpha(float _alpha); 
        
        float evaluateValue(float globalT); 
        void addRandomValue(); 
        void addGivenValue(float newValue); 

        /* 
        std::vector<float> buildArcLengthTable(const std::vector<vmath::vec3>& pts, int samples = 1000); 
        float arcLengthToT(const std::vector<float>& table, float targetLen); 
        */ 
};

