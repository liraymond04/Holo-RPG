#include "RPG.h"
#include "example/MainMenu.h"

bool Holo::RPG::OnUserCreate() {
    ReadConfig();

    audio_result = ma_engine_init(NULL, &audio_engine);
    if (audio_result != MA_SUCCESS) {
        std::cout << "Failed to initialize audio engine"
                  << "\n";
        return false;
    }
    ma_sound_group_init(&audio_engine, 0, NULL, &audio_master);
    ma_sound_group_init(&audio_engine, 0, &audio_master, &audio_sfx);
    ma_sound_group_init(&audio_engine, 0, &audio_master, &audio_music);
    AudioSetConfig();

    splashScreen.SetOptions(2, 1, 2.0f, 0.5f, olc::BLACK, olc::BLUE,
                            olc::DARK_GREY, olc::WHITE);

    return true;
}

bool Holo::RPG::OnUserUpdate(float fElapsedTime) {
    if (splashScreen.AnimateSplashScreen(fElapsedTime))
        return true;
    else if (splashPlaying) {
        ChangeState(new MainMenu);
        splashPlaying = false;
    }
    return HandleEvents(fElapsedTime) && Update(fElapsedTime) &&
           Render(fElapsedTime);
}

void Holo::RPG::Cleanup() {
    ma_engine_uninit(&audio_engine);
    std::cout << "Finish"
              << "\n";
}

void Holo::RPG::ChangeState(GameState *state) {
    if (!states.empty()) {
        states.back()->Cleanup();
        states.pop_back();
    }

    states.push_back(state);
    states.back()->Init(this);
}

void Holo::RPG::PushState(GameState *state) {
    if (!states.empty()) {
        states.back()->Pause();
    }

    states.push_back(state);
    states.back()->Init(this);
}

void Holo::RPG::PopState() {
    if (!states.empty()) {
        states.back()->Cleanup();
        states.pop_back();
    }

    if (!states.empty()) {
        states.back()->Resume();
    }
}

void Holo::RPG::ReadConfig() {
    std::ifstream file;
    file.open("config.json");
    if (file.fail()) {
        std::cout << "Opening config.json failed"
                  << "\n";
        return;
    }
    config = json::parse(file);
    std::cout << "Opening config.json successful"
              << "\n";
}

void Holo::RPG::WriteConfig() {
    AudioSetConfig();
    std::ofstream file("config.json");
    file << config.dump(2);
}

void Holo::RPG::PlaySound(const char *file, ma_sound_group *group) {
    ma_engine_play_sound(&audio_engine, file, group);
}

void Holo::RPG::InitTrack(const char *file, ma_sound_group *group,
                          ma_sound *sound) {
    ma_sound_init_from_file(&audio_engine, file, MA_SOUND_FLAG_STREAM, group,
                            NULL, sound);
}

void Holo::RPG::AudioSetConfig() {
    ma_sound_group_set_volume(&audio_master,
                              (float)config["Master Volume"] / 100);
    ma_sound_group_set_volume(&audio_sfx, (float)config["SFX"] / 100);
    ma_sound_group_set_volume(&audio_music, (float)config["Music"] / 100);
}

bool Holo::RPG::HandleEvents(float fElapsedTime) {
    if (states.empty())
        return false;
    return states.back()->HandleEvents(fElapsedTime);
}

bool Holo::RPG::Update(float fElapsedTime) {
    if (states.empty())
        return false;
    return states.back()->Update(fElapsedTime);
}

bool Holo::RPG::Render(float fElapsedTime) {
    if (states.empty())
        return false;
    return states.back()->Draw(fElapsedTime);
}