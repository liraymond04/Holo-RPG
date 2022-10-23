#ifndef ENGINE3D_H
#define ENGINE3D_H

#include "RPG.h"
#include "Engine3D/Math.h"
#include "Engine3D/GameObject.h"

class Camera;
class Shader;
struct MVPTransform;

/*
TODO:
- change z sort to depth buffer
- lighting system and shadow pass
*/

class Engine3D {
  public:
    Engine3D() {}

  private:
    Holo::RPG *game;

    std::vector<GameObject *> vRenderBuffer;
    int bufferSize;
    float *depthBuffer = nullptr;

    GameObject *gameObject;
    mat4x4 matProj;
    float fFov = 90.0f;
    float fNear = 0.1f;
    float fFar = 1000.0f;

    float fTheta = 0.0f;
    vec3d vScale = { 1.0f, 1.0f, 1.0f };

    void ClipAndRasterize(const std::vector<triangle> &vecTrianglesToRaster,
                          const olc::Pixel &col, Shader *shader);
    void RasterizeTriangle(triangle &t, olc::Pixel p, Shader *shader);

  public:
    Camera *camera; // possibly change camera to struct
    Shader *shader;

    void Init(Holo::RPG *game);
    bool Update(float fElapsedTime);
};

#endif