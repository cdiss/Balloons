#ifndef BALLOON_HPP_
#define BALLOON_HPP_

#include "glm.h"

class Balloon {
public:
  Balloon();
  void randomizeForStart();
  void draw();
  void recompute();
  bool isOutOfBounds();
  static void setModel(GLMmodel*);

protected:
  float pos[3];
  float spd;
  static GLMmodel* pmodel;
};

#endif