/** 
  @description used to log stuff to the console + save it to a .txt file, it handle some errors too 
**/

#pragma once
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <vector>

namespace Logger{
  extern std::vector<std::string> logs;

  void log(const std::string message);
  void log_group(const std::string name, const std::string message);
  void error(const std::string message);

  void write_to_file(const std::string file_name);
  void signal_handler(int signal);
  void terminate_handler();
  void setup_crash_handlers();
}

#endif
