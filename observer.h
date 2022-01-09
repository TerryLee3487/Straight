#ifndef _OBSERVER_H_
#define _OBSERVER_H_

class Observer {
 public:
  // receive notifications from subject
  virtual void notify() = 0;
  virtual ~Observer();
};

#endif
