#pragma once 

#ifndef PROFILER_HPP
#define PROFILER_HPP

#define PROFILER_TICK 60

struct ProfilerBaseInfo{
  int max_cpu_usage = 50;
  int max_ram_usage = 500;

  int cpu_line_y = 480;
  int cpu_max_line_y = 455;

  int ram_line_y = 550;
  int ram_max_line_y = 525;
};

class Profiler{
public: 
  Profiler();
  ~Profiler();

  void update();
  void draw();
private:
  ProfilerBaseInfo m_base_info;

  int m_cpu_usage = 0;
  int m_ram_usage = 0;
  int m_frames = 0;
  int m_calls = 0;
};

#endif
