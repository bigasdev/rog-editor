#include "Timer.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include "../tools/Logger.hpp"

void Timer::update(){
  frame_count++;
  //this is the dt calculation
  auto new_time = SDL_GetPerformanceCounter();
  elapsed_time = (double)(new_time - last_time_stamp)/ (double)SDL_GetPerformanceFrequency();
  last_time_stamp = new_time;

  //ensures it never gets to a crazy value
  elapsed_time = std::min(elapsed_time, max_delta_time);
  accumulator += elapsed_time;
  current_dt = elapsed_time;
}

//this is the fixed delta time, it should be used for physics 
double Timer::get_tmod(){
  return dt*tmod_multiplier;
}

//delta time, use it for everything else and stuff that is at update() or post_update()
double Timer::get_dt(){
  return current_dt*tmod_multiplier;
}

double Timer::get_accumulator(){
  return accumulator;
}

float Timer::get_fps(){
  return 1/current_dt;
}

int Timer::get_frame_count(){
  return frame_count;
}
//skip a frame
void Timer::skip(){
  last_time_stamp = SDL_GetPerformanceCounter();
}

//hooked to the main.cpp while loop, this ensures the fixed delta time loop occurs
void Timer::fixed_t(){
  dt = fixed_dt;
  accumulator -= fixed_dt;
}
void Timer::set_slow_mo(double slowmo){
  tmod_multiplier = slowmo;
  tmod_multiplier = std::max(tmod_multiplier, min_tmod_multiplier);
  tmod_multiplier = std::min(tmod_multiplier, 5.0);
}
//apply slow_mo over time
void Timer::apply_slow_mo(double slowmo){
  tmod_multiplier -= slowmo;
  tmod_multiplier = std::max(tmod_multiplier, min_tmod_multiplier);
  tmod_multiplier = std::min(tmod_multiplier, 5.0);
  Logger::log("slowmo: " + std::to_string(tmod_multiplier));
}
void Timer::reset_slow_mo(){
  tmod_multiplier = 1.0;
}
