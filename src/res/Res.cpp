#include "SDL_gpu.h"
#include "SDL_pixels.h"
#include <string>
#define CUTE_ASEPRITE_IMPLEMENTATION
#include "../core/Engine.hpp"
#include "../core/SoundManager.hpp"
#include "../core/global.hpp"
#include "../tools/Reader.hpp"
#include "Res.hpp"
#include "SDL_render.h"
#include "cute_aseprite.h"

#include "../tools/Logger.hpp"
#include <filesystem>

namespace fs = std::filesystem;

std::string get_aseprite_edited_time(const char *file) {
  auto ftime = fs::last_write_time(file);
  auto ftime_since_epoch = ftime.time_since_epoch();

  auto now = std::chrono::system_clock::now();
  auto now_since_epoch = now.time_since_epoch();

  auto duration = ftime_since_epoch - now_since_epoch;
  auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
      now + duration);

  auto cftime = std::chrono::system_clock::to_time_t(sctp);

  return std::to_string(cftime);
}

GPU_Image *CreateTextureFromRGBA(SDL_Renderer *renderer, const uint8_t *rgba,
                                 int width, int height) {
  SDL_Surface *surface =
      SDL_CreateRGBSurfaceFrom((void *)rgba, width, height, 32, width * 4,
                               0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

  if (!surface) {
    std::cerr << "Failed to create SDL surface: " << SDL_GetError()
              << std::endl;
    return nullptr;
  }

  GPU_Image *texture = GPU_CopyImageFromSurface(surface);
  GPU_SetImageFilter(texture, GPU_FILTER_NEAREST);

  if (!texture) {
    std::cerr << "Failed to create SDL texture: " << SDL_GetError()
              << std::endl;
  }

  return texture;
}

Res::Res(SDL_Renderer *renderer) { m_renderer = renderer; }

Res::~Res() {}

void Res::init() {
  load_fonts();
  load_sounds();
  load_aseprites();
  //load_pallete();
  load_shaders();
}

void Res::load_fonts() {
  auto files = Reader::read_file("res/font");

  for (auto file : files) {
    std::string path = file;
    std::string file_name = path.substr(path.find_last_of("/\\") + 1);
    file_name = file_name.substr(0, file_name.find_last_of("."));
    Logger::log("Loading font: " + file_name);
    TTF_Font *font = TTF_OpenFont(path.c_str(), 8);
    if (font == nullptr) {
      Logger::error("Failed to load font: " + path);
    } else {
      Logger::log("Loaded font: " + path);
      m_fonts.insert(std::make_pair(file_name, font));
    }
  }
}

void Res::load_sounds() {
  auto sounds = Reader::get_extension_files("res/sounds", ".wav");

  for (auto sound : sounds) {
    std::string path = sound;
    std::string file_name = path.substr(path.find_last_of("/\\") + 1);
    file_name = file_name.substr(0, file_name.find_last_of("."));
    Logger::log("Loading sound: " + file_name);
    g_sound_manager->load_sound(path.c_str(), file_name);
  }
}

void Res::load_aseprites() {
  auto files = Reader::get_extension_files("res/", ".aseprite");
  m_aseprite_files.clear();

  if (m_aseprite_textures.size() > 0) {
    Logger::log("Detected changes!! Reloading...");
    for (auto const &file : files) {
      std::string fileName = file.substr(file.find_last_of("/") + 1);
      fileName = fileName.substr(0, fileName.find_last_of("."));

      GPU_Image **text = get_texture(fileName.c_str());

      if (text != nullptr) {
        ase_t *ase = cute_aseprite_load_from_file(file.c_str(), NULL);

        ase_frame_t *frame = &ase->frames[0];
        const uint8_t *rgba =
            reinterpret_cast<const uint8_t *>(frame->ase->frames[0].pixels);
        GPU_Image *texture = CreateTextureFromRGBA(
            m_renderer, rgba, frame->ase->w, frame->ase->h);

        AsepriteHelper asepriteHelper;
        asepriteHelper.file = file;
        asepriteHelper.last_edited_time =
            get_aseprite_edited_time(file.c_str());
        m_aseprite_files.push_back(asepriteHelper);

        auto it = m_aseprite_textures.find(fileName.c_str());
        it->second = texture;

        Logger::log("Aseprite file reloaded: " + file);
      }
    }

    return;
  }

  for (auto const &file : files) {
    // first we get just the file name without the aseprite extension
    std::string fileName = file.substr(file.find_last_of("/") + 1);
    fileName = fileName.substr(0, fileName.find_last_of("."));

    Logger::log(file.c_str());

    ase_t *ase = cute_aseprite_load_from_file(file.c_str(), NULL);

    ase_frame_t *frame = &ase->frames[0];
    const uint8_t *rgba =
        reinterpret_cast<const uint8_t *>(frame->ase->frames[0].pixels);
    GPU_Image *texture =
        CreateTextureFromRGBA(m_renderer, rgba, frame->ase->w, frame->ase->h);

    AsepriteHelper asepriteHelper;
    asepriteHelper.file = file;
    asepriteHelper.last_edited_time = get_aseprite_edited_time(file.c_str());
    m_aseprite_files.push_back(asepriteHelper);

    m_aseprite_textures.insert(
        std::pair<const char *, GPU_Image *>(fileName.c_str(), texture));
  }
  Logger::log("Finished aseprite load!");
  Logger::log("------------");
  for (auto const &aseprite : m_aseprite_textures) {
    Logger::log_group("Aseprite", aseprite.first);
  }
}

void Res::load_pallete() {
  std::string path = "res/sweetie-16-1x.png";

  GPU_Image *image = GPU_LoadImage(path.c_str());

  if (!image) {
    Logger::error("Failed to load pallete: " + path);
    return;
  }

  int w = image->w;
  int h = image->h;
  Logger::log("Loading pallete: " + path + " w: " + std::to_string(w) +
              " h: " + std::to_string(h));

  Uint8 *pixels = (Uint8 *)image->data;

  if (pixels == nullptr) {
    Logger::error("Failed to get pixels from pallete: " + path);
    return;
  }

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      int pixel = ((i * w) + j) * 3;
      Logger::log("Loading pixel: " + std::to_string(pixels[pixel]));
      Uint8 r, g, b, a;

      r = pixels[pixel+0];
      g = pixels[pixel+1];
      b = pixels[pixel+2];
      a = 255;

      Logger::log("Loading color : " + std::to_string(r) + " " +
                  std::to_string(g) + " " + std::to_string(b) + " " +
                  std::to_string(a));

      m_palette.push_back({r, g, b, a});
    }
  }

  GPU_FreeImage(image);
}

