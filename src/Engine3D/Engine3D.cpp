#include "Engine3D/Engine3D.h"
#include "Engine3D/Camera.h"
#include "Engine3D/Shaders/SimpleShader.h"

void Engine3D::Init(Holo::RPG *g)
{
    game = g;

    meshCube.LoadFromObj("assets/models/mountains.obj");
    vRenderBuffer.push_back(&meshCube);
    // load texture

    camera = new Camera();
    matProj = mat4x4_MakeProjection(90.0f, (float)g->ScreenHeight() / (float)g->ScreenWidth(), 0.1f, 1000.0f);

    shader = new SimpleShader;
    shader->camera = camera;
    shader->game = game;
}

bool Engine3D::Update(float fElapsedTime)
{
    vec3d vUp = {0, 1, 0};

    vec3d vTarget = {0, 0, 1};
    mat4x4 matCameraRot = mat4x4_MakeRotation(camera->fPitch, camera->fYaw, 0.0f);
    camera->vLookDir = matCameraRot * vTarget;
    vTarget = camera->vPos + camera->vLookDir;
    mat4x4 matCamera = mat4x4_PointAt(camera->vPos, vTarget, vUp);

    mat4x4 matView = mat4x4_QuickInverse(matCamera);

    if (game->GetKey(olc::J).bHeld)
    {
        fTheta -= camera->fTurnSpeed * fElapsedTime;
    }
    if (game->GetKey(olc::K).bHeld)
    {
        fTheta += camera->fTurnSpeed * fElapsedTime;
    }

    if (game->GetKey(olc::X).bHeld)
    {
        vScale.x += camera->fTurnSpeed * fElapsedTime * pow(-1, game->GetKey(olc::CTRL).bHeld);
    }
    if (game->GetKey(olc::Y).bHeld)
    {
        vScale.y += camera->fTurnSpeed * fElapsedTime * pow(-1, game->GetKey(olc::CTRL).bHeld);
    }
    if (game->GetKey(olc::Z).bHeld)
    {
        vScale.z += camera->fTurnSpeed * fElapsedTime * pow(-1, game->GetKey(olc::CTRL).bHeld);
    }

    // depth buffer
    // lighting system

    std::vector<triangle> vecTrianglesToRaster;

    for (const mesh* m : vRenderBuffer) {
        mat4x4 matScale, matRot, matTrans, matWorld;
        // fTheta += 1.0f * fElapsedTime;
        matScale = mat4x4_MakeScale(vScale);
        matRot = mat4x4_MakeRotation(0.0f, fTheta, 0.0f);
        matTrans = mat4x4_MakeTranslation(0.0f, 0.0f, 5.0f);
        matWorld = matScale * matRot * matTrans;

        MVPTransform mvp = {matWorld, matView, matProj};

        for (const triangle &tri : m->tris) {
            shader->VertexShader(vecTrianglesToRaster, tri, mvp);
        }
    }

    // replace z sort with a depth buffer
    sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle &t1, triangle &t2) {
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
        return z1 > z2;
    });

    ClipAndRasterize(vecTrianglesToRaster, olc::WHITE, shader);

    // Clear Screen
    // Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

    return true;
}

void Engine3D::ClipAndRasterize(const std::vector<triangle> &vecTrianglesToRaster, const olc::Pixel &col, Shader* shader) {
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

                switch (p)
                {
                case 0:
                    nTrisToAdd = triangle_ClipAgainstPlane({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, test, clipped[0], clipped[1]);
                    break;
                case 1:
                    nTrisToAdd = triangle_ClipAgainstPlane({0.0f, (float)game->ScreenHeight() - 1, 0.0f}, {0.0f, -1.0f, 0.0f}, test, clipped[0], clipped[1]);
                    break;
                case 2:
                    nTrisToAdd = triangle_ClipAgainstPlane({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, test, clipped[0], clipped[1]);
                    break;
                case 3:
                    nTrisToAdd = triangle_ClipAgainstPlane({(float)game->ScreenWidth() - 1, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, test, clipped[0], clipped[1]);
                    break;
                }

                for (int w = 0; w < nTrisToAdd; w++)
                    listTriangles.push_back(clipped[w]);
            }
            nNewTriangles = listTriangles.size();
        }

        for (auto &t : listTriangles) {
            // game->FillTriangle({(int)t.p[0].x, (int)t.p[0].y}, {(int)t.p[1].x, (int)t.p[1].y}, {(int)t.p[2].x, (int)t.p[2].y}, t.col);
            RasterizeTriangle(t, olc::WHITE, shader);
        }
    }
}

