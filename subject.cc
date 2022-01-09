#include "subject.h"

Subject::Subject() {}
Subject::~Subject() {}

// attach observer
void Subject::attach( Observer *o ) {
  observers.emplace_back(o);
}

// detach observer
void Subject::detach( Observer *o ) {
  for ( auto it = observers.begin(); it != observers.end(); ++it ) {
    if ( *it == o ) {
      observers.erase(it);
      break;
    }
  }
}

// notify all observers on a change of state 
void Subject::notifyObservers() {
  for (auto ob : observers) ob->notify();
}
