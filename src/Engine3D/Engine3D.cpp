#include "Engine3D/Engine3D.h"
#include "Engine3D/Camera.h"
#include "Engine3D/Shaders/SimpleShader.h"

void Engine3D::Init(Holo::RPG *g) {
    game = g;

    gameObject = new GameObject;
    mesh *m = new mesh;
    m->LoadFromObj("assets/models/mountains.obj");
    gameObject->_mesh = m;
    // load texture

    vRenderBuffer.push_back(gameObject);
    bufferSize = game->ScreenWidth() * game->ScreenHeight();
    depthBuffer = new float[bufferSize];

    camera = new Camera();
    matProj = mat4x4_MakeProjection(
        fFov, (float)g->ScreenHeight() / (float)g->ScreenWidth(), fNear, fFar);

    shader = new SimpleShader;
    shader->camera = camera;
    shader->game = game;
}

bool Engine3D::Update(float fElapsedTime) {
    vec3d vUp = { 0, 1, 0 };

    vec3d vTarget = { 0, 0, 1 };
    mat4x4 matCameraRot =
        mat4x4_MakeRotation(camera->fPitch, camera->fYaw, 0.0f);
    camera->vLookDir = matCameraRot * vTarget;
    vTarget = camera->vPos + camera->vLookDir;
    mat4x4 matCamera = mat4x4_PointAt(camera->vPos, vTarget, vUp);

    mat4x4 matView = mat4x4_QuickInverse(matCamera);

    if (game->GetKey(olc::J).bHeld) {
        gameObject->rotation.y -= camera->fTurnSpeed * fElapsedTime;
    }
    if (game->GetKey(olc::K).bHeld) {
        gameObject->rotation.y += camera->fTurnSpeed * fElapsedTime;
    }

    if (game->GetKey(olc::X).bHeld) {
        gameObject->scale.x += camera->fTurnSpeed * fElapsedTime *
                               pow(-1, game->GetKey(olc::CTRL).bHeld);
    }
    if (game->GetKey(olc::Y).bHeld) {
        gameObject->scale.y += camera->fTurnSpeed * fElapsedTime *
                               pow(-1, game->GetKey(olc::CTRL).bHeld);
    }
    if (game->GetKey(olc::Z).bHeld) {
        gameObject->scale.z += camera->fTurnSpeed * fElapsedTime *
                               pow(-1, game->GetKey(olc::CTRL).bHeld);
    }

    // lighting system

    std::vector<triangle> vecTrianglesToRaster;

    for (const GameObject *go : vRenderBuffer) {
        mesh *m = go->_mesh;

        mat4x4 matScale, matRot, matTrans, matWorld;
        // fTheta += 1.0f * fElapsedTime;
        matScale = mat4x4_MakeScale(go->scale);
        matRot = mat4x4_MakeRotation(go->rotation);
        matTrans = mat4x4_MakeTranslation(go->position);
        matWorld = matScale * matRot * matTrans;

        MVPTransform mvp = { matWorld, matView, matProj };

        for (const triangle &tri : m->tris) {
            shader->VertexShader(vecTrianglesToRaster, tri, mvp);
        }
    }

    memset(depthBuffer, 0.0f, sizeof(float) * bufferSize);
    ClipAndRasterize(vecTrianglesToRaster, olc::WHITE, shader);

    // Clear Screen
    // Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

    return true;
}

void Engine3D::ClipAndRasterize(
    const std::vector<triangle> &vecTrianglesToRaster, const olc::Pixel &col,
    Shader *shader) {
    for (auto &triToRaster : vecTrianglesToRaster) {
        triangle clipped[2];
        std::list<triangle> listTriangles;

        listTriangles.push_back(triToRaster);
        int nNewTriangles = 1;

        for (int p = 0; p < 4; p++) {
            int nTrisToAdd = 0;
            while (nNewTriangles > 0) {
                triangle test = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles--;

                switch (p) {
                case 0:
                    nTrisToAdd = triangle_ClipAgainstPlane(
                        { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test,
                        clipped[0], clipped[1]);
                    break;
                case 1:
                    nTrisToAdd = triangle_ClipAgainstPlane(
                        { 0.0f, (float)game->ScreenHeight() - 1, 0.0f },
                        { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]);
                    break;
                case 2:
                    nTrisToAdd = triangle_ClipAgainstPlane(
                        { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test,
                        clipped[0], clipped[1]);
                    break;
                case 3:
                    nTrisToAdd = triangle_ClipAgainstPlane(
                        { (float)game->ScreenWidth() - 1, 0.0f, 0.0f },
                        { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
                    break;
                }

                for (int w = 0; w < nTrisToAdd; w++)
                    listTriangles.push_back(clipped[w]);
            }
            nNewTriangles = listTriangles.size();
        }

        for (auto &t : listTriangles) {
            // game->FillTriangle({(int)t.p[0].x, (int)t.p[0].y},
            //                    {(int)t.p[1].x, (int)t.p[1].y},
            //                    {(int)t.p[2].x, (int)t.p[2].y}, t.col);
            RasterizeTriangle(t, olc::WHITE, shader);
        }
    }
}