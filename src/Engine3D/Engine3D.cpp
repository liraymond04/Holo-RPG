#include "Engine3D/Engine3D.h"

void Engine3D::Init(Game* g) {
    game = g;

    meshCube.tris = {
		// SOUTH
		{ glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 0.0f) },
		{ glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 0.0f),    glm::vec3(1.0f, 0.0f, 0.0f) },

		// EAST                                                      
		{ glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(1.0f, 0.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 0.0f, 1.0f) },

		// NORTH                                                     
		{ glm::vec3(1.0f, 0.0f, 1.0f),    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(0.0f, 1.0f, 1.0f) },
		{ glm::vec3(1.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 1.0f, 1.0f),    glm::vec3(0.0f, 0.0f, 1.0f) },

		// WEST                                                      
		{ glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 1.0f, 1.0f),    glm::vec3(0.0f, 1.0f, 0.0f) },
		{ glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec3(0.0f, 0.0f, 0.0f) },

		// TOP                                                       
		{ glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec3(0.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(0.0f, 1.0f, 0.0f),    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 1.0f, 0.0f) },

		// BOTTOM                                                    
		{ glm::vec3(1.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 0.0f, 0.0f) },
		{ glm::vec3(1.0f, 0.0f, 1.0f),    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(1.0f, 0.0f, 0.0f) },
    };

    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)game->ScreenHeight() / (float)game->ScreenWidth();
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    float proj[16] = {
        fAspectRatio * fFovRad, 0.0f, 0.0f, 0.0f,
        0.0f, fFovRad, 0.0f, 0.0f,
        0.0f, 0.0f, fFar / (fFar - fNear), 1.0f,
        0.0f, 0.0f, (-fFar * fNear) / (fFar - fNear), 0.0f
    };
    matProj = glm::make_mat4(proj);
}

bool Engine3D::Update(float fElapsedTime) {
	glm::mat4 matRotZ, matRotX;
	fTheta += 1.0f * fElapsedTime;

	float rotZ[16] = {
        cosf(fTheta), sinf(fTheta), 0.0f, 0.0f,
        -sinf(fTheta), cosf(fTheta), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
	matRotZ = glm::make_mat4(rotZ);

	float rotX[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosf(fTheta * 0.5f), sinf(fTheta * 0.5f), 0.0f,
        0.0f, -sinf(fTheta * 0.5f), cosf(fTheta * 0.5f), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
	matRotX = glm::make_mat4(rotX);

	for (auto tri : meshCube.tris) {
		e3d_triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

		MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
		MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
		MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

		MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
		MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
		MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

		triTranslated = triRotatedZX;
		triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
		triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
		triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

		MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
		MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
		MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

		triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
		triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
		triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
		triProjected.p[0].x *= 0.5f * (float)game->ScreenWidth();
		triProjected.p[0].y *= 0.5f * (float)game->ScreenHeight();
		triProjected.p[1].x *= 0.5f * (float)game->ScreenWidth();
		triProjected.p[1].y *= 0.5f * (float)game->ScreenHeight();
		triProjected.p[2].x *= 0.5f * (float)game->ScreenWidth();
		triProjected.p[2].y *= 0.5f * (float)game->ScreenHeight();

		game->DrawTriangle({triProjected.p[0].x, triProjected.p[0].y},
			{triProjected.p[1].x, triProjected.p[1].y},
			{triProjected.p[2].x, triProjected.p[2].y},
			olc::WHITE);
	}
	

	return true;
}