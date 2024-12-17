#ifndef DISTANCE_H
#define DISTANCE_H

class Distance : private Dice{
       public:
  virtual void f() = 0; 
  template <class T>
T addDistance(T distance1, T distance2)
 {
 return distance1+distance2;
  }
};
#endif