void Engine3D::RasterizeTriangle(triangle &t, olc::Pixel p, Shader *shader)
{
    int32_t x1 = t.p[0].x, y1 = t.p[0].y, x2 = t.p[1].x, y2 = t.p[1].y, x3 = t.p[2].x, y3 = t.p[2].y;

    auto drawline = [&](int sx, int ex, int ny)
    {
        float u, v, w;
        for (int i = sx; i <= ex; i++)
        {
            t.Barycentric(i, ny, u, v, w);
            game->Draw(i, ny, shader->FragmentShader(p, t, u, v, w));
        }
    };

    int t1x, t2x, y, minx, maxx, t1xp, t2xp;
    bool changed1 = false;
    bool changed2 = false;
    int signx1, signx2, dx1, dy1, dx2, dy2;
    int e1, e2;
    // Sort vertices
    if (y1 > y2)
    {
        std::swap(y1, y2);
        std::swap(x1, x2);
    }
    if (y1 > y3)
    {
        std::swap(y1, y3);
        std::swap(x1, x3);
    }
    if (y2 > y3)
    {
        std::swap(y2, y3);
        std::swap(x2, x3);
    }

    t1x = t2x = x1;
    y = y1; // Starting points
    dx1 = (int)(x2 - x1);
    if (dx1 < 0)
    {
        dx1 = -dx1;
        signx1 = -1;
    }
    else
        signx1 = 1;
    dy1 = (int)(y2 - y1);

    dx2 = (int)(x3 - x1);
    if (dx2 < 0)
    {
        dx2 = -dx2;
        signx2 = -1;
    }
    else
        signx2 = 1;
    dy2 = (int)(y3 - y1);

    if (dy1 > dx1)
    {
        std::swap(dx1, dy1);
        changed1 = true;
    }
    if (dy2 > dx2)
    {
        std::swap(dy2, dx2);
        changed2 = true;
    }

    e2 = (int)(dx2 >> 1);
    // Flat top, just process the second half
    if (y1 == y2)
        goto next;
    e1 = (int)(dx1 >> 1);

    for (int i = 0; i < dx1;)
    {
        t1xp = 0;
        t2xp = 0;
        if (t1x < t2x)
        {
            minx = t1x;
            maxx = t2x;
        }
        else
        {
            minx = t2x;
            maxx = t1x;
        }
        // process first line until y value is about to change
        while (i < dx1)
        {
            i++;
            e1 += dy1;
            while (e1 >= dx1)
            {
                e1 -= dx1;
                if (changed1)
                    t1xp = signx1; // t1x += signx1;
                else
                    goto next1;
            }
            if (changed1)
                break;
            else
                t1x += signx1;
        }
        // Move line
    next1:
        // process second line until y value is about to change
        while (1)
        {
            e2 += dy2;
            while (e2 >= dx2)
            {
                e2 -= dx2;
                if (changed2)
                    t2xp = signx2; // t2x += signx2;
                else
                    goto next2;
            }
            if (changed2)
                break;
            else
                t2x += signx2;
        }
    next2:
        if (minx > t1x)
            minx = t1x;
        if (minx > t2x)
            minx = t2x;
        if (maxx < t1x)
            maxx = t1x;
        if (maxx < t2x)
            maxx = t2x;
        drawline(minx, maxx, y); // Draw line from min to max points found on the y
                                 // Now increase y
        if (!changed1)
            t1x += signx1;
        t1x += t1xp;
        if (!changed2)
            t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y == y2)
            break;
    }
next:
    // Second half
    dx1 = (int)(x3 - x2);
    if (dx1 < 0)
    {
        dx1 = -dx1;
        signx1 = -1;
    }
    else
        signx1 = 1;
    dy1 = (int)(y3 - y2);
    t1x = x2;

    if (dy1 > dx1)
    { // swap values
        std::swap(dy1, dx1);
        changed1 = true;
    }
    else
        changed1 = false;

    e1 = (int)(dx1 >> 1);

    for (int i = 0; i <= dx1; i++)
    {
        t1xp = 0;
        t2xp = 0;
        if (t1x < t2x)
        {
            minx = t1x;
            maxx = t2x;
        }
        else
        {
            minx = t2x;
            maxx = t1x;
        }
        // process first line until y value is about to change
        while (i < dx1)
        {
            e1 += dy1;
            while (e1 >= dx1)
            {
                e1 -= dx1;
                if (changed1)
                {
                    t1xp = signx1;
                    break;
                } // t1x += signx1;
                else
                    goto next3;
            }
            if (changed1)
                break;
            else
                t1x += signx1;
            if (i < dx1)
                i++;
        }
    next3:
        // process second line until y value is about to change
        while (t2x != x3)
        {
            e2 += dy2;
            while (e2 >= dx2)
            {
                e2 -= dx2;
                if (changed2)
                    t2xp = signx2;
                else
                    goto next4;
            }
            if (changed2)
                break;
            else
                t2x += signx2;
        }
    next4:

        if (minx > t1x)
            minx = t1x;
        if (minx > t2x)
            minx = t2x;
        if (maxx < t1x)
            maxx = t1x;
        if (maxx < t2x)
            maxx = t2x;
        drawline(minx, maxx, y);
        if (!changed1)
            t1x += signx1;
        t1x += t1xp;
        if (!changed2)
            t2x += signx2;
        t2x += t2xp;
        y += 1;
        if (y > y3)
            return;
    }
}

void Engine3D::Barycentric(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, float &u, float &v, float &w)
{
    vec3d p = { (float)x0, (float)y0 }, a = { (float)x1, (float)y1 }, b = { (float)x2, (float)y2 }, c = { (float)x3, (float)y3 };
    vec3d v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = vec3d_DotProduct(v0, v0);
    float d01 = vec3d_DotProduct(v0, v1);
    float d11 = vec3d_DotProduct(v1, v1);
    float d20 = vec3d_DotProduct(v2, v0);
    float d21 = vec3d_DotProduct(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
}