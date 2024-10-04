#pragma once 
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../tools/Common.hpp"

class GPU_Camera;

class Camera{
public: 
  Camera(vec2* size);
  ~Camera();

  void move();
  void update();

  GPU_Camera** get_gpu_cam();
  int get_game_scale() { return m_game_scale; }
  vec2 get_camera_pos();

  bool is_on_screen(vec2 pos);
  void track_pos(vec2* pos);
  void set_zoom(float zoom);
private:
  GPU_Camera *m_camera;
  vec2 m_pos;
  vec2 m_ref_pos;
  vec2 *m_size;
  vec2 *m_tracked_pos;

  int m_game_scale;

  //tracking stuff 
  bool clamp_bounds = false;
  float tracking_speed = 30.f;
  float base_frict = .4f;
  float dx;
  float dy;
  vec2 raw_focus;
  vec2 clamped_focus;
};

#endif
