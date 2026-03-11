#pragma once 

#include <iostream> 
#include <vector> 
#include <cstdio> 
#include <cassert> 

#include <gl/glew.h> 
#include <gl/GL.h> 

#include "vmath.h"  
#include "shaderProgram.hpp" 
#include "global_header.hpp" 

// no two corrosponding positions can be same 
class Spline3D 
{
    private: 
        std::vector<vmath::vec3> controlPoints; 
        float alpha = 0.5f; // 0.5 -> centripetal catmull-rom spline 

        float getKnotValue(float t, float alpha, vmath::vec3 p0, vmath::vec3 p1); 
        vmath::vec3 catmullRom(vmath::vec3 p0, vmath::vec3 p1, vmath::vec3 p2, vmath::vec3 p3, float t, float alpha);
        vmath::vec3 getPoint(const std::vector<vmath::vec3>& pts, int index);

        // opengl related 
        ShaderProgram shaderProgram; 
        GLuint mvpMatrixUniform; 
        GLuint vao; 
        GLuint vbo;
        bool isOpenGLStateInitialized; 
        std::vector<vmath::vec3> positionsOnSpline; 
        void initOpenGLState(); 

    public: 
        Spline3D(); 
        Spline3D(std::vector<vmath::vec3> controlPointsArray); 

        void initialize(std::vector<vmath::vec3> controlPointsArray); 

        void setAlpha(float _alpha); 
        vmath::mat4 getViewMatrix(float t); 
        
        vmath::vec3 evaluatePositionAtT(float globalT); 
        void getPositionsOnSpline(std::vector<vmath::vec3>& positions, int count); 

        void show(vmath::mat4 _mvpMatrix); 
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

        void initialize(std::vector<float> valuesArray); 

        void setAlpha(float _alpha); 
        
        float evaluateValueAtT(float globalT); 
        void addNewControlValue(); 
        void addNewControlValue(float newValue); 

        /* 
        std::vector<float> buildArcLengthTable(const std::vector<vmath::vec3>& pts, int samples = 1000); 
        float arcLengthToT(const std::vector<float>& table, float targetLen); 
        */ 
};

