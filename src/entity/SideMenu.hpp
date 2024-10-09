#pragma once 

enum State{
  ASSET,
  NONE
};

class SideMenu{
public:
  SideMenu();

  void show();
  State get_state() { return m_state; }
private: 
  State m_state = ASSET;
};
