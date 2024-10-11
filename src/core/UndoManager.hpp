#pragma once 

#include <stack>
class IAction;

class UndoManager{
public:
  UndoManager();
  ~UndoManager();

  void add(IAction* action);
  void undo();
private:
  std::stack<IAction*> m_undo_stack;
};
