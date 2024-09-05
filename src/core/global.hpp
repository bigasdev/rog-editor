#pragma once 
#ifndef GLOBAL_HPP
#define GLOBAL_HPP

class Engine;
class Res;
class SoundManager;
class Renderer;
class Camera;
class Cooldown;
class InputManager;

extern Engine* g_engine;
extern Res* g_res;
extern Renderer* g_renderer;
extern Camera* g_camera;
extern Cooldown* g_cooldown;
extern SoundManager* g_sound_manager;
extern InputManager* g_input_manager;

extern bool g_controller_connected;

#endif
