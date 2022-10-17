#ifndef ENGINE3D_MATH_H
#define ENGINE3D_MATH_H

#include "Game.h"

struct vec3d
{
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;

    friend vec3d operator+(const vec3d &v1, const vec3d &v2)
    {
        return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    }

    vec3d &operator+=(const vec3d &v)
    {
        *this = *this + v;
        return *this;
    }

    friend vec3d operator-(const vec3d &v1, const vec3d &v2)
    {
        return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    }

    vec3d &operator-=(const vec3d &v)
    {
        *this = *this - v;
        return *this;
    }

    friend vec3d operator*(const vec3d v1, float k)
    {
        return {v1.x * k, v1.y * k, v1.z * k};
    }

    vec3d &operator*=(float k)
    {
        *this = *this * k;
        return *this;
    }

    friend vec3d operator/(const vec3d &v1, float k)
    {
        return {v1.x / k, v1.y / k, v1.z / k};
    }

    vec3d &operator/=(float k)
    {
        *this = *this / k;
        return *this;
    }
};

struct mat4x4
{
    float m[4][4] = {0};

    friend mat4x4 operator*(const mat4x4 &m1, const mat4x4 &m2)
    {
        mat4x4 matrix;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
        return matrix;
    }

    mat4x4 &operator*=(const mat4x4 &m)
    {
        *this = *this * m;
        return *this;
    }

    friend vec3d operator*(const mat4x4 &m, const vec3d &i)
    {
        vec3d v;
        v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
        v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
        v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
        v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
        return v;
    }
};

static float vec3d_DotProduct(vec3d &v1, vec3d &v2);
struct triangle
{
    vec3d p[3];
    olc::Pixel col;

    vec3d a, b, c;
    vec3d v0, v1;
    float d00, d01, d11;
    float denom;

    triangle() {
        Init();
    }

    triangle(vec3d p0, vec3d p1, vec3d p2) {
        p[0] = p0;
        p[1] = p1;
        p[2] = p2;
        Init();
    }

    void Init() {
        a = {p[0].x, p[0].y};
        b = {p[1].x, p[1].y};
        c = {p[2].x, p[2].y};
        v0 = b - a, v1 = c - a;
        d00 = vec3d_DotProduct(v0, v0);
        d01 = vec3d_DotProduct(v0, v1);
        d11 = vec3d_DotProduct(v1, v1);
        denom = 1.0f / (d00 * d11 - d01 * d01);
    }

    void Barycentric(int32_t x0, int32_t y0, float &u, float &v, float &w)
    {
        vec3d p = {(float)x0, (float)y0};
        vec3d v2 = p - a;
        float d20 = vec3d_DotProduct(v2, v0);
        float d21 = vec3d_DotProduct(v2, v1);

        v = (d11 * d20 - d01 * d21) * denom;
        w = (d00 * d21 - d01 * d20) * denom;
        u = 1.0f - v - w;
    }
};

struct mesh
{
    std::vector<triangle> tris;

    bool LoadFromObj(std::string sFileName)
    {
        std::ifstream f(sFileName);
        if (!f.is_open())
            return false;

        std::vector<vec3d> verts;

        while (!f.eof())
        {
            char line[128];
            f.getline(line, 128);

            std::strstream s;
            s << line;

            char junk;

            if (line[0] == 'v')
            {
                vec3d v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }
            if (line[0] == 'f')
            {
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];
                tris.push_back({verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]});
            }
        }

        return true;
    }
};

static float vec3d_DotProduct(vec3d &v1, vec3d &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static float vec3d_Length(vec3d &v)
{
    return sqrtf(vec3d_DotProduct(v, v));
}

static vec3d vec3d_Normalise(vec3d &v)
{
    float l = vec3d_Length(v);
    return {v.x / l, v.y / l, v.z / l};
}

static vec3d vec3d_CrossProduct(vec3d &v1, vec3d &v2)
{
    vec3d v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}

static vec3d vec3d_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd)
{
    plane_n = vec3d_Normalise(plane_n);
    float plane_d = -vec3d_DotProduct(plane_n, plane_p);
    float ad = vec3d_DotProduct(lineStart, plane_n);
    float bd = vec3d_DotProduct(lineEnd, plane_n);
    float t = (-plane_d - ad) / (bd - ad);
    vec3d lineStartToEnd = lineEnd - lineStart;
    vec3d lineToIntersect = lineStartToEnd * t;
    return lineStart + lineToIntersect;
}

