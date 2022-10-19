#ifndef ENGINE3D_SHADER_H
#define ENGINE3D_SHADER_H

#include "Engine3D/Engine3D.h"

struct MVPTransform {
    const mat4x4& matModel;
    const mat4x4& matView;
    const mat4x4& matProj;
};

class Shader {
protected:
    Shader () {}
    
public:
    Holo::RPG* game;
    Camera* camera;
    virtual void VertexShader(std::vector<triangle>& vecTrianglesToRaster, const triangle& tri, const MVPTransform& mvp) = 0;
    virtual olc::Pixel FragmentShader(olc::Pixel color, const triangle& tri, float barcoord0, float barcoord1, float barcoord2) = 0;
};

#endif