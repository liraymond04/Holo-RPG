#include "Game.h"
#include "GameState/Play/PlayScene.h"

#include "Engine3D/Camera.h"

#define PI_BY_2 M_PI / 2

void PlayScene::Init(Game* g, GameState* s) {
    game = g; state = s;
    e3d.Init(game);
    std::cout << sSceneName << " scene initialized" << "\n";
}

void PlayScene::Cleanup() {
    std::cout << sSceneName << " scene cleanup" << "\n";
}

void PlayScene::Pause() {
    std::cout << sSceneName << " scene paused" << "\n";
}

void PlayScene::Resume() {
    std::cout << sSceneName << " scene resumed" << "\n";
}

bool PlayScene::HandleEvents(float fElapsedTime) {
    vec3d vUp = { 0, 1, 0 };
	vec3d vForward = e3d.camera->vLookDir * (e3d.camera->fMoveSpeed * fElapsedTime);
	vec3d vRight = vec3d_CrossProduct(vUp, vForward);

    if (game->GetKey(olc::W).bHeld) {
		e3d.camera->vPos += vForward;
	}
	if (game->GetKey(olc::S).bHeld) {
		e3d.camera->vPos -= vForward;
	}
	if (game->GetKey(olc::A).bHeld) {
		e3d.camera->vPos += vRight;
	}
	if (game->GetKey(olc::D).bHeld) {
		e3d.camera->vPos -= vRight;
	}
	if (game->GetKey(olc::SPACE).bHeld) {
		e3d.camera->vPos.y += e3d.camera->fMoveSpeed * fElapsedTime;
	}
	if (game->GetKey(olc::SHIFT).bHeld) {
		e3d.camera->vPos.y -= e3d.camera->fMoveSpeed * fElapsedTime;
	}

	auto pitch_clamp = [](float fAngle, float min, float max, float margin) {
		if (fAngle < min) return min + margin;
		else if (fAngle > max) return max - margin;
		return fAngle;
	};

	if (game->GetKey(olc::UP).bHeld) {
		e3d.camera->fPitch -= e3d.camera->fTurnSpeed * fElapsedTime;
	}
	if (game->GetKey(olc::DOWN).bHeld) {
		e3d.camera->fPitch += e3d.camera->fTurnSpeed * fElapsedTime;
	}
	if (game->GetKey(olc::LEFT).bHeld) {
		e3d.camera->fYaw -= e3d.camera->fTurnSpeed * fElapsedTime;
	}
	if (game->GetKey(olc::RIGHT).bHeld) {
		e3d.camera->fYaw += e3d.camera->fTurnSpeed * fElapsedTime;
	}
	e3d.camera->fPitch = pitch_clamp(e3d.camera->fPitch, -PI_BY_2, PI_BY_2, 0.001f);

    return true;
}

bool PlayScene::Update(float fElapsedTime) {
    game->Clear(olc::BLACK);
    return e3d.Update(fElapsedTime);
}

bool PlayScene::Draw(float fElapsedTime) {
    return true;
}