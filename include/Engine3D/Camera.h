#ifndef ENGINE3D_CAMERA_H
#define ENGINE3D_CAMERA_H

#include "Engine3D/Engine3D.h"
#include "RPG.h"

class Camera {
  public:
    Camera() {}

  private:
  public:
    vec3d vPos;
    vec3d vLookDir;
    float fYaw = 0.0f;
    float fPitch = 0.0f;

    float fMoveSpeed = 8.0f;
    float fTurnSpeed = 2.0f;
};

#endif