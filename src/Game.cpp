#include "Game.h"
#include "GameState/MainMenu.h"

bool Game::OnUserCreate() {
    ReadConfig();

    audio_result = ma_engine_init(NULL, &audio_engine);
    if (audio_result != MA_SUCCESS) {
        std::cout << "Failed to initialize audio engine" << "\n";
        return false;
    }
    ma_sound_group_init(&audio_engine, 0, NULL, &audio_master);
    ma_sound_group_init(&audio_engine, 0, &audio_master, &audio_sfx);
    ma_sound_group_init(&audio_engine, 0, &audio_master, &audio_music);
    AudioSetConfig();
    
    splashScreen.SetOptions(2, 1, 2.0f, 0.5f, olc::BLACK, olc::BLUE, olc::DARK_GREY, olc::WHITE);
    ChangeState(MainMenu::Instance());

    return true;
}

bool Game::OnUserUpdate(float fElapsedTime) {
    if (splashScreen.AnimateSplashScreen(fElapsedTime)) return true;
    return HandleEvents() &&
        Update() && 
        Draw();
}

void Game::Cleanup() {
    ma_engine_uninit(&audio_engine);
    std::cout << "Finish" << "\n";
}

void Game::ChangeState(GameState* state) {
    if (!states.empty()) {
        states.back()->Cleanup();
        states.pop_back();
    }

    states.push_back(state);
    states.back()->Init(this);
}

void Game::PushState(GameState* state) {
    if (!states.empty()) {
        states.back()->Pause();
    }

    states.push_back(state);
    states.back()->Init(this);
}

void Game::PopState() {
    if (!states.empty()) {
        states.back()->Cleanup();
        states.pop_back();
    }

    if (!states.empty()) {
        states.back()->Resume();
    }
}

void Game::ReadConfig() {
    std::ifstream file;
    file.open("config.json");
    if (file.fail()) {
        std::cout << "Opening config.json failed" << "\n";
        return;
    }
    config = json::parse(file);
    std::cout << "Opening config.json successful" << "\n";
}

void Game::WriteConfig() {
    AudioSetConfig();
    std::ofstream file("config.json");
    file << config.dump(2);
}

void Game::PlaySound(const char* file, ma_sound_group* group) {
    ma_engine_play_sound(&audio_engine, file, group);
}

void Game::AudioSetConfig() {
    ma_sound_group_set_volume(&audio_master, (float)config["Master Volume"] / 100);
    ma_sound_group_set_volume(&audio_sfx, (float)config["SFX"] / 100);
    ma_sound_group_set_volume(&audio_music, (float)config["Music"] / 100);
}

bool Game::HandleEvents() {
    if (states.empty()) return false;
    return states.back()->HandleEvents();
}

bool Game::Update() {
    if (states.empty()) return false;
    return states.back()->Update();
}

bool Game::Draw() {
    if (states.empty()) return false;
    return states.back()->Draw();
}