#pragma once 
#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "../tools/Common.hpp"
#include <map>

// the sprite frame will assigned to the dst_x and dst_y variables of the sprite
struct SpriteFrame{
  int x, y;
  int frame;
  int max_frames;
  float frame_timer = 0.16f;
  bool loop = true;
};

struct Sprite{
  //positions 

  //basically an offset,this won't take the collisions into account
  float spr_x = 0.0f, spr_y = 0.0f;

  //scale

  //used to determine the position of the sprite in the Atlas
  float dst_x = 0, dst_y = 0;
  float wid = 16, hei = 16;
  float scale_x = 1.0f, scale_y = 1.0f;
  float squash_x = 1.0f, squash_y = 1.0f;

  //rotation 
  float angle = 0;

  //visibility
  int z_order = 0;
  int dir = 1;
  int alpha = 255;
  bool visible = true;

  //shaders 
  bool has_shader = false;
  int shader_id = 0;
  int shader_block_id = 0;
  PCol blink_color = WHITE;
  PCol outline_color = WHITE;
};

class SpriteAnimator{
  public:
    SpriteAnimator(Sprite spr);
    ~SpriteAnimator();

    void update(double dt);
    void register_anim(SpriteFrame frame, bool* state);
  private:
    Sprite m_spr;
    std::map<SpriteFrame, bool*> m_states;
    SpriteFrame m_current_frame;
    float m_timer = 0;
    bool m_playing = false;
};

#endif
