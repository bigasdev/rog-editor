#include "Sprite.hpp"
#include "../tools/Logger.hpp"

SpriteAnimator::SpriteAnimator(Sprite spr){
  m_spr = spr;
}

SpriteAnimator::~SpriteAnimator(){
}

void SpriteAnimator::update(double dt){
  /*
  if(m_playing){
    m_timer += dt;
    if(m_timer >= m_current_frame.frame_timer){
      m_timer = 0;
      m_current_frame.frame++;

      if(m_current_frame.frame >= m_current_frame.max_frames){
        if(m_current_frame.loop){
          m_current_frame.frame = 0;
        }else{
          m_playing = false;
        }
      }
    }
  }
  */
  for(auto& state : m_states){
    if(*state.second){
      m_current_frame = state.first;
      Logger::log("Playing animation " + std::to_string(m_current_frame.frame) + " " + std::to_string(m_current_frame.max_frames));
      m_playing = true;
      break;
    }
  }
}

void SpriteAnimator::register_anim(SpriteFrame frame, bool* state){
  m_states.insert(std::pair<SpriteFrame, bool*>(frame, state));
}
