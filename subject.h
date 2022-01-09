#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <vector>
#include "observer.h"

class Subject {
   std::vector<Observer*> observers;

 public:
   Subject();
   virtual ~Subject() = 0;
   // attach observer
   void attach( Observer *o );
   // detach observer
   void detach( Observer *o );
   // notify all observers on a change of state 
   void notifyObservers();
   
};

#endif
