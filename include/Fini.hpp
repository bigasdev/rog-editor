#pragma once

#include "inicpp.h"
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

// read and save ini files, support for hot reload 
class Fini
{
public:
  Fini(std::string file) : filename(file) {
    load();
  }

  ~Fini() {
    save();
  }

  //hot reload
  bool update() {
    if (m_last_edited != std::to_string(std::filesystem::last_write_time(filename).time_since_epoch().count())) {
      m_last_edited = std::to_string(std::filesystem::last_write_time(filename).time_since_epoch().count());
      return true;
    }
    return false;
  }

  //save the current ini
  void save() {
    ini.save(filename);
  }

  //load the ini file
  void load() {
    ini.load(filename);
  }

  //initialize a value if it doesn't exist
  template <typename T>
  void initialize_value(std::string section, std::string key, T value) {
    if (ini[section][key].as<std::string>().empty()) {
      ini[section][key] = value;
    }
  }

  //grab a value from the ini file its useful for hot reload
  template <typename T>
  void grab_value(std::string section, std::string key, T *var) {
    try {
      *var = ini[section][key].as<T>();
    }
    catch (const std::exception& e) {
      *var = ini[section][key].as<T>();
    }
  }

  //get a value from the ini file
  template <typename T>
  T get_value(std::string section, std::string key) {
    try {
      return ini[section][key].as<T>();
    }
    catch (const std::exception& e) {
    }
  }

  //set a value in the ini file
  template <typename T>
  void set_value(std::string section, std::string key, T value) {
    ini[section][key] = value;
  }

private:
  ini::IniFile ini;
  std::string filename = "fini.ini";
  std::string m_last_edited;
};
