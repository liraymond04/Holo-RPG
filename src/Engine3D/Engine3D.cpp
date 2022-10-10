#include "Engine3D/Engine3D.h"

void Engine3D::Init(Game* g) {
    game = g;

	meshCube.LoadFromObj("assets/models/ame.obj");
	matProj = mat4x4_MakeProjection(90.0f, (float)g->ScreenHeight() / (float)g->ScreenWidth(), 0.1f, 1000.0f);
}

bool Engine3D::Update(float fElapsedTime) {
	vec3d vUp = { 0, 1, 0 };

	vec3d vForward = vLookDir* (fCameraSpeed * fElapsedTime);
	vec3d vRight = vec3d_CrossProduct(vUp, vForward);

	if (game->GetKey(olc::W).bHeld) {
		vCamera += vForward;
	}
	if (game->GetKey(olc::S).bHeld) {
		vCamera -= vForward;
	}
	if (game->GetKey(olc::A).bHeld) {
		vCamera += vRight;
	}
	if (game->GetKey(olc::D).bHeld) {
		vCamera -= vRight;
	}

	if (game->GetKey(olc::UP).bHeld) {
		vCamera.y += fCameraSpeed * fElapsedTime;
	}
	if (game->GetKey(olc::DOWN).bHeld) {
		vCamera.y -= fCameraSpeed * fElapsedTime;
	}
	if (game->GetKey(olc::LEFT).bHeld) {
		fYaw -= fTurnSpeed * fElapsedTime;
	}
	if (game->GetKey(olc::RIGHT).bHeld) {
		fYaw += fTurnSpeed * fElapsedTime;
	}

	if (game->GetKey(olc::J).bHeld) {
		fTheta -= fTurnSpeed * fElapsedTime;
	}
	if (game->GetKey(olc::K).bHeld) {
		fTheta += fTurnSpeed * fElapsedTime;
	}

	mat4x4 matRotZ, matRotX;
	//fTheta += 1.0f * fElapsedTime;
	matRotZ = mat4x4_MakeRotationZ(0);
	matRotX = mat4x4_MakeRotationY(fTheta);

	mat4x4 matTrans;
	matTrans = mat4x4_MakeTranslation(0.0f, 0.0f, 5.0f);

	mat4x4 matWorld;
	matWorld = mat4x4_MakeIdentity();
	matWorld = matRotZ * matRotX;
	matWorld = matWorld * matTrans;

	vec3d vTarget = { 0, 0, 1 };
	mat4x4 matCameraRot = mat4x4_MakeRotationY(fYaw);
	vLookDir = matCameraRot * vTarget;
	vTarget = vCamera + vLookDir;
	mat4x4 matCamera = mat4x4_PointAt(vCamera, vTarget, vUp);

	mat4x4 matView = mat4x4_QuickInverse(matCamera);

	std::vector<triangle> vecTrianglesToRaster;

	for (auto &tri : meshCube.tris) {
		triangle triProjected, triTransformed, triViewed;

		triTransformed.p[0] = matWorld * tri.p[0];
		triTransformed.p[1] = matWorld * tri.p[1];
		triTransformed.p[2] = matWorld * tri.p[2];

		vec3d normal, line1, line2;

		line1 = triTransformed.p[1] - triTransformed.p[0];
		line2 = triTransformed.p[2] - triTransformed.p[0];

		normal = vec3d_CrossProduct(line1, line2);
		normal = vec3d_Normalise(normal);
		
		vec3d vCameraRay = triTransformed.p[0] - vCamera;

		if (vec3d_DotProduct(normal, vCameraRay) < 0.0f) {
			vec3d light_direction = { 0.0f, 1.0f, -1.0f };
			light_direction = vec3d_Normalise(light_direction);

			float dp = std::max(0.1f, vec3d_DotProduct(light_direction, normal));

			olc::Pixel c = GetColour(dp);
			triTransformed.col = c;

			triViewed.p[0] = matView * triTransformed.p[0];
			triViewed.p[1] = matView * triTransformed.p[1];
			triViewed.p[2] = matView * triTransformed.p[2];
			triViewed.col = triTransformed.col;

			int nClippedTriangles = 0;
			triangle clipped[2];
			nClippedTriangles = triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

			for (int n = 0; n < nClippedTriangles; n++) {
				triProjected.p[0] = matProj * clipped[n].p[0];
				triProjected.p[1] = matProj * clipped[n].p[1];
				triProjected.p[2] = matProj * clipped[n].p[2];
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

				vec3d vOffsetView = { 1,1,0 };
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

	sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle &t1, triangle &t2) {
		float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
		float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
		return z1 > z2;
	});

	// Clear Screen
	// Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

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
				case 0:	nTrisToAdd = triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 1:	nTrisToAdd = triangle_ClipAgainstPlane({ 0.0f, (float)game->ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 2:	nTrisToAdd = triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 3:	nTrisToAdd = triangle_ClipAgainstPlane({ (float)game->ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				}

				for (int w = 0; w < nTrisToAdd; w++)
					listTriangles.push_back(clipped[w]);
			}
			nNewTriangles = listTriangles.size();
		}

		for (auto &t : listTriangles) {
			game->FillTriangle({(int)t.p[0].x, (int)t.p[0].y}, {(int)t.p[1].x, (int)t.p[1].y}, {(int)t.p[2].x, (int)t.p[2].y}, t.col);
		}
	}

	return true;
}