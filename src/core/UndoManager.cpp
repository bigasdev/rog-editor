#include "UndoManager.hpp"
#include "../entity/IAction.hpp"
#include "../tools/Logger.hpp"

UndoManager::UndoManager() {}

UndoManager::~UndoManager() {}

void UndoManager::add(IAction* action) {
  Logger::log("Adding action to undo stack");
  action->execute();
  m_undo_stack.push(action);
}

void UndoManager::undo() {
  if (m_undo_stack.empty()) {
    return;
  }
  Logger::log("Undoing last action");
  IAction* action = m_undo_stack.top();
  action->undo();
  m_undo_stack.pop();
  delete action;
}
