#include "Sprite.hpp"
#include "../tools/Logger.hpp"
#include <utility>

SpriteAnimator::SpriteAnimator(Sprite spr){
  m_spr = spr;
}

SpriteAnimator::~SpriteAnimator(){
}

void SpriteAnimator::update(double dt){
  if(m_playing){
    m_timer += dt;
    if(m_timer >= m_current_frame.frame_timer){
      m_timer = 0;
      m_current_frame.frame++;
      if(m_current_frame.frame >= m_current_frame.max_frames){
        if(m_current_frame.loop){
          //disabling the block even in loop, this is used for animations that needs to be fully played before transitioning
          m_current_frame.block_transition = false;
          m_current_frame.frame = 0;
        }else{
          m_current_frame.block_transition = false;
          m_playing = false;
        }
      }
    }
    //animation moves only in the horizontal, no need for vertical support yet
    m_spr.dst_x = m_current_frame.x + (m_current_frame.frame * m_spr.wid);
    m_spr.dst_y = m_current_frame.y;

    Logger::log("Frame " + std::to_string(m_current_frame.frame) + " " + std::to_string(m_current_frame.max_frames));
  }

  //loop through the frames and check if the state is true
  for(auto& state : m_frames){
    if(state.state != nullptr and m_current_frame.name != state.name and m_current_frame.block_transition == false){
      if(*state.state){
        m_current_frame = state;
        Logger::log("Playing animation " + std::to_string(m_current_frame.frame) + " " + std::to_string(m_current_frame.max_frames));
        m_playing = true;
        break;
      }
    }
  }
}

void SpriteAnimator::register_anim(SpriteFrame frame){
  m_frames.push_back(frame);
}
