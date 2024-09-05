#include "Logger.hpp"
#include <fstream>
#include <iomanip>

namespace Logger {
std::vector<std::string> logs;

void log(const std::string message) {
  logs.push_back(message);
#if _DEBUG
  std::cout << "LOG: " << message << "\n";
#endif
}

void log_group(const std::string name, const std::string message) {
  logs.push_back(name + ": " + message);
#if _DEBUG
  std::cout << std::left << std::setfill('.') << std::setw(20) << name
            << std::right << std::setfill('.') << std::setw(20) << message
            << "\n";
#endif
}

void error(const std::string message) {
  logs.push_back("ERROR: " + message);
#if _DEBUG
  std::cerr << "ERROR: " << message << "\n";
#endif
}

void write_to_file(const std::string file_name) {
  std::ofstream file(file_name);
  if (file.is_open()) {
    for (auto log : logs) {
      file << log << "\n";
    }
    file.close();
  } else {
    error("could not open file: " + file_name);
  }
}

void signal_handler(int signal) {
  error("Signal: " + std::to_string(signal));
  write_to_file("crash_log.txt");
  exit(signal);
}

void terminate_handler() { write_to_file("crash_log.txt"); }

void setup_crash_handlers() {
  signal(SIGSEGV, signal_handler);
  signal(SIGABRT, signal_handler);
  signal(SIGFPE, signal_handler);
  signal(SIGILL, signal_handler);
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  std::set_terminate(terminate_handler);
}
} // namespace Logger
