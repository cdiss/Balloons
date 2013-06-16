#ifndef BALLOON_HPP_
#define BALLOON_HPP_

#include "glm.h"

enum BalloonColor {
  RED,
  YELLOW,
  GREEN,
  BLUE,
  NONE
};

class Balloon {
public:
  Balloon(BalloonColor aColor = NONE);
  void randomizeForStart();
  void draw();
  void recompute();
  bool isOutOfBounds();
  static void setModel(GLMmodel*);
  static void setModelYellow(GLMmodel*);
  static void setModelBlue(GLMmodel*);
  static void setModelGreen(GLMmodel*);
    
protected:
  float pos[3];
  float spd;
  BalloonColor color;
  static GLMmodel* pmodel;
  static GLMmodel* pmodelYellow;
};

#endif