static mat4x4 mat4x4_MakeIdentity()
{
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

static mat4x4 mat4x4_MakeScale(float x, float y, float z)
{
    mat4x4 matrix;
    matrix.m[0][0] = x;
    matrix.m[1][1] = y;
    matrix.m[2][2] = z;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

static mat4x4 mat4x4_MakeScale(const vec3d &v)
{
    return mat4x4_MakeScale(v.x, v.y, v.z);
}

static mat4x4 mat4x4_MakeRotationX(float fAngleRad)
{
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[1][2] = sinf(fAngleRad);
    matrix.m[2][1] = -sinf(fAngleRad);
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

static mat4x4 mat4x4_MakeRotationY(float fAngleRad)
{
    mat4x4 matrix;
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][2] = sinf(fAngleRad);
    matrix.m[2][0] = -sinf(fAngleRad);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

static mat4x4 mat4x4_MakeRotationZ(float fAngleRad)
{
    mat4x4 matrix;
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][1] = sinf(fAngleRad);
    matrix.m[1][0] = -sinf(fAngleRad);
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

static mat4x4 mat4x4_MakeRotation(float x, float y, float z)
{
    return mat4x4_MakeRotationX(x) * mat4x4_MakeRotationY(y) * mat4x4_MakeRotationY(y);
}

static mat4x4 mat4x4_MakeRotation(const vec3d &v)
{
    return mat4x4_MakeRotation(v.x, v.y, v.z);
}

static mat4x4 mat4x4_MakeTranslation(float x, float y, float z)
{
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    return matrix;
}

static mat4x4 mat4x4_MakeTranslation(const vec3d &v)
{
    return mat4x4_MakeTranslation(v.x, v.y, v.z);
}

static mat4x4 mat4x4_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
    float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
    mat4x4 matrix;
    matrix.m[0][0] = fAspectRatio * fFovRad;
    matrix.m[1][1] = fFovRad;
    matrix.m[2][2] = fFar / (fFar - fNear);
    matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;
    return matrix;
}

static mat4x4 mat4x4_PointAt(vec3d &pos, vec3d &target, vec3d &up)
{
    vec3d newForward = target - pos;
    newForward = vec3d_Normalise(newForward);

    vec3d a = newForward * vec3d_DotProduct(up, newForward);
    vec3d newUp = up - a;
    newUp = vec3d_Normalise(newUp);

    vec3d newRight = vec3d_CrossProduct(newUp, newForward);

    mat4x4 matrix;
    matrix.m[0][0] = newRight.x;
    matrix.m[0][1] = newRight.y;
    matrix.m[0][2] = newRight.z;
    matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = newUp.x;
    matrix.m[1][1] = newUp.y;
    matrix.m[1][2] = newUp.z;
    matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = newForward.x;
    matrix.m[2][1] = newForward.y;
    matrix.m[2][2] = newForward.z;
    matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = pos.x;
    matrix.m[3][1] = pos.y;
    matrix.m[3][2] = pos.z;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

static mat4x4 mat4x4_QuickInverse(mat4x4 &m)
{
    mat4x4 matrix;
    matrix.m[0][0] = m.m[0][0];
    matrix.m[0][1] = m.m[1][0];
    matrix.m[0][2] = m.m[2][0];
    matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = m.m[0][1];
    matrix.m[1][1] = m.m[1][1];
    matrix.m[1][2] = m.m[2][1];
    matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = m.m[0][2];
    matrix.m[2][1] = m.m[1][2];
    matrix.m[2][2] = m.m[2][2];
    matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
    matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
    matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

static int triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2)
{
    plane_n = vec3d_Normalise(plane_n);

    auto dist = [&](vec3d &p)
    {
        vec3d n = vec3d_Normalise(p);
        return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - vec3d_DotProduct(plane_n, plane_p));
    };

    vec3d *inside_points[3];
    int nInsidePointCount = 0;
    vec3d *outside_points[3];
    int nOutsidePointCount = 0;

    float d0 = dist(in_tri.p[0]);
    float d1 = dist(in_tri.p[1]);
    float d2 = dist(in_tri.p[2]);

    if (d0 >= 0)
        inside_points[nInsidePointCount++] = &in_tri.p[0];
    else
        outside_points[nOutsidePointCount++] = &in_tri.p[0];
    if (d1 >= 0)
        inside_points[nInsidePointCount++] = &in_tri.p[1];
    else
        outside_points[nOutsidePointCount++] = &in_tri.p[1];
    if (d2 >= 0)
        inside_points[nInsidePointCount++] = &in_tri.p[2];
    else
        outside_points[nOutsidePointCount++] = &in_tri.p[2];

    if (nInsidePointCount == 0)
    {
        return 0;
    }
    if (nInsidePointCount == 3)
    {
        out_tri1 = in_tri;

        return 1;
    }
    if (nInsidePointCount == 1 && nOutsidePointCount == 2)
    {
        out_tri1.col = in_tri.col;

        out_tri1.p[0] = *inside_points[0];
        out_tri1.p[1] = vec3d_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
        out_tri1.p[2] = vec3d_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

        return 1;
    }
    if (nInsidePointCount == 2 && nOutsidePointCount == 1)
    {
        out_tri1.col = in_tri.col;
        out_tri2.col = in_tri.col;

        out_tri1.p[0] = *inside_points[0];
        out_tri1.p[1] = *inside_points[1];
        out_tri1.p[2] = vec3d_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

        out_tri2.p[0] = *inside_points[1];
        out_tri2.p[1] = out_tri1.p[2];
        out_tri2.p[2] = vec3d_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

        return 2;
    }

    return 0;
}

#endif