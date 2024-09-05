#include "SoundManager.hpp"
#include "../tools/Logger.hpp"

SoundManager::SoundManager() {
  m_sounds = {};
  Logger::log(std::to_string(m_sounds.size()));
}

SoundManager::~SoundManager() {
  for (auto sound : m_sounds) {
    Mix_FreeChunk(sound.chunk);
  }
  Mix_FreeMusic(m_music);
  Mix_CloseAudio();
}

void SoundManager::load_sound(const char *path, std::string name) {
  Sound sound;
  sound.chunk = Mix_LoadWAV(path);
  if(sound.chunk == NULL){
    Logger::error("SoundManager::load_sound: failed to load sound " + name);
    return;
  }else{
    Logger::log("SoundManager::load_sound: loaded sound " + name);
  }
  sound.name = name;
  m_sounds.push_back(sound);
}

void SoundManager::play_sound(std::string name) {
  for (auto sound : m_sounds) {
    if (sound.name == name) {
      Mix_PlayChannel(-1, sound.chunk, 0);
      return;
    }
  }
  Logger::log("SoundManager::play_sound: sound not found");
}

void SoundManager::play_music() {
  Mix_PlayMusic(m_music, -1);
}

void SoundManager::stop_music() {
  Mix_HaltMusic();
}

void SoundManager::pause_music() {
  Mix_PauseMusic();
}

void SoundManager::resume_music() {
  Mix_ResumeMusic();
}

void SoundManager::set_volume(int volume) {
  m_volume = volume;
  Mix_Volume(-1, m_volume);
}

void SoundManager::set_music_volume(int volume) {
  m_music_volume = volume;
  Mix_VolumeMusic(m_music_volume);
}
