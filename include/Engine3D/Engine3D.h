#ifndef ENGINE3D_H
#define ENGINE3D_H

#include "Game.h"

#include <lib/glm/glm.hpp>
#include <lib/glm/ext.hpp>

struct e3d_triangle {
    glm::vec3 p[3];
};

struct e3d_mesh {
    std::vector<e3d_triangle> tris;
};

class Engine3D {
public:
    Engine3D() {}

private:
    Game* game;

    e3d_mesh meshCube;
    glm::mat4 matProj;
    float fTheta;

    void MultiplyMatrixVector(glm::vec3 &i, glm::vec3 &o, glm::mat4 &m)
	{
		o.x = i.x * m[0][0] + i.y * m[1][0] + i.z * m[2][0] + m[3][0];
		o.y = i.x * m[0][1] + i.y * m[1][1] + i.z * m[2][1] + m[3][1];
		o.z = i.x * m[0][2] + i.y * m[1][2] + i.z * m[2][2] + m[3][2];
		float w = i.x * m[0][3] + i.y * m[1][3] + i.z * m[2][3] + m[3][3];
        
		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}
	}

public:
    void Init(Game* game);
    bool Update(float fElapsedTime);
};

#endif