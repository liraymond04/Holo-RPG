#ifndef ENGINE3D_SIMPLESHADER_H
#define ENGINE3D_SIMPLESHADER_H

#include "Engine3D/Camera.h"
#include "Engine3D/Shaders/Shader.h"

class SimpleShader : public Shader {
  public:
    SimpleShader() {}

  private:
  public:
    void VertexShader(std::vector<triangle> &vecTrianglesToRaster,
                      const triangle &tri, const MVPTransform &mvp) {
        triangle triProjected, triTransformed, triViewed;

        triTransformed.p[0] = mvp.matModel * tri.p[0];
        triTransformed.p[1] = mvp.matModel * tri.p[1];
        triTransformed.p[2] = mvp.matModel * tri.p[2];

        vec3d normal, line1, line2;

        line1 = triTransformed.p[1] - triTransformed.p[0];
        line2 = triTransformed.p[2] - triTransformed.p[0];

        normal = vec3d_CrossProduct(line1, line2);
        normal = vec3d_Normalise(normal);

        vec3d vCameraRay = triTransformed.p[0] - camera->vPos;

        if (vec3d_DotProduct(normal, vCameraRay) < 0.0f) {
            vec3d light_direction = { 0.0f, 1.0f, -1.0f };
            light_direction = vec3d_Normalise(light_direction);

            float dp =
                std::max(0.1f, vec3d_DotProduct(light_direction, normal));

            olc::Pixel c = GetColour(dp);
            triTransformed.col = c;

            triViewed.p[0] = mvp.matView * triTransformed.p[0];
            triViewed.p[1] = mvp.matView * triTransformed.p[1];
            triViewed.p[2] = mvp.matView * triTransformed.p[2];
            triViewed.col = triTransformed.col;

            int nClippedTriangles = 0;
            triangle clipped[2];
            nClippedTriangles = triangle_ClipAgainstPlane(
                { 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed,
                clipped[0], clipped[1]);

            for (int n = 0; n < nClippedTriangles; n++) {
                triProjected.p[0] = mvp.matProj * clipped[n].p[0];
                triProjected.p[1] = mvp.matProj * clipped[n].p[1];
                triProjected.p[2] = mvp.matProj * clipped[n].p[2];
                triProjected.col = clipped[n].col;

                triProjected.p[0] /= triProjected.p[0].w;
                triProjected.p[1] /= triProjected.p[1].w;
                triProjected.p[2] /= triProjected.p[2].w;

                triProjected.p[0].x *= -1.0f;
                triProjected.p[1].x *= -1.0f;
                triProjected.p[2].x *= -1.0f;
                triProjected.p[0].y *= -1.0f;
                triProjected.p[1].y *= -1.0f;
                triProjected.p[2].y *= -1.0f;

                vec3d vOffsetView = { 1, 1, 0 };
                triProjected.p[0] += vOffsetView;
                triProjected.p[1] += vOffsetView;
                triProjected.p[2] += vOffsetView;
                triProjected.p[0].x *= 0.5f * (float)game->ScreenWidth();
                triProjected.p[0].y *= 0.5f * (float)game->ScreenHeight();
                triProjected.p[1].x *= 0.5f * (float)game->ScreenWidth();
                triProjected.p[1].y *= 0.5f * (float)game->ScreenHeight();
                triProjected.p[2].x *= 0.5f * (float)game->ScreenWidth();
                triProjected.p[2].y *= 0.5f * (float)game->ScreenHeight();

                vecTrianglesToRaster.push_back(triProjected);
            }
        }
    }

    olc::Pixel FragmentShader(olc::Pixel color, const triangle &tri,
                              float barcoord0, float barcoord1,
                              float barcoord2) {
        return tri.col;
    }

    olc::Pixel GetColour(float lum) {
        olc::Pixel col;
        int pixel_bw = (int)(21.0f * lum);
        switch (pixel_bw) {
        case 0:
            col = olc::VERY_DARK_GREY;
            break;

        case 1:
            col = olc::DARK_GREY;
            break;
        case 2:
            col = olc::DARK_GREY;
            break;
        case 3:
            col = olc::DARK_GREY;
            break;
        case 4:
            col = olc::DARK_GREY;
            break;

        case 5:
            col = olc::DARK_GREY;
            break;
        case 6:
            col = olc::DARK_GREY;
            break;
        case 7:
            col = olc::DARK_GREY;
            break;
        case 8:
            col = olc::DARK_GREY;
            break;

        case 9:
            col = olc::GREY;
            break;
        case 10:
            col = olc::GREY;
            break;
        case 11:
            col = olc::GREY;
            break;
        case 12:
            col = olc::GREY;
            break;

        case 13:
            col = olc::GREY;
            break;
        case 14:
            col = olc::GREY;
            break;
        case 15:
            col = olc::GREY;
            break;
        case 16:
            col = olc::GREY;
            break;

        case 17:
            col = olc::WHITE;
            break;
        case 18:
            col = olc::WHITE;
            break;
        case 19:
            col = olc::WHITE;
            break;
        case 20:
            col = olc::WHITE;
            break;
        default:
            col = olc::BLACK;
        }

        return col;
    }
};

#endif