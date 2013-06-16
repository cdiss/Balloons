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
  static void setModelRed(GLMmodel*);
  static void setModelYellow(GLMmodel*);
  static void setModelBlue(GLMmodel*);
  static void setModelGreen(GLMmodel*);
    
protected:
  float pos[3];
  float spd;
  BalloonColor color;
  static GLMmodel* pmodelRed;
  static GLMmodel* pmodelYellow;
  static GLMmodel* pmodelBlue;
  static GLMmodel* pmodelGreen;
};

#endif