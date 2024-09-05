/** 
  @description used as a global time measurement tool, this will handle all the fps calculatino for dt (delta time) and fixed update stuff
**/

#ifndef TIMER_HPP
#define TIMER_HPP

#include "SDL.h"
#include "SDL_timer.h"

namespace Timer{
  //fixed update is calculated at 30fps so it runs in every computer
  static double wanted_fps = 30.0f;
  //slowmo
  static double tmod_multiplier = 1.0f;
  static double min_tmod_multiplier = 0.1f;

  static double max_delta_time = 0.5;

  static double smooth_factor = 0.95f;

  static double last_time_stamp = SDL_GetPerformanceCounter();

  static double elapsed_time = 0.0f;

  static int frame_count = 0;

  static double accumulator = 0.0f;

  static double dt = 1 / wanted_fps;
  static double fixed_dt = 1 / wanted_fps;
  static double current_dt = 1 / wanted_fps;

  static double fps = 0.0f;
  static double tmod = 0.0f;

  void update();
  double get_tmod();
  double get_dt();
  double get_accumulator();
  float get_fps();
  int get_frame_count();
  void skip();
  void fixed_t();
  void set_slow_mo(double slowmo);
  void apply_slow_mo(double slowmo);
  void reset_slow_mo();
}


#endif
