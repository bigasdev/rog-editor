#include "Reader.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

std::vector<std::string> Reader::get_folders(std::string path)
{
        std::vector<std::string> folders;
        const std::filesystem::path realPath{ path };
        for (auto const& dir_entry : std::filesystem::directory_iterator{ realPath }){
            int extension = dir_entry.path().string().find(".");
            
            if(extension != -1){
                //folders.push_back(dir_entry.path().string());
                continue;
            }
            
            folders.push_back(dir_entry.path().string());
        }
        return folders;
}

std::string Reader::get_file_contents(std::string file)
{
  std::ifstream in(file);
  if(!in.is_open()){
    return "";
  }
  std::stringstream buffer;
  buffer << in.rdbuf();
  std::string contents = buffer.str();
  return contents;
}

std::vector<std::string> Reader::read_file(std::string path)
{
    std::vector<std::string> files;
        const std::filesystem::path realPath{ path };
        for (auto const& dir_entry : std::filesystem::directory_iterator{ realPath }){
            int extension = dir_entry.path().string().find(".");
            
            if(extension == -1){
                //folders.push_back(dir_entry.path().string());
                continue;
            }
            
            files.push_back(dir_entry.path().string());
        }
        return files;
}
std::vector<std::string> Reader::get_extension_files(std::string path, const char *ext)
{
    std::vector<std::string> files;
    const std::filesystem::path realPath{ path };
    for (const auto & entry : std::filesystem::directory_iterator{realPath})
    {
        if(entry.path().extension() == ext)
        {
            files.push_back(entry.path().string());
        }
    }

    return files;
}