// FIX: To my older self..
//  this shader part needs to be rewritten to easily load more shaders, for now
//  its hard Loading remember for every frag we need a vert (and we can change
//  the .glsl to .frag and .vert)
void Res::load_shaders() {
  // auto files = Reader::get_extension_files("res/shaders", ".glsl");
  std::string shader_vert = "res/shaders/light_vertex.glsl";
  std::string shader_frag = "res/shaders/newoutline.frag.glsl";

  std::string vert_code = Reader::get_file_contents(shader_vert);
  std::string frag_code = Reader::get_file_contents(shader_frag);

  if (vert_code.empty() || frag_code.empty()) {
    Logger::error("Failed to read shader: " + shader_vert + " or " +
                  shader_frag);
    return;
  }

  auto vert_shader = GPU_CompileShader(GPU_VERTEX_SHADER, vert_code.c_str());
  if (!vert_shader) {
    Logger::error("Failed to compile shader: " + shader_vert);
    return;
  }

  auto frag_shader = GPU_CompileShader(GPU_FRAGMENT_SHADER, frag_code.c_str());
  if (!frag_shader) {
    Logger::error("Failed to compile shader: " + shader_frag);
    return;
  }

  Uint32 program = GPU_LinkShaders(vert_shader, frag_shader);
  if (!program) {
    Logger::error("Failed to link shader: " + shader_vert + " and " +
                  shader_frag);
    return;
  }

  GPU_ShaderBlock block =
      GPU_LoadShaderBlock(program, "gpu_Vertex", "gpu_TexCoord", "gpu_Color",
                          "gpu_ModelViewProjectionMatrix");

  GPU_ActivateShaderProgram(program, &block);
  m_shader_blocks.push_back(block);
  m_shaders_id.push_back(program);
  Logger::log("Loaded shader: " + shader_vert + " and " + shader_frag);

  /*for (auto file : files) {
    std::string path = file;
    std::string file_name = path.substr(path.find_last_of("/\\") + 1);
    file_name = file_name.substr(0, file_name.find_last_of("."));
    Logger::log("Loading shader: " + file_name);

    std::string shader_code = Reader::get_file_contents(file);
    if(shader_code.empty()){
      Logger::error("Failed to read shader: " + file);
      continue;
    }

    GPU_ShaderEnum shader_type;
    if (file_name.find("vertex") != std::string::npos) {
      shader_type = GPU_VERTEX_SHADER;
    } else if (file_name.find("fragment") != std::string::npos) {
      shader_type = GPU_FRAGMENT_SHADER;
    } else {
      Logger::error("Failed to get shader type: " + file);
      continue;
    }

    GPU_ShaderBlock block;
    auto shader = GPU_CompileShader(shader_type, shader_code.c_str());
    if (!shader) {
      Logger::error("Failed to compile shader: " + file);
      continue;
    }

    auto program = GPU_LinkShaders(shader, 0);
    if (!program) {
      Logger::error("Failed to link shader: " + file);
      GPU_FreeShader(shader);
      continue;
    }

    m_shaders.push_back(file);
  }*/
}

// FIX: needs to be rewrite later, this is just returning the light shader for
// now
Uint32 Res::get_shader_id() { return m_shaders_id[0]; }
// same as the above
GPU_ShaderBlock Res::get_shader_block() { return m_shader_blocks[0]; }

GPU_Image **Res::get_texture(std::string name) {
  try {
    return &m_aseprite_textures.at(name);
  } catch (const std::out_of_range &e) {
    Logger::error("Texture " + std::string(name) + " not found!");
    return nullptr;
  }
}

void Res::update() {
#if _DEBUG
  for (auto f : m_aseprite_files) {
    std::string new_edited_time = get_aseprite_edited_time(f.file.c_str());

    if (new_edited_time != f.last_edited_time) {
      Logger::log("Aseprite file changed: " + f.file);
      f.last_edited_time = new_edited_time;
      load_aseprites();
    }
  }
#endif
}
